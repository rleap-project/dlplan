#include "include/dlplan/core.h"


#include <boost/serialization/base_object.hpp>


namespace dlplan::core {
Numerical::Numerical(std::shared_ptr<VocabularyInfo> vocabulary_info, ElementIndex index, bool is_static)
    : BaseElement(vocabulary_info, index, is_static) {
}

Numerical::Numerical(const Numerical& other) = default;

Numerical& Numerical::operator=(const Numerical& other) = default;

Numerical::Numerical(Numerical&& other) = default;

Numerical& Numerical::operator=(Numerical&& other) = default;

Numerical::~Numerical() = default;

int Numerical::evaluate(const State& state, DenotationsCaches& caches) const {
    const int* cached = caches.numerical_denotation_cache.get_denotation(
        get_index(),
        state.get_instance_info()->get_index(),
        is_static() ? -1 : state.get_index());
    if (cached) return *cached;
    const int* denotation = caches.numerical_denotation_cache.insert_denotation(evaluate_impl(state, caches));
    caches.numerical_denotation_cache.insert_denotation(
        get_index(),
        state.get_instance_info()->get_index(),
        is_static() ? -1 : state.get_index(),
        denotation);
    return *denotation;
}

const NumericalDenotations* Numerical::evaluate(const States& states, DenotationsCaches& caches) const {
    auto cached = caches.numerical_denotations_cache.get_denotation(get_index(), -1, -1);
    if (cached) return cached;
    auto result_denotations = caches.numerical_denotations_cache.insert_denotation(evaluate_impl(states, caches));
    caches.numerical_denotations_cache.insert_denotation(get_index(), -1, -1, result_denotations);
    return result_denotations;
}

}


namespace boost::serialization {
template<typename Archive>
void serialize(Archive& /* ar */ , dlplan::core::Numerical& t, const unsigned int /* version */ )
{
    boost::serialization::base_object<dlplan::core::BaseElement>(t);
}

template<class Archive>
void save_construct_data(Archive& /* ar */ , const dlplan::core::Numerical* /* t */ , const unsigned int /* version */ )
{
}

template<class Archive>
void load_construct_data(Archive& /* ar */ , dlplan::core::Numerical* /* t */ , const unsigned int /* version */ )
{
}
}
