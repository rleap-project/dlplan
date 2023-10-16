/// @brief Provides functionality for serialization of the whole library state.

#ifndef DLPLAN_INCLUDE_DLPLAN_SERIALIZATION_H_
#define DLPLAN_INCLUDE_DLPLAN_SERIALIZATION_H_

#include <istream>
#include <unordered_map>

#include "core.h"
#include "novelty.h"
#include "policy.h"
#include "state_space.h"

using namespace dlplan;


namespace dlplan::serialization {

/// @brief Encapsulates a forest of library objects for serialization and deserialization.
///
/// The key is a string to allow the user the encode more complex nestings.
/// The value is a shared_ptr<T> to the allow sharing between other objects.
struct Data {
    // core
    std::unordered_map<std::string, std::shared_ptr<core::VocabularyInfo>> vocabulary_infos;
    std::unordered_map<std::string, std::shared_ptr<core::InstanceInfo>> instance_infos;
    std::unordered_map<std::string, std::shared_ptr<core::SyntacticElementFactory>> syntatic_element_factories;
    std::unordered_map<std::string, std::shared_ptr<core::DenotationsCaches>> denotations_caches;

    // state_space
    std::unordered_map<std::string, std::shared_ptr<state_space::StateSpace>> state_spaces;

    // novelty
    std::unordered_map<std::string, std::shared_ptr<novelty::TupleGraph>> tuple_graphs;

    // policy
    std::unordered_map<std::string, std::shared_ptr<const policy::Policy>> policies;
    std::unordered_map<std::string, std::shared_ptr<policy::PolicyFactory>> policy_factories;
};

/// @brief Serializes the data and places it into the out_buffer.
/// @param data
/// @param out_buffer
void serialize(const Data& data, std::ostream& out_buffer);

/// @brief Deserializes the data from the buffer.
/// @param buffer
/// @return
Data deserialize(std::istream& buffer);

}

#endif
