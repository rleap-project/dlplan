#ifndef DLPLAN_SRC_CORE_CACHE_H_
#define DLPLAN_SRC_CORE_CACHE_H_

#include <memory>

#include <boost/serialization/shared_ptr.hpp>

#include "include/dlplan/core.h"
#include "src/utils/cache.h"


namespace dlplan::core {
/**
 * One cache for each template instantiated element.
 */
struct Caches {
    std::shared_ptr<dlplan::utils::ReferenceCountedObjectCache<std::string, Concept>> m_concept_cache;
    std::shared_ptr<dlplan::utils::ReferenceCountedObjectCache<std::string, Role>> m_role_cache;
    std::shared_ptr<dlplan::utils::ReferenceCountedObjectCache<std::string, Numerical>> m_numerical_cache;
    std::shared_ptr<dlplan::utils::ReferenceCountedObjectCache<std::string, Boolean>> m_boolean_cache;

    Caches()
        : m_concept_cache(std::make_shared<dlplan::utils::ReferenceCountedObjectCache<std::string, Concept>>()),
          m_role_cache(std::make_shared<dlplan::utils::ReferenceCountedObjectCache<std::string, Role>>()),
          m_numerical_cache(std::make_shared<dlplan::utils::ReferenceCountedObjectCache<std::string, Numerical>>()),
          m_boolean_cache(std::make_shared<dlplan::utils::ReferenceCountedObjectCache<std::string, Boolean>>()) { }
};

}


namespace boost::serialization {
template<typename Archive>
void serialize(Archive& ar, dlplan::core::Caches& t, const unsigned int /* version */ )
{
    ar & t.m_boolean_cache;
    ar & t.m_numerical_cache;
    ar & t.m_concept_cache;
    ar & t.m_role_cache;
}

}

#endif