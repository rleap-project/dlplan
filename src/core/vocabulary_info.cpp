#include "vocabulary_info.h"

#include "predicate.h"
#include "instance_info.h"
#include "element_factory.h"

#include "../utils/collections.h"

namespace dlp {
namespace core {

static bool exists(const std::string& name, std::unordered_map<std::string, unsigned>& mapping) {
    auto f = mapping.find(name);
    return (f != mapping.end());
}

const Predicate& VocabularyInfoImpl::add_predicate(const VocabularyInfo& parent, const std::string &predicate_name, unsigned arity) {
    if (m_predicate_name_to_predicate_idx.find(predicate_name) != m_predicate_name_to_predicate_idx.end()) {
        throw std::runtime_error("VocabularyInfoImpl::add_predicate - predicate with name ("s + predicate_name + ") already exists.");
    }
    unsigned predicate_idx = m_predicates.size();
    m_predicates.push_back(Predicate(parent, predicate_name, predicate_idx, arity));
    m_predicate_name_to_predicate_idx.emplace(predicate_name, predicate_idx);
    return m_predicates.back();
}

bool VocabularyInfoImpl::exists_predicate_name(const std::string& name) const {
    return m_predicate_name_to_predicate_idx.find(name) != m_predicate_name_to_predicate_idx.end();
}

const std::vector<Predicate>& VocabularyInfoImpl::get_predicates() const {
    return m_predicates;
}

unsigned VocabularyInfoImpl::get_predicate_idx(const std::string& name) const {
    if (!exists_predicate_name(name)) {
        throw std::runtime_error("VocabularyInfoImpl::get_predicate_idx - no predicate with name ("s + name + ").");
    }
    return m_predicate_name_to_predicate_idx.at(name);
}

const Predicate& VocabularyInfoImpl::get_predicate(unsigned predicate_idx) const {
    if (!utils::in_bounds(predicate_idx, m_predicates)) {
        throw std::runtime_error("VocabularyInfoImpl::get_predicate - predicate index out of range.");
    }
    return m_predicates[predicate_idx];
}

std::unordered_map<std::string, EXPRESSION_TYPE> VocabularyInfoImpl::m_element_name_to_expression_type = {
    {"c_all", C_ALL },
    {"c_and", C_AND },
    {"c_bot", C_BOT },
    {"c_diff", C_DIFF },
    {"c_not", C_NOT },
    {"c_one_of", C_ONE_OF },
    {"c_or", C_OR },
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
    {"r_restrict", R_RESTRICT },
    {"r_top", R_TOP },
    {"r_transitive_closure", R_TRANSITIVE_CLOSURE },
    {"r_transitive_reflexive_closure", R_TRANSITIVE_REFLEXIVE_CLOSURE },
    {"n_count", N_COUNT },
    {"n_concept_distance", N_CONCEPT_DISTANCE },
    {"n_sum_concept_distance", N_SUM_CONCEPT_DISTANCE },
    {"n_role_distance", N_ROLE_DISTANCE },
    {"n_sum_role_distance", N_SUM_ROLE_DISTANCE },
    {"b_empty", B_EMPTY }
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
}
