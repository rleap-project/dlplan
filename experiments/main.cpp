#include <fstream>
#include <iostream>
#include <cassert>
#include <string>
#include <memory>
#include <regex>
#include <unordered_set>

#include <dlplan/core.h>

std::string read_and_verify_line(std::ifstream& file, std::string content) {
    std::string line;
    std::getline(file,line);
    if (line != content) {
        throw std::runtime_error("Unexpected line: " + line + ". Expected: " + content);
    }
    return line;
}

std::vector<std::string> read_lines_until_empty_line(std::ifstream& file) {
    std::vector<std::string> result;
    std::string line;
    while ( std::getline (file,line) ) {
        if (line == "") break;
        result.push_back(line);
    }
    return result;
}

void parse_predicate_line(dlplan::core::VocabularyInfo& vocabulary_info, const std::string& line) {
    std::smatch match;
    std::regex_search(line, match, std::regex("(.*) (.*)"));
    vocabulary_info.add_predicate(match[1], std::atoi(match[2].str().c_str()));
}

void parse_constant_line(dlplan::core::VocabularyInfo& vocabulary_info, const std::string& line) {
    std::smatch match;
    std::regex_search(line, match, std::regex("(.*)"));
    vocabulary_info.add_constant(match[1]);
}

std::shared_ptr<const dlplan::core::VocabularyInfo> construct_vocabulary_info(
    const std::vector<std::string>& predicates_lines,
    const std::vector<std::string>& constants_lines) {
    std::shared_ptr<dlplan::core::VocabularyInfo> vocabulary_info = std::make_shared<dlplan::core::VocabularyInfo>();
    for (const auto& line : predicates_lines) {
        parse_predicate_line(*vocabulary_info, line);
    }
    for (const auto& line : constants_lines) {
        parse_constant_line(*vocabulary_info, line);
    }
    return vocabulary_info;
}

void parse_static_atom_idx_line(std::unordered_set<int>& static_atom_idxs, const std::string& line) {
    std::smatch match;
    std::regex_search(line, match, std::regex("(.*)"));
    static_atom_idxs.insert(std::atoi(match[1].str().c_str()));
}


enum class AtomTokenType {
    COMMA,
    OPENING_PARENTHESIS,
    CLOSING_PARENTHESIS,
    IDENTIFIER
};

static std::regex build_regex(const std::string &s, std::regex::flag_type f = std::regex_constants::ECMAScript) {
    return std::regex("^\\s*(" + s + ")\\s*", f);
}

static const std::vector<std::pair<AtomTokenType, std::regex>> atom_token_type_expressions = {
    { AtomTokenType::COMMA, build_regex(",") },
    { AtomTokenType::OPENING_PARENTHESIS, build_regex("\\(") },
    { AtomTokenType::CLOSING_PARENTHESIS, build_regex("\\)") },
    { AtomTokenType::IDENTIFIER, build_regex("[a-zA-Z_-]\\w*") },
};

void parse_atom_line(dlplan::core::InstanceInfo& instance_info, const std::string& line, int idx, const std::unordered_set<int>& static_atom_idxs) {
    auto start = line.begin();
    const auto end = line.end();
    std::smatch match;
    std::vector<std::pair<AtomTokenType, std::string>> tokens;
    while (start != end) {
        bool has_match = false;
        for (const auto& pair : atom_token_type_expressions) {
            std::regex regex = pair.second;
            if (std::regex_search(start, end, match, regex)) {
                tokens.emplace_back(pair.first, match[1].str());
                start += match[0].str().size();
                has_match = true;
            }
        }
        if (!has_match) {
            throw std::runtime_error("parse_atom_line - cannot parse line into atom. Line is: " + line);
        }
    }
    if (tokens.size() < 1) throw std::runtime_error("parse_atom_line - isufficient number of tokens: " + tokens.size());
    if (tokens[0].first != AtomTokenType::IDENTIFIER) throw std::runtime_error("parse_atom_line - expected identifier at position 0.");
    std::string predicate_name = tokens[0].second;
    std::vector<std::string> object_names;
    for (int i = 2; i < static_cast<int>(tokens.size()); i += 2) {
        if (tokens[i].first != AtomTokenType::IDENTIFIER) throw std::runtime_error("parse_atom_line - expected identifier at position " + std::to_string(i) + ".");
        object_names.push_back(tokens[i].second);
    }
    if (static_atom_idxs.find(idx) == static_atom_idxs.end()) {
        instance_info.add_static_atom(predicate_name, object_names);
    } else {
        instance_info.add_atom(predicate_name, object_names);
    }
}

