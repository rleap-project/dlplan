#include "include/dlplan/core.h"

#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/serialization/unordered_map.hpp>
#include <boost/serialization/unordered_set.hpp>
#include <boost/serialization/unique_ptr.hpp>

#include "include/dlplan/utils/hash.h"


namespace dlplan::core {

DenotationsCaches::DenotationsCaches() = default;

DenotationsCaches::~DenotationsCaches() = default;

DenotationsCaches::DenotationsCaches(DenotationsCaches&& other) = default;

DenotationsCaches& DenotationsCaches::operator=(DenotationsCaches&& other) = default;

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

namespace boost::serialization {

template <typename Archive>
void serialize(Archive& ar, dlplan::core::DenotationsCaches& t, const unsigned int /* version */ ) {
    ar & t.concept_denotation_cache;
    ar & t.role_denotation_cache;
    ar & t.boolean_denotation_cache;
    ar & t.numerical_denotation_cache;
    ar & t.concept_denotations_cache;
    ar & t.role_denotations_cache;
    ar & t.boolean_denotations_cache;
    ar & t.numerical_denotations_cache;
}

template <typename Archive>
void serialize(Archive& ar, dlplan::core::DenotationsCaches::Key& t, const unsigned int /* version */ ) {
    ar & t.element;
    ar & t.instance;
    ar & t.state;
}

template <typename Archive, typename T>
void serialize(Archive& ar, dlplan::core::DenotationsCaches::Cache<T>& t, const unsigned int /* version */ ) {
    ar & t.uniqueness;
    ar & t.per_element_instance_state_mapping;
}

template <typename Archive>
void serialize(Archive& ar, bool& t, const unsigned int /* version */ ) {
    ar & t;
}

template <typename Archive>
void serialize(Archive& ar, int& t, const unsigned int /* version */ ) {
    ar & t;
}

template void serialize(boost::archive::text_iarchive& ar,
    dlplan::core::DenotationsCaches& t, const unsigned int version);
template void serialize(boost::archive::text_oarchive& ar,
    dlplan::core::DenotationsCaches& t, const unsigned int version);

template void serialize(boost::archive::text_iarchive& ar,
    dlplan::core::DenotationsCaches::Key& t, const unsigned int version);
template void serialize(boost::archive::text_oarchive& ar,
    dlplan::core::DenotationsCaches::Key& t, const unsigned int version);

template void serialize(boost::archive::text_iarchive& ar,
    dlplan::core::DenotationsCaches::Cache<dlplan::core::ConceptDenotation>& t, const unsigned int version);
template void serialize(boost::archive::text_oarchive& ar,
    dlplan::core::DenotationsCaches::Cache<dlplan::core::ConceptDenotation>& t, const unsigned int version);

template void serialize(boost::archive::text_iarchive& ar,
    dlplan::core::DenotationsCaches::Cache<dlplan::core::RoleDenotation>& t, const unsigned int version);
template void serialize(boost::archive::text_oarchive& ar,
    dlplan::core::DenotationsCaches::Cache<dlplan::core::RoleDenotation>& t, const unsigned int version);

template void serialize(boost::archive::text_iarchive& ar,
    dlplan::core::DenotationsCaches::Cache<bool>& t, const unsigned int version);
template void serialize(boost::archive::text_oarchive& ar,
    dlplan::core::DenotationsCaches::Cache<bool>& t, const unsigned int version);

template void serialize(boost::archive::text_iarchive& ar,
    dlplan::core::DenotationsCaches::Cache<int>& t, const unsigned int version);
template void serialize(boost::archive::text_oarchive& ar,
    dlplan::core::DenotationsCaches::Cache<int>& t, const unsigned int version);

template void serialize(boost::archive::text_iarchive& ar,
    bool& t, const unsigned int version);
template void serialize(boost::archive::text_oarchive& ar,
    bool& t, const unsigned int version);

template void serialize(boost::archive::text_iarchive& ar,
    int& t, const unsigned int version);
template void serialize(boost::archive::text_oarchive& ar,
    int& t, const unsigned int version);

}
