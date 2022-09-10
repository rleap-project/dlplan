#include "../../include/dlplan/state_space.h"

#include <experimental/filesystem>
#include <iostream>
#include <fstream>
#include <regex>

#include "../utils/command.h"
#include "../utils/tokenizer.h"


using namespace dlplan::core;


namespace dlplan::state_space {

static void parse_predicates_file(const std::string& filename, VocabularyInfo& vocabulary_info) {
    std::ifstream infile(filename);
    std::string name;
    int arity;
    while (infile >> name >> arity) {
        vocabulary_info.add_predicate(name, arity);
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
    IDENTIFIER
};

static const std::vector<std::pair<AtomTokenType, std::regex>> atom_token_regexes = {
    { AtomTokenType::COMMA, utils::Tokenizer<AtomTokenType>::build_regex(",") },
    { AtomTokenType::OPENING_PARENTHESIS, utils::Tokenizer<AtomTokenType>::build_regex("\\(") },
    { AtomTokenType::CLOSING_PARENTHESIS, utils::Tokenizer<AtomTokenType>::build_regex("\\)") },
    { AtomTokenType::IDENTIFIER, utils::Tokenizer<AtomTokenType>::build_regex("[a-zA-Z0-9_\\-]+") },
};

static void parse_atom(const std::string& atom_name, InstanceInfo& instance_info) {
    auto tokens = utils::Tokenizer<AtomTokenType>().tokenize(atom_name, atom_token_regexes);
    if (tokens.size() < 3) throw std::runtime_error("parse_atom - insufficient number of tokens: " + tokens.size());
    if (tokens[0].first != AtomTokenType::IDENTIFIER) throw std::runtime_error("parse_atom_line - expected predicate name at position 0.");
    if (tokens[1].first != AtomTokenType::OPENING_PARENTHESIS) throw std::runtime_error("parse_atom_line - expected opening parenthesis at position 1.");
    std::string predicate_name = tokens[0].second;
    std::vector<std::string> object_names;
    int i = 2; // position of first object_name
    while (i < static_cast<int>(tokens.size())) {
        if (tokens[i].first == AtomTokenType::CLOSING_PARENTHESIS) {
            break;
        } else if (tokens[i].first == AtomTokenType::COMMA) {
            ++i;
        } else {
            object_names.push_back(tokens[i].second);
            ++i;
        }
    }
    if (tokens.back().first != AtomTokenType::CLOSING_PARENTHESIS) throw std::runtime_error("parse_atom_line - expected closing parenthesis.");
    instance_info.add_atom(predicate_name, object_names);
}

static void parse_atoms_file(const std::string& filename, InstanceInfo& instance_info) {
   std::ifstream infile(filename);
   std::string name;
    while (infile >> name) {
        parse_atom(name, instance_info);
    }
}

static void parse_static_atoms_file(const std::string& filename, InstanceInfo& instance_info) {

}

static void parse_goal_atoms_file(const std::string& filename, InstanceInfo& instance_info) {

}

static std::pair<core::States, StateIndicesSet> parse_states_file(const std::string& filename, std::shared_ptr<InstanceInfo> instance_info) {

}

static AdjacencyMatrix parse_transitions_file(const std::string& filename, int num_states) {

}


StateSpace StateSpaceGenerator::generate_state_space(
    const std::string& domain_file,
    const std::string& instance_file) const {
    // Generate all necessary output files.
    // https://raymii.org/s/articles/Execute_a_command_and_get_both_output_and_exit_code.html
    std::stringstream command;
    command << std::getenv("DLPLAN_ROOT_DIR") << "/libs/scorpion/fast-downward.py"
            << " " << domain_file
            << " " << instance_file
            << " --translate-options"
            << " --dump-predicates"
            << " --dump-constants"
            << " --dump-static-atoms"
            << " --dump-goal-atoms"
            << " --search-options"
            << " --search"
            << " \"dump_reachable_search_space()\"";
    auto command_result = utils::Command::exec(command.str());
    std::ofstream state_space_file;
    state_space_file.open("planner.log");
    state_space_file << command_result.output;
    state_space_file.close();

    auto vocabulary_info = std::make_shared<core::VocabularyInfo>();
    parse_predicates_file("predicates.txt", *vocabulary_info);
    parse_constants_file("constants.txt", *vocabulary_info);
    auto instance_info = std::make_shared<core::InstanceInfo>(vocabulary_info);
    parse_atoms_file("atoms.txt", *instance_info);
    parse_static_atoms_file("static_atoms.txt", *instance_info);
    parse_goal_atoms_file("goal_atoms.txt", *instance_info);
    //auto parse_states_result = parse_states_file("states.txt", instance_info);
    //auto states = std::move(parse_states_result.first);
    //auto goal_state_indices = std::move(parse_states_result.second);
    //auto adjacency_matrix = parse_transitions_file("transition.txt", states.size());
    return StateSpace(std::move(instance_info), {}, {}, {}, {});
}

}