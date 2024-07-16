#include "../../include/dlplan/core.h"

#include "element_factory.h"
#include "../../include/dlplan/utils/hash.h"


namespace std {
    std::size_t hash<dlplan::core::Constant>::operator()(const dlplan::core::Constant& constant) const {
        return constant.hash();
    }
    std::size_t hash<dlplan::core::Predicate>::operator()(const dlplan::core::Predicate& predicate) const {
        return predicate.hash();
    }
    std::size_t hash<dlplan::core::Object>::operator()(const dlplan::core::Object& object) const {
        return object.hash();
    }
    std::size_t hash<dlplan::core::Atom>::operator()(const dlplan::core::Atom& atom) const {
        return atom.hash();
    }
    size_t hash<dlplan::core::State>::operator()(const dlplan::core::State& state) const {
        return state.hash();
    }
    size_t hash<dlplan::core::ConceptDenotation>::operator()(const dlplan::core::ConceptDenotation& denotation) const {
        return denotation.hash();
    }
    size_t hash<dlplan::core::RoleDenotation>::operator()(const dlplan::core::RoleDenotation& denotation) const {
        return denotation.hash();
    }
    size_t hash<dlplan::core::ConceptDenotations>::operator()(const dlplan::core::ConceptDenotations& denotations) const {
        return dlplan::hash_vector(denotations);
    }
    size_t hash<dlplan::core::RoleDenotations>::operator()(const dlplan::core::RoleDenotations& denotations) const {
        return dlplan::hash_vector(denotations);
    }
    size_t hash<dlplan::core::DenotationsCacheKey>::operator()(const dlplan::core::DenotationsCacheKey& key) const {
        return key.hash();
    }
}

namespace dlplan::core {
SyntacticElementFactory::SyntacticElementFactory(std::shared_ptr<VocabularyInfo> vocabulary_info)
    : m_pImpl(SyntacticElementFactoryImpl(vocabulary_info)) { }

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

std::shared_ptr<VocabularyInfo> SyntacticElementFactory::get_vocabulary_info() const {
    return m_pImpl->get_vocabulary_info();
}

std::shared_ptr<const Concept> SyntacticElementFactory::parse_concept(
    const std::string &description, const std::string& filename) {
    return m_pImpl->parse_concept(*this, description, filename);
}

std::shared_ptr<const Concept> SyntacticElementFactory::parse_concept(
    iterator_type& iter, iterator_type end, const std::string& filename) {
    return m_pImpl->parse_concept(*this, iter, end, filename);
}

std::shared_ptr<const Role> SyntacticElementFactory::parse_role(
    const std::string &description, const std::string& filename) {
    return m_pImpl->parse_role(*this, description, filename);
}

std::shared_ptr<const Role> SyntacticElementFactory::parse_role(
    iterator_type& iter, iterator_type end, const std::string& filename) {
    return m_pImpl->parse_role(*this, iter, end, filename);
}

std::shared_ptr<const Boolean> SyntacticElementFactory::parse_boolean(
    const std::string &description, const std::string& filename) {
    return m_pImpl->parse_boolean(*this, description, filename);
}

std::shared_ptr<const Boolean> SyntacticElementFactory::parse_boolean(
    iterator_type& iter, iterator_type end, const std::string& filename) {
    return m_pImpl->parse_boolean(*this, iter, end, filename);
}

std::shared_ptr<const Numerical> SyntacticElementFactory::parse_numerical(
    const std::string &description, const std::string& filename) {
    return m_pImpl->parse_numerical(*this, description, filename);
}

std::shared_ptr<const Numerical> SyntacticElementFactory::parse_numerical(
    iterator_type& iter, iterator_type end, const std::string& filename) {
    return m_pImpl->parse_numerical(*this, iter, end, filename);
}


std::shared_ptr<const Boolean>SyntacticElementFactory::make_empty_boolean(const std::shared_ptr<const Concept>& concept_) {
    return m_pImpl->make_empty_boolean(concept_);
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

std::shared_ptr<const Concept>SyntacticElementFactory::make_all_concept(const std::shared_ptr<const Role>& role, const std::shared_ptr<const Concept>& concept_) {
    return m_pImpl->make_all_concept(role, concept_);
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

std::shared_ptr<const Concept>SyntacticElementFactory::make_not_concept(const std::shared_ptr<const Concept>& concept_) {
    return m_pImpl->make_not_concept(concept_);
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

std::shared_ptr<const Concept>SyntacticElementFactory::make_some_concept(const std::shared_ptr<const Role>& role, const std::shared_ptr<const Concept>& concept_) {
    return m_pImpl->make_some_concept(role, concept_);
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

std::shared_ptr<const Numerical>SyntacticElementFactory::make_count_numerical(const std::shared_ptr<const Concept>& concept_) {
    return m_pImpl->make_count_numerical(concept_);
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

std::shared_ptr<const Role>SyntacticElementFactory::make_identity_role(const std::shared_ptr<const Concept>& concept_) {
    return m_pImpl->make_identity_role(concept_);
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

std::shared_ptr<const Role>SyntacticElementFactory::make_restrict_role(const std::shared_ptr<const Role>& role, const std::shared_ptr<const Concept>& concept_) {
    return m_pImpl->make_restrict_role(role, concept_);
}

std::shared_ptr<const Role>SyntacticElementFactory::make_til_c_role(const std::shared_ptr<const Role>& role, const std::shared_ptr<const Concept>& concept_) {
    return m_pImpl->make_til_c_role(role, concept_);
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

// Explicit template instantiations
template class Element<ConceptDenotation, ConceptDenotations>;
template class Element<RoleDenotation, RoleDenotations>;
template class ElementLight<bool, BooleanDenotations>;
template class ElementLight<int, NumericalDenotations>;
}

namespace dlplan {
template class SharedObjectCache<core::DenotationsCacheKey,
    core::ConceptDenotation,
    core::RoleDenotation,
    bool,
    int,
    core::ConceptDenotations,
    core::RoleDenotations,
    core::BooleanDenotations,
    core::NumericalDenotations>;
}