std::shared_ptr<const dlplan::core::InstanceInfo> construct_instance_info(
    std::shared_ptr<const dlplan::core::VocabularyInfo> vocabulary_info,
    const std::vector<std::string>& objects_lines,
    const std::vector<std::string>& atoms_lines,
    const std::vector<std::string>& static_atom_indices_lines) {
    std::shared_ptr<dlplan::core::InstanceInfo> instance_info = std::make_shared<dlplan::core::InstanceInfo>(vocabulary_info);
    std::unordered_set<int> static_atom_idxs;
    for (const auto& line : static_atom_indices_lines) {
        parse_static_atom_idx_line(static_atom_idxs, line);
    }
    int idx = 0;
    for (const auto& line : atoms_lines) {
        parse_atom_line(*instance_info, line, idx, static_atom_idxs);
        ++idx;
    }
    return instance_info;
}

dlplan::core::SyntacticElementFactory construct_factory(
    std::shared_ptr<const dlplan::core::VocabularyInfo> vocabulary_info) {
    return dlplan::core::SyntacticElementFactory(vocabulary_info);
}

dlplan::core::State parse_state_line(
    std::shared_ptr<const dlplan::core::InstanceInfo> instance_info,
    const std::string& line) {
    auto start = line.begin();
    const auto end = line.end();
    std::smatch match;
    std::regex_search(start, end, match, std::regex("(.*)\\("));
    std::string predicate_name = match[1].str();
    start += match[0].str().size();
    std::vector<std::string> object_names;
    // {at(tray1,kitchen), at(tray3,kitchen), at(tray2,kitchen), at(tray4,kitchen), at_kitchen_bread(bread19), at_kitchen_bread(bread18), at_kitchen_bread(bread14), at_kitchen_bread(bread8), at_kitchen_bread(bread10), at_kitchen_bread(bread2), at_kitchen_bread(bread9), at_kitchen_bread(bread16), at_kitchen_bread(bread20), at_kitchen_bread(bread17), at_kitchen_bread(bread7), at_kitchen_bread(bread24), at_kitchen_bread(bread12), at_kitchen_bread(bread3), at_kitchen_bread(bread22), at_kitchen_bread(bread13), at_kitchen_bread(bread11), at_kitchen_bread(bread5), at_kitchen_bread(bread6), at_kitchen_bread(bread21), at_kitchen_bread(bread15), at_kitchen_bread(bread1), at_kitchen_bread(bread23), at_kitchen_content(content22), at_kitchen_content(content14), at_kitchen_content(content6), at_kitchen_content(content19), at_kitchen_content(content13), at_kitchen_content(content20), at_kitchen_content(content23), at_kitchen_content(content2), at_kitchen_content(content8), at_kitchen_content(content10), at_kitchen_content(content16), at_kitchen_content(content15), at_kitchen_content(content1), at_kitchen_content(content12), at_kitchen_content(content3), at_kitchen_content(content17), at_kitchen_content(content11), at_kitchen_content(content4), at_kitchen_content(content5), at_kitchen_content(content9), at_kitchen_content(content18), at_kitchen_content(content7), at_kitchen_content(content24), notexist(sandw27), notexist(sandw18), notexist(sandw21), notexist(sandw32), notexist(sandw9), notexist(sandw13), notexist(sandw20), notexist(sandw25), notexist(sandw28), notexist(sandw11), notexist(sandw4), notexist(sandw8), notexist(sandw19), notexist(sandw29), notexist(sandw14), notexist(sandw17), notexist(sandw5), notexist(sandw24), notexist(sandw15), notexist(sandw16), notexist(sandw1), notexist(sandw22), notexist(sandw30), notexist(sandw3), notexist(sandw6), notexist(sandw23), notexist(sandw10), notexist(sandw7), notexist(sandw31), notexist(sandw12), notexist(sandw26), at_kitchen_sandwich(sandw2), no_gluten_sandwich(sandw2), not_allergic_gluten(child12), not_allergic_gluten(child21), not_allergic_gluten(child23), not_allergic_gluten(child16), not_allergic_gluten(child10), not_allergic_gluten(child17), not_allergic_gluten(child4), not_allergic_gluten(child18), not_allergic_gluten(child19), not_allergic_gluten(child24), not_allergic_gluten(child13), not_allergic_gluten(child3), not_allergic_gluten(child20), not_allergic_gluten(child7), not_allergic_gluten(child1), allergic_gluten(child9), allergic_gluten(child22), allergic_gluten(child6), allergic_gluten(child8), allergic_gluten(child5), allergic_gluten(child14), allergic_gluten(child15), allergic_gluten(child2), allergic_gluten(child11), no_gluten_bread(bread13), no_gluten_bread(bread4), no_gluten_bread(bread18), no_gluten_bread(bread20), no_gluten_bread(bread23), no_gluten_bread(bread5), no_gluten_bread(bread22), no_gluten_bread(bread14), no_gluten_bread(bread10), no_gluten_content(content7), no_gluten_content(content14), no_gluten_content(content2), no_gluten_content(content21), no_gluten_content(content4), no_gluten_content(content5), no_gluten_content(content22), no_gluten_content(content17), no_gluten_content(content8), waiting(child3,table2), waiting(child23,table2), waiting(child6,table2), waiting(child5,table2), waiting(child16,table1), waiting(child22,table3), waiting(child12,table1), waiting(child11,table2), waiting(child1,table2), waiting(child18,table1), waiting(child13,table3), waiting(child9,table2), waiting(child24,table2), waiting(child7,table3), waiting(child20,table1), waiting(child8,table1), waiting(child2,table3), waiting(child10,table2), waiting(child4,table2), waiting(child19,table1), waiting(child21,table3), waiting(child17,table1), waiting(child14,table1), waiting(child15,table2), served_g(child1), served_g(child2), served_g(child3), served_g(child4), served_g(child5), served_g(child6), served_g(child7), served_g(child8), served_g(child9), served_g(child10), served_g(child11), served_g(child12), served_g(child13), served_g(child14), served_g(child15), served_g(child16), served_g(child17), served_g(child18), served_g(child19), served_g(child20), served_g(child21), served_g(child22), served_g(child23), served_g(child24)}

    while (start != end) {
        bool has_match = false;
        for (const auto& regex : {std::regex("(.*),"), std::regex("(.*)\\)")}) {
            if (std::regex_search(start, end, match, regex)) {
                object_names.push_back(match[1].str());
                start += match[0].str().size();
                has_match = true;
            }
        }
        if (!has_match) {
            std::runtime_error("parse_atom_line - cannot parse line into atom. Line is: " + line);
        }
    }
}

