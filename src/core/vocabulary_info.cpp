#include "vocabulary_info.h"

#include "instance_info.h"
#include "element_factory.h"

#include "../utils/collections.h"
#include "../utils/hash_utils.h"

namespace dlplan::core {

VocabularyInfoImpl::VocabularyInfoImpl() { }

const Predicate& VocabularyInfoImpl::add_predicate(const std::string &predicate_name, int arity) {
    Predicate predicate = Predicate(predicate_name, m_predicates.size(), arity);
    auto result = m_predicate_name_to_predicate_idx.emplace(predicate_name, m_predicates.size());
    if (!result.second) {
        throw std::runtime_error("VocabularyInfoImpl::add_predicate - predicate with name ("s + predicate_name + ") already exists.");
    }
    m_predicates.push_back(std::move(predicate));
    return m_predicates.back();
}

const Constant& VocabularyInfoImpl::add_constant(const std::string& constant_name) {
    Constant constant = Constant(constant_name, m_constants.size());
    auto result = m_constant_name_to_constant_idx.emplace(constant_name, m_constants.size());
    if (!result.second) {
        throw std::runtime_error("VocabularyInfoImpl::add_constant - constant with name ("s + constant_name + ") already exists.");
    }
    m_constants.push_back(std::move(constant));
    return m_constants.back();
}

bool VocabularyInfoImpl::exists_predicate(const Predicate& predicate) const {
    if (!utils::in_bounds(predicate.get_index(), m_predicates)) {
        throw std::runtime_error("VocabularyInfoImpl::exists_predicate: predicate index out of range.");
    }
    return (m_predicates[predicate.get_index()] == predicate) ? true : false;
}

bool VocabularyInfoImpl::exists_predicate_name(const std::string& name) const {
    return m_predicate_name_to_predicate_idx.find(name) != m_predicate_name_to_predicate_idx.end();
}

const std::vector<Predicate>& VocabularyInfoImpl::get_predicates() const {
    return m_predicates;
}

int VocabularyInfoImpl::get_predicate_idx(const std::string& name) const {
    if (!exists_predicate_name(name)) {
        throw std::runtime_error("VocabularyInfoImpl::get_predicate_idx - no predicate with name ("s + name + ").");
    }
    return m_predicate_name_to_predicate_idx.at(name);
}

const Predicate& VocabularyInfoImpl::get_predicate(int predicate_idx) const {
    if (!utils::in_bounds(predicate_idx, m_predicates)) {
        throw std::runtime_error("VocabularyInfoImpl::get_predicate - predicate index out of range.");
    }
    return m_predicates[predicate_idx];
}

bool VocabularyInfoImpl::exists_constant(const Constant& constant) const {
    if (!utils::in_bounds(constant.get_index(), m_constants)) {
        throw std::runtime_error("VocabularyInfoImpl::exists_constant: constant index out of range.");
    }
    // we only need to check the position with the corresponding index.
    return (m_constants[constant.get_index()] == constant) ? true : false;
}

bool VocabularyInfoImpl::exists_constant_name(const std::string& name) const {
    return m_constant_name_to_constant_idx.find(name) != m_constant_name_to_constant_idx.end();
}

int VocabularyInfoImpl::get_constant_idx(const std::string& name) const {
    if (!exists_constant_name(name)) {
        throw std::runtime_error("VocabularyInfoImpl::get_constant_idx - no constant with name ("s + name + ").");
    }
    return m_constant_name_to_constant_idx.at(name);
}

const Constant& VocabularyInfoImpl::get_constant(int constant_idx) const {
    if (!utils::in_bounds(constant_idx, m_constants)) {
        throw std::runtime_error("VocabularyInfoImpl::get_constant - constant index out of range.");
    }
    return m_constants[constant_idx];
}

const std::vector<Constant>& VocabularyInfoImpl::get_constants() const {
    return m_constants;
}

std::unordered_map<std::string, EXPRESSION_TYPE> VocabularyInfoImpl::m_element_name_to_expression_type = {
    {"c_all", C_ALL },
    {"c_and", C_AND },
    {"c_bot", C_BOT },
    {"c_diff", C_DIFF },
    {"c_equal", C_EQUAL },
    {"c_not", C_NOT },
    {"c_one_of", C_ONE_OF },
    {"c_or", C_OR },
    {"c_primitive", C_PRIMITIVE },
    {"c_projection", C_PROJECTION },
    {"c_some", C_SOME },
    {"c_subset", C_SUBSET},
    {"c_top", C_TOP },
    {"r_and", R_AND },
    {"r_compose", R_COMPOSE },
    {"r_diff", R_DIFF },
    {"r_identity", R_IDENTITY },
    {"r_inverse", R_INVERSE },
    {"r_not", R_NOT },
    {"r_or", R_OR },
    {"r_primitive", R_PRIMITIVE },
    {"r_restrict", R_RESTRICT },
    {"r_top", R_TOP },
    {"r_transitive_closure", R_TRANSITIVE_CLOSURE },
    {"r_transitive_reflexive_closure", R_TRANSITIVE_REFLEXIVE_CLOSURE },
    {"n_count", N_COUNT },
    {"n_concept_distance", N_CONCEPT_DISTANCE },
    {"n_sum_concept_distance", N_SUM_CONCEPT_DISTANCE },
    {"n_role_distance", N_ROLE_DISTANCE },
    {"n_sum_role_distance", N_SUM_ROLE_DISTANCE },
    {"b_empty", B_EMPTY },
    {"b_inclusion", B_INCLUSION },
    {"b_nullary", B_NULLARY }
};

bool VocabularyInfoImpl::exists_element_name(const std::string& name) {
    return VocabularyInfoImpl::m_element_name_to_expression_type.find(name) != VocabularyInfoImpl::m_element_name_to_expression_type.end();
}

EXPRESSION_TYPE VocabularyInfoImpl::element_name_to_expression_type(const std::string &name) {
    auto p = VocabularyInfoImpl::m_element_name_to_expression_type.find(name);
    if (p == VocabularyInfoImpl::m_element_name_to_expression_type.end()) {
        throw std::runtime_error("VocabularyInfoImpl::name_to_expression_type - undefined expression name ("s + name + ").");
    }
    return p->second;
}


}
