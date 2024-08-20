#include "../../include/dlplan/state_space.h"

#include "../utils/tokenizer.h"

#include <iostream>
#include <fstream>
#include <regex>
#include <sstream>


using namespace dlplan::core;
using namespace std::string_literals;

namespace dlplan::state_space::reader {

static void parse_predicates_file(const std::string& filename, VocabularyInfo& vocabulary_info, bool is_static) {
    std::ifstream infile(filename);
    std::string name;
    int arity;
    while (infile >> name >> arity) {
        vocabulary_info.add_predicate(name, arity, is_static);
        vocabulary_info.add_predicate(name + "_g", arity, true);
    }
}


static void parse_constants_file(const std::string& filename, VocabularyInfo& vocabulary_info) {
    std::ifstream infile(filename);
    std::string name;
    while (infile >> name) {
        vocabulary_info.add_constant(name);
    }
}


enum class AtomTokenType {
    COMMA,
    OPENING_PARENTHESIS,
    CLOSING_PARENTHESIS,
    NAME
};


static const std::vector<std::pair<AtomTokenType, std::regex>> atom_token_regexes = {
    { AtomTokenType::COMMA, utils::Tokenizer<AtomTokenType>::build_regex(",") },
    { AtomTokenType::OPENING_PARENTHESIS, utils::Tokenizer<AtomTokenType>::build_regex("\\(") },
    { AtomTokenType::CLOSING_PARENTHESIS, utils::Tokenizer<AtomTokenType>::build_regex("\\)") },
    { AtomTokenType::NAME, utils::Tokenizer<AtomTokenType>::build_regex("[a-zA-Z0-9_@\\-]+") },
};


static int parse_atom(const std::string& atom_name, dlplan::core::InstanceInfo& instance_info, bool is_static, bool is_goal) {
    auto tokens = utils::Tokenizer<AtomTokenType>().tokenize(atom_name, atom_token_regexes);
    if (tokens.size() < 3) throw std::runtime_error("parse_atom - insufficient number of tokens: " + std::to_string(tokens.size()));
    if (tokens[0].first != AtomTokenType::NAME) throw std::runtime_error("parse_atom_line - expected predicate name at position 0.");
    if (tokens[1].first != AtomTokenType::OPENING_PARENTHESIS) throw std::runtime_error("parse_atom_line - expected opening parenthesis at position 1.");
    std::string predicate_name = tokens[0].second;
    if (is_goal) {
        predicate_name += "_g";
    }
    if (predicate_name == "dummy") {
        return UNDEFINED;
    } else if (predicate_name.substr(0, 10) == "new-axiom@") {
        return UNDEFINED;
    }
    std::vector<std::string> object_names;
    int i = 2; // position of first object_name
    while (i < static_cast<int>(tokens.size())) {
        if (tokens[i].first == AtomTokenType::CLOSING_PARENTHESIS) {
            break;
        } else if (tokens[i].first == AtomTokenType::COMMA) {
            ++i;
        } else if (tokens[i].first == AtomTokenType::NAME) {
            object_names.push_back(tokens[i].second);
            ++i;
        } else {
            throw std::runtime_error("parse_atom_line - expected comma or name: " + tokens[i].second);
        }
    }
    if (tokens.back().first != AtomTokenType::CLOSING_PARENTHESIS) throw std::runtime_error("parse_atom_line - expected closing parenthesis.");
    const auto& atom = (is_static)
        ? instance_info.add_static_atom(predicate_name, object_names)
        : instance_info.add_atom(predicate_name, object_names);
    return atom.get_index();
}


static std::vector<int> parse_atoms_file(const std::string& filename, InstanceInfo& instance_info) {
    std::ifstream infile(filename);
    std::string name;
    std::vector<int> new_atom_indices;
    while (infile >> name) {
        new_atom_indices.push_back(parse_atom(name, instance_info, false, false));
    }
    return new_atom_indices;
}


static void parse_static_atoms_file(const std::string& filename, InstanceInfo& instance_info) {
    std::ifstream infile(filename);
    std::string name;
    std::vector<int> new_atom_indices;
    while (infile >> name) {
        new_atom_indices.push_back(parse_atom(name, instance_info, true, false));
    }
}


static void parse_goal_atoms_file(const std::string& filename, InstanceInfo& instance_info) {
    std::ifstream infile(filename);
    std::string name;
    std::vector<int> new_atom_indices;
    while (infile >> name) {
        new_atom_indices.push_back(parse_atom(name, instance_info, true, true));
    }
}


static std::pair<StateMapping, StateIndicesSet> parse_states_file(const std::string& filename, std::shared_ptr<InstanceInfo> instance_info, const std::vector<int>& new_atom_indices) {
    StateMapping states;
    StateIndicesSet goal_state_indices;
    std::ifstream infile(filename);
    std::string line;
    while (std::getline(infile, line)) {
        std::stringstream linestream(line);
        std::string type;
        linestream >> type;
        int state_index;
        linestream >> state_index;
        if (state_index != static_cast<int>(states.size())) {
            throw std::runtime_error("StateSpaceGenerator::parse_states_file - expected persistent indexing of states.");
        }
        if (type == "G") {
            goal_state_indices.insert(state_index);
        }
        std::vector<int> atom_indices;
        int atom_index;
        while (linestream >> atom_index) {
            int new_atom_index = new_atom_indices[atom_index];
            if (new_atom_index != UNDEFINED) {
                atom_indices.push_back(new_atom_index);
            }
        }
        auto result = states.emplace(state_index, core::State(state_index, instance_info, atom_indices));
        if (!result.second) {
            throw std::runtime_error("StateSpaceGenerator::parse_states_file - tried parsing duplicate states.");
        }
    }
    return std::make_pair(std::move(states), std::move(goal_state_indices));
}


static AdjacencyList parse_transitions_file(const std::string& filename) {
    std::ifstream infile(filename);
    int source_idx;
    int target_idx;
    AdjacencyList adjacency_list;
    while (infile >> source_idx >> target_idx) {
        adjacency_list[source_idx].insert(target_idx);
    }
    return adjacency_list;
}

static GeneratorExitCode parse_run_file(const std::string& filename) {
    std::ifstream infile(filename);
    std::string line;
    while (std::getline(infile, line)) {
        // [t=0.00182678s, 11492 KB] Time limit reached. Abort search.
        if (std::regex_search(line, std::regex("Time limit reached\\. Abort search\\.", std::regex_constants::ECMAScript))) {
            return GeneratorExitCode::INCOMPLETE;
        }
        // [t=0.00408488s, 11316 KB] Num states limit reached. Abort search.
        else if (std::regex_search(line, std::regex("Num states limit reached\\. Abort search\\.", std::regex_constants::ECMAScript))) {
            return GeneratorExitCode::INCOMPLETE;
        }
        // [t=0.000984712s, 11492 KB] Finished dumping the reachable state space.
        else if (std::regex_search(line, std::regex("Finished dumping the reachable state space\\.", std::regex_constants::ECMAScript))) {
            return GeneratorExitCode::COMPLETE;
        }
    }
    return GeneratorExitCode::FAIL;
}

GeneratorResult read(std::shared_ptr<VocabularyInfo> vocabulary_info, int index) {
    auto exit_code = parse_run_file("run.log");
    if (exit_code != GeneratorExitCode::COMPLETE) {
        return GeneratorResult{
            exit_code,
            nullptr
        };
    }
    if (!vocabulary_info) {
        std::shared_ptr<VocabularyInfo> new_vocabulary_info = std::make_shared<core::VocabularyInfo>();
        parse_predicates_file("predicates.txt", *new_vocabulary_info, false);
        /*
         we parse static predicates as non static ones because
         we want to ensure we cannot deduce this information from
         a spefic instance of the domain
        */
        parse_predicates_file("static-predicates.txt", *new_vocabulary_info, false);
        parse_constants_file("constants.txt", *new_vocabulary_info);
        vocabulary_info = new_vocabulary_info;
    }
    auto instance_info = std::make_shared<core::InstanceInfo>(index, vocabulary_info);
    auto new_atom_indices = parse_atoms_file("atoms.txt", *instance_info);
    parse_static_atoms_file("static-atoms.txt", *instance_info);
    parse_goal_atoms_file("goal-atoms.txt", *instance_info);
    auto parse_states_result = parse_states_file("states.txt", instance_info, new_atom_indices);
    auto states = std::move(parse_states_result.first);
    auto goal_state_indices = std::move(parse_states_result.second);
    auto adjacency_list = parse_transitions_file("transitions.txt");
    int initial_state_index = 0;
    return GeneratorResult{
        exit_code,
        std::make_shared<StateSpace>(std::move(instance_info), std::move(states), initial_state_index, std::move(adjacency_list), std::move(goal_state_indices))
    };
}

}