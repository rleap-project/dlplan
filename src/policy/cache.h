#ifndef DLPLAN_SRC_POLICY_CACHE_H_
#define DLPLAN_SRC_POLICY_CACHE_H_

#include <memory>

#include <boost/serialization/shared_ptr.hpp>

#include "src/utils/cache.h"
#include "include/dlplan/policy.h"


namespace dlplan::policy {
/**
 * One cache for each template instantiated element.
 */
struct Caches {
    std::shared_ptr<dlplan::utils::ReferenceCountedObjectCache<std::string, NamedBoolean>> m_boolean_cache;
    std::shared_ptr<dlplan::utils::ReferenceCountedObjectCache<std::string, NamedNumerical>> m_numerical_cache;
    std::shared_ptr<dlplan::utils::ReferenceCountedObjectCache<std::string, NamedConcept>> m_concept_cache;
    std::shared_ptr<dlplan::utils::ReferenceCountedObjectCache<std::string, NamedRole>> m_role_cache;
    std::shared_ptr<dlplan::utils::ReferenceCountedObjectCache<std::string, BaseCondition>> m_condition_cache;
    std::shared_ptr<dlplan::utils::ReferenceCountedObjectCache<std::string, BaseEffect>> m_effect_cache;
    std::shared_ptr<dlplan::utils::ReferenceCountedObjectCache<std::string, Rule>> m_rule_cache;
    std::shared_ptr<dlplan::utils::ReferenceCountedObjectCache<std::string, Policy>> m_policy_cache;

    Caches()
        : m_boolean_cache(std::make_shared<dlplan::utils::ReferenceCountedObjectCache<std::string, NamedBoolean>>()),
          m_numerical_cache(std::make_shared<dlplan::utils::ReferenceCountedObjectCache<std::string, NamedNumerical>>()),
          m_concept_cache(std::make_shared<dlplan::utils::ReferenceCountedObjectCache<std::string, NamedConcept>>()),
          m_role_cache(std::make_shared<dlplan::utils::ReferenceCountedObjectCache<std::string, NamedRole>>()),
          m_condition_cache(std::make_shared<dlplan::utils::ReferenceCountedObjectCache<std::string, BaseCondition>>()),
          m_effect_cache(std::make_shared<dlplan::utils::ReferenceCountedObjectCache<std::string, BaseEffect>>()),
          m_rule_cache(std::make_shared<dlplan::utils::ReferenceCountedObjectCache<std::string, Rule>>()),
          m_policy_cache(std::make_shared<dlplan::utils::ReferenceCountedObjectCache<std::string, Policy>>()) { }
};

}


namespace boost::serialization {
template<typename Archive>
void serialize(Archive& ar, dlplan::policy::Caches& t, const unsigned int /* version */ )
{
    ar & t.m_boolean_cache;
    ar & t.m_numerical_cache;
    ar & t.m_concept_cache;
    ar & t.m_role_cache;
    ar & t.m_condition_cache;
    ar & t.m_effect_cache;
    ar & t.m_rule_cache;
    ar & t.m_policy_cache;
}

}

#endif