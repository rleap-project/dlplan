#include "../../include/dlplan/core.h"
#include "../../include/dlplan/utils/hash.h"

#include "element_factory.h"


namespace dlplan::core {
size_t hash_impl<State>::operator()(const State& state) const {
    return state.hash();
}
size_t hash_impl<ConceptDenotation>::operator()(const ConceptDenotation& denotation) const {
    return denotation.hash();
}
size_t hash_impl<RoleDenotation>::operator()(const RoleDenotation& denotation) const {
    return denotation.hash();
}
size_t hash_impl<ConceptDenotations>::operator()(const ConceptDenotations& denotations) const {
    size_t seed = 0;
    for (const auto denot_ptr : denotations) {
        dlplan::utils::hash_combine(seed, denot_ptr);
    }
    return seed;
}
size_t hash_impl<RoleDenotations>::operator()(const RoleDenotations& denotations) const {
    size_t seed = 0;
    for (const auto denot_ptr : denotations) {
        dlplan::utils::hash_combine(seed, denot_ptr);
    }
    return seed;
}
size_t hash_impl<bool>::operator()(const bool& value) const {
    return std::hash<bool>()(value);
}
size_t hash_impl<int>::operator()(const int& value) const {
    return std::hash<int>()(value);
}
size_t hash_impl<std::vector<bool>>::operator()(const std::vector<bool>& data) const {
    return std::hash<std::vector<bool>>()(data);
}
size_t hash_impl<std::vector<unsigned>>::operator()(const std::vector<unsigned>& data) const {
    return dlplan::utils::hash<std::vector<unsigned>>()(data);
}
size_t hash_impl<std::vector<int>>::operator()(const std::vector<int>& data) const {
    return dlplan::utils::hash<std::vector<int>>()(data);
}


SyntacticElementFactory::SyntacticElementFactory(std::shared_ptr<const VocabularyInfo> vocabulary_info) : m_pImpl(SyntacticElementFactoryImpl(vocabulary_info)) { }

SyntacticElementFactory::SyntacticElementFactory(const SyntacticElementFactory& other) : m_pImpl(*other.m_pImpl) { }

SyntacticElementFactory& SyntacticElementFactory::operator=(const SyntacticElementFactory& other) {
    if (this != &other) {
        *m_pImpl = *other.m_pImpl;
    }
    return *this;
}

SyntacticElementFactory::SyntacticElementFactory(SyntacticElementFactory&& other)
    : m_pImpl(std::move(*other.m_pImpl)) { }

SyntacticElementFactory& SyntacticElementFactory::operator=(SyntacticElementFactory&& other) {
    if (this != &other) {
        std::swap(*m_pImpl, *other.m_pImpl);
    }
    return *this;
}

SyntacticElementFactory::~SyntacticElementFactory() = default;

std::shared_ptr<const VocabularyInfo> SyntacticElementFactory::get_vocabulary_info() const {
    return m_pImpl->get_vocabulary_info();
}

std::shared_ptr<const Concept> SyntacticElementFactory::parse_concept(const std::string &description) {
    return m_pImpl->parse_concept(description);
}

std::shared_ptr<const Role>SyntacticElementFactory::parse_role(const std::string &description) {
    return m_pImpl->parse_role(description);
}

std::shared_ptr<const Numerical>SyntacticElementFactory::parse_numerical(const std::string &description) {
    return m_pImpl->parse_numerical(description);
}

std::shared_ptr<const Boolean>SyntacticElementFactory::parse_boolean(const std::string &description) {
    return m_pImpl->parse_boolean(description);
}


std::shared_ptr<const Boolean>SyntacticElementFactory::make_empty_boolean(const std::shared_ptr<const Concept>& concept) {
    return m_pImpl->make_empty_boolean(concept);
}

std::shared_ptr<const Boolean>SyntacticElementFactory::make_empty_boolean(const std::shared_ptr<const Role>& role) {
    return m_pImpl->make_empty_boolean(role);
}

std::shared_ptr<const Boolean>SyntacticElementFactory::make_inclusion_boolean(const std::shared_ptr<const Concept>& concept_left, const std::shared_ptr<const Concept>& concept_right) {
    return m_pImpl->make_inclusion_boolean(concept_left, concept_right);
}

std::shared_ptr<const Boolean>SyntacticElementFactory::make_inclusion_boolean(const std::shared_ptr<const Role>& role_left, const std::shared_ptr<const Role>& role_right) {
    return m_pImpl->make_inclusion_boolean(role_left, role_right);
}

std::shared_ptr<const Boolean>SyntacticElementFactory::make_nullary_boolean(const Predicate& predicate) {
    return m_pImpl->make_nullary_boolean(predicate);
}

std::shared_ptr<const Concept>SyntacticElementFactory::make_all_concept(const std::shared_ptr<const Role>& role, const std::shared_ptr<const Concept>& concept) {
    return m_pImpl->make_all_concept(role, concept);
}

std::shared_ptr<const Concept>SyntacticElementFactory::make_and_concept(const std::shared_ptr<const Concept>& concept_left, const std::shared_ptr<const Concept>& concept_right) {
    return m_pImpl->make_and_concept(concept_left, concept_right);
}

std::shared_ptr<const Concept>SyntacticElementFactory::make_bot_concept() {
    return m_pImpl->make_bot_concept();
}

std::shared_ptr<const Concept>SyntacticElementFactory::make_diff_concept(const std::shared_ptr<const Concept>& concept_left, const std::shared_ptr<const Concept>& concept_right) {
    return m_pImpl->make_diff_concept(concept_left, concept_right);
}

std::shared_ptr<const Concept>SyntacticElementFactory::make_equal_concept(const std::shared_ptr<const Role>& role_left, const std::shared_ptr<const Role>& role_right) {
    return m_pImpl->make_equal_concept(role_left, role_right);
}

std::shared_ptr<const Concept>SyntacticElementFactory::make_not_concept(const std::shared_ptr<const Concept>& concept) {
    return m_pImpl->make_not_concept(concept);
}

std::shared_ptr<const Concept>SyntacticElementFactory::make_one_of_concept(const Constant& constant) {
    return m_pImpl->make_one_of_concept(constant);
}

std::shared_ptr<const Concept>SyntacticElementFactory::make_or_concept(const std::shared_ptr<const Concept>& concept_left, const std::shared_ptr<const Concept>& concept_right) {
    return m_pImpl->make_or_concept(concept_left, concept_right);
}

std::shared_ptr<const Concept>SyntacticElementFactory::make_projection_concept(const std::shared_ptr<const Role>& role, int pos) {
    return m_pImpl->make_projection_concept(role, pos);
}

std::shared_ptr<const Concept>SyntacticElementFactory::make_primitive_concept(const Predicate& predicate, int pos) {
    return m_pImpl->make_primitive_concept(predicate, pos);
}

std::shared_ptr<const Concept>SyntacticElementFactory::make_some_concept(const std::shared_ptr<const Role>& role, const std::shared_ptr<const Concept>& concept) {
    return m_pImpl->make_some_concept(role, concept);
}

std::shared_ptr<const Concept>SyntacticElementFactory::make_subset_concept(const std::shared_ptr<const Role>& role_left, const std::shared_ptr<const Role>& role_right) {
    return m_pImpl->make_subset_concept(role_left, role_right);
}

std::shared_ptr<const Concept>SyntacticElementFactory::make_top_concept() {
    return m_pImpl->make_top_concept();
}

std::shared_ptr<const Numerical>SyntacticElementFactory::make_concept_distance_numerical(const std::shared_ptr<const Concept>& concept_from, const std::shared_ptr<const Role>& role, const std::shared_ptr<const Concept>& concept_to) {
    return m_pImpl->make_concept_distance_numerical(concept_from, role, concept_to);
}

std::shared_ptr<const Numerical>SyntacticElementFactory::make_count_numerical(const std::shared_ptr<const Concept>& concept) {
    return m_pImpl->make_count_numerical(concept);
}

std::shared_ptr<const Numerical>SyntacticElementFactory::make_count_numerical(const std::shared_ptr<const Role>& role) {
    return m_pImpl->make_count_numerical(role);
}

std::shared_ptr<const Numerical>SyntacticElementFactory::make_role_distance_numerical(const std::shared_ptr<const Role>& role_from, const std::shared_ptr<const Role>& role, const std::shared_ptr<const Role>& role_to) {
    return m_pImpl->make_role_distance_numerical(role_from, role, role_to);
}

std::shared_ptr<const Numerical>SyntacticElementFactory::make_sum_concept_distance_numerical(const std::shared_ptr<const Concept>& concept_from, const std::shared_ptr<const Role>& role, const std::shared_ptr<const Concept>& concept_to) {
    return m_pImpl->make_sum_concept_distance_numerical(concept_from, role, concept_to);
}

std::shared_ptr<const Numerical>SyntacticElementFactory::make_sum_role_distance_numerical(const std::shared_ptr<const Role>& role_from, const std::shared_ptr<const Role>& role, const std::shared_ptr<const Role>& role_to) {
    return m_pImpl->make_sum_role_distance_numerical(role_from, role, role_to);
}

std::shared_ptr<const Role>SyntacticElementFactory::make_and_role(const std::shared_ptr<const Role>& role_left, const std::shared_ptr<const Role>& role_right) {
    return m_pImpl->make_and_role(role_left, role_right);
}

std::shared_ptr<const Role>SyntacticElementFactory::make_compose_role(const std::shared_ptr<const Role>& role_left, const std::shared_ptr<const Role>& role_right) {
    return m_pImpl->make_compose_role(role_left, role_right);
}

std::shared_ptr<const Role>SyntacticElementFactory::make_diff_role(const std::shared_ptr<const Role>& role_left, const std::shared_ptr<const Role>& role_right) {
    return m_pImpl->make_diff_role(role_left, role_right);
}

std::shared_ptr<const Role>SyntacticElementFactory::make_identity_role(const std::shared_ptr<const Concept>& concept) {
    return m_pImpl->make_identity_role(concept);
}

std::shared_ptr<const Role>SyntacticElementFactory::make_inverse_role(const std::shared_ptr<const Role>& role) {
    return m_pImpl->make_inverse_role(role);
}

std::shared_ptr<const Role>SyntacticElementFactory::make_not_role(const std::shared_ptr<const Role>& role) {
    return m_pImpl->make_not_role(role);
}

std::shared_ptr<const Role>SyntacticElementFactory::make_or_role(const std::shared_ptr<const Role>& role_left, const std::shared_ptr<const Role>& role_right) {
    return m_pImpl->make_or_role(role_left, role_right);
}

std::shared_ptr<const Role>SyntacticElementFactory::make_primitive_role(const Predicate& predicate, int pos_1, int pos_2) {
    return m_pImpl->make_primitive_role(predicate, pos_1, pos_2);
}

std::shared_ptr<const Role>SyntacticElementFactory::make_restrict_role(const std::shared_ptr<const Role>& role, const std::shared_ptr<const Concept>& concept) {
    return m_pImpl->make_restrict_role(role, concept);
}

std::shared_ptr<const Role>SyntacticElementFactory::make_top_role() {
    return m_pImpl->make_top_role();
}

std::shared_ptr<const Role>SyntacticElementFactory::make_transitive_closure(const std::shared_ptr<const Role>& role) {
    return m_pImpl->make_transitive_closure(role);
}

std::shared_ptr<const Role>SyntacticElementFactory::make_transitive_reflexive_closure(const std::shared_ptr<const Role>& role) {
    return m_pImpl->make_transitive_reflexive_closure(role);
}

}
