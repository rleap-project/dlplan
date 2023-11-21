#include "primitive.h"

#include "../utils.h"
#include "../../../utils/collections.h"
#include "../../../../include/dlplan/core.h"

#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/serialization/base_object.hpp>
#include <boost/serialization/shared_ptr.hpp>
#include <boost/serialization/weak_ptr.hpp>

using namespace std::string_literals;


namespace dlplan::core {
void PrimitiveRole::compute_result(const State& state, RoleDenotation& result) const {
    const auto& instance_info = *state.get_instance_info();
    const auto& atoms = instance_info.get_atoms();
    for (int atom_idx : state.get_atom_indices()) {
        const auto& atom = atoms[atom_idx];
        if (atom.get_predicate_index() == m_predicate.get_index()) {
            assert(dlplan::utils::in_bounds(m_pos_1, atom.get_object_indices()));
            assert(dlplan::utils::in_bounds(m_pos_2, atom.get_object_indices()));
            result.insert(std::make_pair(atom.get_object_indices()[m_pos_1], atom.get_object_indices()[m_pos_2]));
        }
    }
    for (const auto& atom : state.get_instance_info()->get_static_atoms()) {
        if (atom.get_predicate_index() == m_predicate.get_index()) {
            assert(dlplan::utils::in_bounds(m_pos_1, atom.get_object_indices()));
            assert(dlplan::utils::in_bounds(m_pos_2, atom.get_object_indices()));
            result.insert(std::make_pair(atom.get_object_indices()[m_pos_1], atom.get_object_indices()[m_pos_2]));
        }
    }
}

RoleDenotation PrimitiveRole::evaluate_impl(const State& state, DenotationsCaches&) const {
    RoleDenotation denotation(state.get_instance_info()->get_objects().size());
    compute_result(
        state,
        denotation);
    return denotation;
}

RoleDenotations PrimitiveRole::evaluate_impl(const States& states, DenotationsCaches& caches) const {
    RoleDenotations denotations;
    denotations.reserve(states.size());
    for (size_t i = 0; i < states.size(); ++i) {
        RoleDenotation denotation(states[i].get_instance_info()->get_objects().size());
        compute_result(
            states[i],
            denotation);
        denotations.push_back(caches.role_denotation_cache.insert_denotation(std::move(denotation)));
    }
    return denotations;
}


PrimitiveRole::PrimitiveRole(ElementIndex index, std::shared_ptr<VocabularyInfo> vocabulary_info, const Predicate& predicate, int pos_1, int pos_2)
: Role(vocabulary_info, index, predicate.is_static()), m_predicate(predicate), m_pos_1(pos_1), m_pos_2(pos_2) {
    if (m_pos_1 >= m_predicate.get_arity() || m_pos_2 >= m_predicate.get_arity()) {
        throw std::runtime_error("PrimitiveRole::evaluate_impl - object index does not match predicate arity ("s + std::to_string(m_pos_1) + " or " + std::to_string(m_pos_2)  + " > " + std::to_string(predicate.get_arity()) + ").");
    }
}

bool PrimitiveRole::operator==(const Role& other) const {
    if (typeid(*this) == typeid(other)) {
        const auto& other_derived = static_cast<const PrimitiveRole&>(other);
        return m_is_static == other_derived.m_is_static
            && m_predicate == other_derived.m_predicate
            && m_pos_1 == other_derived.m_pos_1
            && m_pos_2 == other_derived.m_pos_2;
    }
    return false;
}

size_t PrimitiveRole::hash() const {
    return dlplan::utils::hash_combine(m_is_static, m_predicate, m_pos_1, m_pos_2);
}

RoleDenotation PrimitiveRole::evaluate(const State& state) const {
    RoleDenotation denotation(state.get_instance_info()->get_objects().size());
    compute_result(state, denotation);
    return denotation;
}

int PrimitiveRole::compute_complexity() const {
    return 1;
}

void PrimitiveRole::compute_repr(std::stringstream& out) const {
    out << "r_primitive" << "(" << m_predicate.get_name() << "," << std::to_string(m_pos_1) << "," << std::to_string(m_pos_2) << ")";
}

int PrimitiveRole::compute_evaluate_time_score() const {
    return SCORE_LINEAR;
}

const Predicate& PrimitiveRole::get_predicate() const {
    return m_predicate;
}

}


