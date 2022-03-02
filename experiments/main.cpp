#include <fstream>
#include <iostream>
#include <cassert>
#include <string>
#include <memory>
#include <regex>
#include <unordered_set>
#include <chrono>
#include <unistd.h>

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
    const auto& predicate = vocabulary_info.add_predicate(match[1], std::atoi(match[2].str().c_str()));
}

void parse_constant_line(dlplan::core::VocabularyInfo& vocabulary_info, const std::string& line) {
    std::smatch match;
    std::regex_search(line, match, std::regex("(.*)"));
    const auto& constant = vocabulary_info.add_constant(match[1]);
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
    { AtomTokenType::IDENTIFIER, build_regex("[a-zA-Z_\\-]\\w*") },
};

void parse_atom_line(dlplan::core::InstanceInfo& instance_info, const std::string& line, int idx, const std::unordered_set<int>& static_atom_idxs, std::unordered_map<std::string, dlplan::core::Atom>& atom_name_to_atom) {
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
    const auto atom = (static_atom_idxs.find(idx) != static_atom_idxs.end())
        ? instance_info.add_static_atom(predicate_name, object_names)
        : instance_info.add_atom(predicate_name, object_names);
    atom_name_to_atom.emplace(line, atom);
}

std::shared_ptr<const dlplan::core::InstanceInfo> construct_instance_info(
    std::shared_ptr<const dlplan::core::VocabularyInfo> vocabulary_info,
    const std::vector<std::string>& objects_lines,
    const std::vector<std::string>& atoms_lines,
    const std::vector<std::string>& static_atom_indices_lines,
    std::unordered_map<std::string, dlplan::core::Atom>& atom_name_to_atom) {
    std::shared_ptr<dlplan::core::InstanceInfo> instance_info = std::make_shared<dlplan::core::InstanceInfo>(vocabulary_info);
    std::unordered_set<int> static_atom_idxs;
    for (const auto& line : static_atom_indices_lines) {
        parse_static_atom_idx_line(static_atom_idxs, line);
    }
    int idx = 0;
    for (const auto& line : atoms_lines) {
        parse_atom_line(*instance_info, line, idx, static_atom_idxs, atom_name_to_atom);
        ++idx;
    }
    return instance_info;
}

dlplan::core::SyntacticElementFactory construct_factory(
    std::shared_ptr<const dlplan::core::VocabularyInfo> vocabulary_info) {
    return dlplan::core::SyntacticElementFactory(vocabulary_info);
}

enum class StateTokenType {
    COMMA,
    OPENING_BRACE,
    CLOSING_BRACE,
    IDENTIFIER
};

static const std::vector<std::pair<StateTokenType, std::regex>> state_token_type_expressions = {
    { StateTokenType::COMMA, build_regex(",") },
    { StateTokenType::OPENING_BRACE, build_regex("\\{") },
    { StateTokenType::CLOSING_BRACE, build_regex("\\}") },
    { StateTokenType::IDENTIFIER, build_regex("(\\S*)[,}]") },
};

dlplan::core::State parse_state_line(
    std::shared_ptr<const dlplan::core::InstanceInfo> instance_info,
    const std::unordered_map<std::string, dlplan::core::Atom>& atom_name_to_atom,
    const std::string& line) {
    auto start = line.begin();
    const auto end = line.end();
    std::smatch match;
    std::vector<std::pair<StateTokenType, std::string>> tokens;
    while (start != end) {
        bool has_match = false;
        for (const auto& pair : state_token_type_expressions) {
            std::regex regex = pair.second;
            if (std::regex_search(start, end, match, regex)) {
                tokens.emplace_back(pair.first, match[2].str());
                start += match[0].str().size();
                has_match = true;
            }
        }
        if (!has_match) {
            throw std::runtime_error("parse_state_line - cannot parse line into state. Line is: " + line);
        }
    }
    std::vector<int> atom_idxs;
    for (int i = 1; i < tokens.size(); ++i) {
        if (tokens[i].first != StateTokenType::IDENTIFIER) std::runtime_error("parse_state_line - expected identifier at position " + std::to_string(i) + ".");
        const auto& atom = atom_name_to_atom.at(tokens[i].second);
        if (!atom.get_is_static()) atom_idxs.push_back(atom.get_index());
    }
    return dlplan::core::State(instance_info, atom_idxs);
}

std::vector<dlplan::core::State> construct_states(
    std::shared_ptr<const dlplan::core::InstanceInfo> instance_info,
    const std::unordered_map<std::string, dlplan::core::Atom>& atom_name_to_atom,
    std::vector<std::string> states_lines) {
    std::vector<dlplan::core::State> states;
    for (const auto& line : states_lines) {
        states.push_back(parse_state_line(instance_info, atom_name_to_atom, line));
    }
    return states;
}

std::vector<dlplan::core::Boolean> parse_boolean_features(
    dlplan::core::SyntacticElementFactory& factory,
    std::vector<std::string> boolean_feature_lines) {
    std::vector<dlplan::core::Boolean> result;
    for (const auto& line : boolean_feature_lines) {
        result.push_back(factory.parse_boolean(line));
    }
    return result;
}

std::vector<dlplan::core::Numerical> parse_numerical_features(
    dlplan::core::SyntacticElementFactory& factory,
    std::vector<std::string> numerical_feature_lines) {
    std::vector<dlplan::core::Numerical> result;
    for (const auto& line : numerical_feature_lines) {
        result.push_back(factory.parse_numerical(line));
    }
    return result;
}

int main(int argc, char** argv) {
    std::cout << argc << std::endl;
    for (int i = 0; i < argc; ++i) {
        std::cout << argv[i] << std::endl;
    }
    if (argc != 4) {
        std::cout << "User error. Expected: ./experiment_core <str:states.txt> <str:features.txt> <int:num_iterations>" << std::endl;
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
    std::vector<std::string> boolean_features_lines;
    std::vector<std::string> numerical_features_lines;
    if (file2.is_open()) {
        read_and_verify_line(file2, "Boolean features:");
        boolean_features_lines = read_lines_until_empty_line(file2);
        read_and_verify_line(file2, "Numerical features:");
        numerical_features_lines = read_lines_until_empty_line(file2);
        file2.close();
    }

    std::unordered_map<std::string, dlplan::core::Atom> atom_name_to_atom;
    std::shared_ptr<const dlplan::core::VocabularyInfo> vocabulary_info = construct_vocabulary_info(predicates_lines, constants_lines);
    std::shared_ptr<const dlplan::core::InstanceInfo> instance_info = construct_instance_info(vocabulary_info, objects_lines, atoms_lines, static_atom_indices_lines, atom_name_to_atom);
    std::vector<dlplan::core::State> states = construct_states(instance_info, atom_name_to_atom, states_lines);
    dlplan::core::SyntacticElementFactory factory = construct_factory(vocabulary_info);
    std::vector<dlplan::core::Boolean> boolean_features = parse_boolean_features(factory, boolean_features_lines);
    std::vector<dlplan::core::Numerical> numerical_features = parse_numerical_features(factory, numerical_features_lines);

    std::cout << "Started feature evaluation" << std::endl;
    auto start = std::chrono::steady_clock::now();
    for (int i = 0; i < std::atoi(argv[3]); ++i) {
        for (const auto& state : states) {
            for (const auto& boolean : boolean_features) {
                boolean.evaluate(state);
            }
            for (const auto& numerical : numerical_features) {
                numerical.evaluate(state);
            }
        }
    }
    auto end = std::chrono::steady_clock::now();
    std::cout << "Elapsed time in milliseconds: "
        << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()
        << " ms" << std::endl;
    return 0;
}