#include "../../include/dlplan/core.h"
#include "../../include/dlplan/utils/hash.h"


namespace dlplan::core {

DenotationsCaches::DenotationsCaches() = default;

DenotationsCaches::~DenotationsCaches() = default;

DenotationsCaches::DenotationsCaches(DenotationsCaches&& other) = default;

DenotationsCaches& DenotationsCaches::operator=(DenotationsCaches&& other) = default;

DenotationsCaches::Cache<ConceptDenotation>& DenotationsCaches::get_concept_denotation_cache() {
    return m_concept_denotation_cache;
}

DenotationsCaches::Cache<RoleDenotation>& DenotationsCaches::get_role_denotation_cache() {
    return m_role_denotation_cache;
}

DenotationsCaches::Cache<bool>& DenotationsCaches::get_boolean_denotation_cache() {
    return m_boolean_denotation_cache;
}

DenotationsCaches::Cache<int>& DenotationsCaches::get_numerical_denotation_cache() {
    return m_numerical_denotation_cache;
}

DenotationsCaches::Cache<ConceptDenotations>& DenotationsCaches::get_concept_denotations_cache() {
    return m_concept_denotations_cache;
}

DenotationsCaches::Cache<RoleDenotations>& DenotationsCaches::get_role_denotations_cache() {
    return m_role_denotations_cache;
}

DenotationsCaches::Cache<BooleanDenotations>& DenotationsCaches::get_boolean_denotations_cache() {
    return m_boolean_denotations_cache;
}

DenotationsCaches::Cache<NumericalDenotations>& DenotationsCaches::get_numerical_denotations_cache() {
    return m_numerical_denotations_cache;
}


bool DenotationsCaches::Key::operator==(const Key& other) const {
    return (element == other.element) &&
           (instance == other.instance) &&
           (state == other.state);
}

bool DenotationsCaches::Key::operator!=(const Key& other) const {
    return !(*this == other);
}


std::size_t DenotationsCaches::KeyHash::operator()(const Key& key) const {
    std::size_t seed = key.element;
    dlplan::utils::hash_combine(seed, key.instance);
    dlplan::utils::hash_combine(seed, key.state);
    return seed;
}

}
