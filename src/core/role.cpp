#include "include/dlplan/core.h"

#include <boost/serialization/base_object.hpp>


namespace dlplan::core {
Role::Role(std::shared_ptr<VocabularyInfo> vocabulary_info, ElementIndex index, bool is_static)
    : BaseElement(vocabulary_info, index, is_static) {
}

Role::Role(const Role& other) = default;

Role& Role::operator=(const Role& other) = default;

Role::Role(Role&& other) = default;

Role& Role::operator=(Role&& other) = default;

Role::~Role() = default;

const RoleDenotation* Role::evaluate(const State& state, DenotationsCaches& caches) const {
    auto cached = caches.role_denotation_cache.get_denotation(
        get_index(),
        state.get_instance_info()->get_index(),
        is_static() ? -1 : state.get_index());
    if (cached) return cached;
    auto denotation = caches.role_denotation_cache.insert_denotation(evaluate_impl(state, caches));
    caches.role_denotation_cache.insert_denotation(
        get_index(),
        state.get_instance_info()->get_index(),
        is_static() ? -1 : state.get_index(),
        denotation);
    return denotation;
}

const RoleDenotations* Role::evaluate(const States& states, DenotationsCaches& caches) const {
    auto cached = caches.role_denotations_cache.get_denotation(get_index(), -1, -1);
    if (cached) return cached;
    auto result_denotations = caches.role_denotations_cache.insert_denotation(evaluate_impl(states, caches));
    caches.role_denotations_cache.insert_denotation(get_index(), -1, -1, result_denotations);
    return result_denotations;
}

}


namespace boost::serialization {
template<typename Archive>
void serialize(Archive& /* ar */ , dlplan::core::Role& t, const unsigned int /* version */ )
{
    boost::serialization::base_object<dlplan::core::BaseElement>(t);
}

template<class Archive>
void save_construct_data(Archive& /* ar */ , const dlplan::core::Role* /* t */ , const unsigned int /* version */ )
{
}

template<class Archive>
void load_construct_data(Archive& /* ar */ , dlplan::core::Role* /* t */ , const unsigned int /* version */ )
{
}

}