std::vector<dlplan::core::State> construct_states(
    std::shared_ptr<const dlplan::core::InstanceInfo> instance_info,
    std::vector<std::string> states_lines) {
    std::vector<dlplan::core::State> states;
    return states;
}

int main(int argc, char** argv) {
    std::cout << argc << std::endl;
    for (int i = 0; i < argc; ++i) {
        std::cout << argv[i] << std::endl;
    }
    if (argc != 3) {
        std::cout << "User error. Expected: ./experiment_core <states.txt> <features.txt>" << std::endl;
        return 1;
    }
    std::ifstream file1(argv[1]);
    std::string line;
    std::vector<std::string> predicates_lines;
    std::vector<std::string> constants_lines;
    std::vector<std::string> objects_lines;
    std::vector<std::string> atoms_lines;
    std::vector<std::string> static_atom_indices_lines;
    std::vector<std::string> states_lines;
    if (file1.is_open()) {
        read_and_verify_line(file1, "Predicates:");
        predicates_lines = read_lines_until_empty_line(file1);
        read_and_verify_line(file1, "Constants:");
        constants_lines = read_lines_until_empty_line(file1);
        read_and_verify_line(file1, "Objects:");
        objects_lines = read_lines_until_empty_line(file1);
        read_and_verify_line(file1, "Atoms:");
        atoms_lines = read_lines_until_empty_line(file1);
        read_and_verify_line(file1, "Static atom indices:");
        static_atom_indices_lines = read_lines_until_empty_line(file1);
        read_and_verify_line(file1, "States:");
        states_lines = read_lines_until_empty_line(file1);
        file1.close();
    }

    std::ifstream file2;
    file2.open(argv[2]);
    if (file2.is_open()) {
        read_and_verify_line(file2, "Boolean features:");
        std::vector<std::string> boolean_features_lines = read_lines_until_empty_line(file2);
        read_and_verify_line(file2, "Numerical features:");
        std::vector<std::string> numerical_features_lines = read_lines_until_empty_line(file2);
        file2.close();
    }

    std::shared_ptr<const dlplan::core::VocabularyInfo> vocabulary_info = construct_vocabulary_info(predicates_lines, constants_lines);
    std::shared_ptr<const dlplan::core::InstanceInfo> instance_info = construct_instance_info(vocabulary_info, objects_lines, atoms_lines, static_atom_indices_lines);
    std::vector<dlplan::core::State> states = construct_states(instance_info, states_lines);
    return 0;
}