namespace boost::serialization {
template<typename Archive>
void serialize(Archive& /* ar */ , dlplan::core::PrimitiveRole& t, const unsigned int /* version */ )
{
    boost::serialization::base_object<dlplan::core::Role>(t);
}

template<class Archive>
void save_construct_data(Archive & ar, const dlplan::core::PrimitiveRole* t, const unsigned int /* version */ )
{
    ar << t->m_vocabulary_info;
    ar << t->m_index;
    ar << &t->m_predicate;
    ar << t->m_pos_1;
    ar << t->m_pos_2;
}

template<class Archive>
void load_construct_data(Archive & ar, dlplan::core::PrimitiveRole* t, const unsigned int /* version */ )
{
    std::shared_ptr<dlplan::core::VocabularyInfo> vocabulary;
    int index;
    dlplan::core::Predicate* predicate;
    int pos_1;
    int pos_2;
    ar >> vocabulary;
    ar >> index;
    ar >> predicate;
    ar >> pos_1;
    ar >> pos_2;
    ::new(t)dlplan::core::PrimitiveRole(index, vocabulary, *predicate, pos_1, pos_2);
    delete predicate;
}

template<typename Archive>
void serialize(Archive& /*ar*/, std::pair<const dlplan::core::PrimitiveRole, std::weak_ptr<dlplan::core::PrimitiveRole>>& /*t*/, const unsigned int /*version*/) {
}

template<class Archive>
void save_construct_data(Archive& ar, const std::pair<const dlplan::core::PrimitiveRole, std::weak_ptr<dlplan::core::PrimitiveRole>>* t, const unsigned int /*version*/) {
    ar << t->first;
    ar << t->second;
}

template<class Archive>
void load_construct_data(Archive& ar, std::pair<const dlplan::core::PrimitiveRole, std::weak_ptr<dlplan::core::PrimitiveRole>>* t, const unsigned int /*version*/) {
    dlplan::core::PrimitiveRole* first = nullptr;
    std::weak_ptr<dlplan::core::PrimitiveRole>* second = nullptr;
    ar >> const_cast<dlplan::core::PrimitiveRole&>(*first);
    ar >> second;
    ::new(t)std::pair<const dlplan::core::PrimitiveRole, std::weak_ptr<dlplan::core::PrimitiveRole>>(*first, *second);
    delete first;
    delete second;
}

template void serialize(boost::archive::text_iarchive& ar,
    dlplan::core::PrimitiveRole& t, const unsigned int version);
template void serialize(boost::archive::text_oarchive& ar,
    dlplan::core::PrimitiveRole& t, const unsigned int version);
template void save_construct_data(boost::archive::text_oarchive& ar,
    const dlplan::core::PrimitiveRole* t, const unsigned int version);
template void load_construct_data(boost::archive::text_iarchive& ar,
    dlplan::core::PrimitiveRole* t, const unsigned int version);

template void serialize(boost::archive::text_iarchive& ar,
    std::pair<const dlplan::core::PrimitiveRole, std::weak_ptr<dlplan::core::PrimitiveRole>>& t, const unsigned int version);
template void serialize(boost::archive::text_oarchive& ar,
    std::pair<const dlplan::core::PrimitiveRole, std::weak_ptr<dlplan::core::PrimitiveRole>>& t, const unsigned int version);
template void save_construct_data(boost::archive::text_oarchive& ar,
    const std::pair<const dlplan::core::PrimitiveRole, std::weak_ptr<dlplan::core::PrimitiveRole>>* t, const unsigned int version);
template void load_construct_data(boost::archive::text_iarchive& ar,
    std::pair<const dlplan::core::PrimitiveRole, std::weak_ptr<dlplan::core::PrimitiveRole>>* t, const unsigned int version);
}

BOOST_CLASS_EXPORT_IMPLEMENT(dlplan::core::PrimitiveRole)


namespace std {
    bool less<std::shared_ptr<const dlplan::core::PrimitiveRole>>::operator()(
        const std::shared_ptr<const dlplan::core::PrimitiveRole>& left_role,
        const std::shared_ptr<const dlplan::core::PrimitiveRole>& right_role) const {
        return *left_role < *right_role;
    }


    std::size_t hash<dlplan::core::PrimitiveRole>::operator()(const dlplan::core::PrimitiveRole& role) const {
        return role.hash();
    }
}

