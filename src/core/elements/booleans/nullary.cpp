#include "../../../../include/dlplan/core/elements/booleans/nullary.h"

#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/serialization/base_object.hpp>
#include <boost/serialization/shared_ptr.hpp>
#include <boost/serialization/weak_ptr.hpp>


namespace dlplan::core {

void NullaryBoolean::compute_result(const State& state, bool& result) const {
    const auto& atoms = state.get_instance_info()->get_atoms();
    for (int atom_idx : state.get_atom_indices()) {
        const auto& atom = atoms[atom_idx];
        if (atom.get_predicate_index() == m_predicate.get_index()) {
            result = true;
            return;
        }
    }
    for (const auto &atom : state.get_instance_info()->get_static_atoms()) {
        if (atom.get_predicate_index() == m_predicate.get_index()) {
            result = true;
            return;
        }
    }
    result = false;
}

bool NullaryBoolean::evaluate_impl(const State& state, DenotationsCaches&) const {
    return evaluate(state);
}

BooleanDenotations
NullaryBoolean::evaluate_impl(const States& states, DenotationsCaches&) const {
    BooleanDenotations denotations;
    for (size_t i = 0; i < states.size(); ++i) {
        denotations.push_back(evaluate(states[i]));
    }
    return denotations;
}

NullaryBoolean::NullaryBoolean(ElementIndex index, std::shared_ptr<VocabularyInfo> vocabulary_info, const Predicate& predicate)
: Boolean(vocabulary_info, index, predicate.is_static()), m_predicate(predicate) {
    if (predicate.get_arity() != 0) {
        throw std::runtime_error("NullaryBoolean::NullaryBoolean - expected predicate with arity 0.");
    }
}

bool NullaryBoolean::operator==(const Boolean& other) const {
    if (typeid(*this) == typeid(other)) {
        const auto& other_derived = static_cast<const NullaryBoolean&>(other);
        return m_is_static == other_derived.m_is_static
            && m_predicate == other_derived.m_predicate;
    }
    return false;
}

size_t NullaryBoolean::hash() const {
    return dlplan::utils::hash_combine(m_is_static, m_predicate);
}

bool NullaryBoolean::evaluate(const State& state) const {
    bool denotation;
    compute_result(state, denotation);
    return denotation;
}

int NullaryBoolean::compute_complexity() const {
    return 1;
}

void NullaryBoolean::compute_repr(std::stringstream& out) const {
    out << "b_nullary" << "(" << m_predicate.get_name() << ")";
}

int NullaryBoolean::compute_evaluate_time_score() const {
    return SCORE_LINEAR;
}

}


namespace boost::serialization {
template<typename Archive>
void serialize(Archive& /* ar */ , dlplan::core::NullaryBoolean& t, const unsigned int /* version */ )
{
    boost::serialization::base_object<dlplan::core::Boolean>(t);
}

template<class Archive>
void save_construct_data(Archive& ar, const dlplan::core::NullaryBoolean* t, const unsigned int /* version */ )
{
    ar << t->m_vocabulary_info;
    ar << t->m_index;
    ar << &t->m_predicate;
}

template<class Archive>
void load_construct_data(Archive& ar, dlplan::core::NullaryBoolean* t, const unsigned int /* version */ )
{
    std::shared_ptr<dlplan::core::VocabularyInfo> vocabulary;
    int index;
    dlplan::core::Predicate* predicate;
    ar >> vocabulary;
    ar >> index;
    ar >> predicate;
    ::new(t)dlplan::core::NullaryBoolean(index, vocabulary, *predicate);
    delete predicate;
}


template<typename Archive>
void serialize(Archive& /*ar*/, std::pair<const dlplan::core::NullaryBoolean, std::weak_ptr<dlplan::core::NullaryBoolean>>& /*t*/, const unsigned int /*version*/) {
}

template<class Archive>
void save_construct_data(Archive& ar, const std::pair<const dlplan::core::NullaryBoolean, std::weak_ptr<dlplan::core::NullaryBoolean>>* t, const unsigned int /*version*/) {
    ar << t->first;
    ar << t->second;
}

template<class Archive>
void load_construct_data(Archive& ar, std::pair<const dlplan::core::NullaryBoolean, std::weak_ptr<dlplan::core::NullaryBoolean>>* t, const unsigned int /*version*/) {
    dlplan::core::NullaryBoolean* first = nullptr;
    std::weak_ptr<dlplan::core::NullaryBoolean>* second = nullptr;
    ar >> const_cast<dlplan::core::NullaryBoolean&>(*first);
    ar >> second;
    ::new(t)std::pair<const dlplan::core::NullaryBoolean, std::weak_ptr<dlplan::core::NullaryBoolean>>(*first, *second);
    delete first;
    delete second;
}

template void serialize(boost::archive::text_iarchive& ar,
    dlplan::core::NullaryBoolean& t, const unsigned int version);
template void serialize(boost::archive::text_oarchive& ar,
    dlplan::core::NullaryBoolean& t, const unsigned int version);
template void save_construct_data(boost::archive::text_oarchive& ar,
    const dlplan::core::NullaryBoolean* t, const unsigned int version);
template void load_construct_data(boost::archive::text_iarchive& ar,
    dlplan::core::NullaryBoolean* t, const unsigned int version);

template void serialize(boost::archive::text_iarchive& ar,
    std::pair<const dlplan::core::NullaryBoolean, std::weak_ptr<dlplan::core::NullaryBoolean>>& t, const unsigned int version);
template void serialize(boost::archive::text_oarchive& ar,
    std::pair<const dlplan::core::NullaryBoolean, std::weak_ptr<dlplan::core::NullaryBoolean>>& t, const unsigned int version);
template void save_construct_data(boost::archive::text_oarchive& ar,
    const std::pair<const dlplan::core::NullaryBoolean, std::weak_ptr<dlplan::core::NullaryBoolean>>* t, const unsigned int version);
template void load_construct_data(boost::archive::text_iarchive& ar,
    std::pair<const dlplan::core::NullaryBoolean, std::weak_ptr<dlplan::core::NullaryBoolean>>* t, const unsigned int version);
}

BOOST_CLASS_EXPORT_IMPLEMENT(dlplan::core::NullaryBoolean)


namespace std {
    bool less<std::shared_ptr<const dlplan::core::NullaryBoolean>>::operator()(
        const std::shared_ptr<const dlplan::core::NullaryBoolean>& left_boolean,
        const std::shared_ptr<const dlplan::core::NullaryBoolean>& right_boolean) const {
        return *left_boolean < *right_boolean;
    }

    std::size_t hash<dlplan::core::NullaryBoolean>::operator()(const dlplan::core::NullaryBoolean& boolean) const {
        return boolean.hash();
    }
}

