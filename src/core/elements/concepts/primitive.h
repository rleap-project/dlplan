#ifndef DLPLAN_SRC_CORE_ELEMENTS_CONCEPTS_PRIMITIVE_H_
#define DLPLAN_SRC_CORE_ELEMENTS_CONCEPTS_PRIMITIVE_H_

#include "../utils.h"
#include "../../../utils/collections.h"
#include "../../../../include/dlplan/core.h"

#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/serialization/export.hpp>
#include <boost/serialization/base_object.hpp>
#include <boost/serialization/shared_ptr.hpp>

#include <sstream>
#include <memory>


using namespace std::string_literals;


namespace dlplan::utils {
template<typename... Ts>
class ReferenceCountedObjectFactory;
}


namespace dlplan::core {
class PrimitiveConcept;
}


namespace boost::serialization {
    template<typename Archive>
    void serialize(Archive& ar, dlplan::core::PrimitiveConcept& t, const unsigned int version);
    template<class Archive>
    void save_construct_data(Archive& ar, const dlplan::core::PrimitiveConcept* t, const unsigned int version);
    template<class Archive>
    void load_construct_data(Archive& ar, dlplan::core::PrimitiveConcept* t, const unsigned int version);

    template<typename Archive>
    void serialize(Archive& ar, std::pair<const dlplan::core::PrimitiveConcept, std::weak_ptr<dlplan::core::PrimitiveConcept>>& t, const unsigned int version);
    template<class Archive>
    void save_construct_data(Archive& ar, const std::pair<const dlplan::core::PrimitiveConcept, std::weak_ptr<dlplan::core::PrimitiveConcept>>* t, const unsigned int version);
    template<class Archive>
    void load_construct_data(Archive& ar, std::pair<const dlplan::core::PrimitiveConcept, std::weak_ptr<dlplan::core::PrimitiveConcept>>* t, const unsigned int version);
}


namespace dlplan::core {
class PrimitiveConcept : public Concept {
private:
    const Predicate m_predicate;
    const int m_pos;

    void compute_result(const State& state, ConceptDenotation& result) const {
        const auto& instance_info = *state.get_instance_info();
        const auto& atoms = instance_info.get_atoms();
        for (int atom_idx : state.get_atom_indices()) {
            const auto& atom = atoms[atom_idx];
            if (atom.get_predicate_index() == m_predicate.get_index()) {
                assert(dlplan::utils::in_bounds(m_pos, atom.get_object_indices()));
                result.insert(atom.get_object_indices()[m_pos]);
            }
        }
        for (const auto &atom : state.get_instance_info()->get_static_atoms()) {
            if (atom.get_predicate_index() == m_predicate.get_index()) {
                assert(dlplan::utils::in_bounds(m_pos, atom.get_object_indices()));
                result.insert(atom.get_object_indices()[m_pos]);
            }
        }
    }

    ConceptDenotation evaluate_impl(const State& state, DenotationsCaches&) const override {
        ConceptDenotation denotation(state.get_instance_info()->get_objects().size());
        compute_result(
            state,
            denotation);
        return denotation;
    }

    ConceptDenotations evaluate_impl(const States& states, DenotationsCaches& caches) const override {
        ConceptDenotations denotations;
        denotations.reserve(states.size());
        for (size_t i = 0; i < states.size(); ++i) {
            ConceptDenotation denotation(states[i].get_instance_info()->get_objects().size());
            compute_result(
                states[i],
                denotation);
            denotations.push_back(caches.concept_denotation_cache.insert_denotation(std::move(denotation)));
        }
        return denotations;
    }

    PrimitiveConcept(ElementIndex index, std::shared_ptr<VocabularyInfo> vocabulary_info, const Predicate& predicate, int pos)
        : Concept(vocabulary_info, index, predicate.is_static()), m_predicate(predicate), m_pos(pos) {
        if (m_pos >= m_predicate.get_arity()) {
            throw std::runtime_error("PrimitiveConcept::PrimitiveConcept - object index does not match predicate arity ("s + std::to_string(m_pos) + " > " + std::to_string(predicate.get_arity()) + ").");
        }
    }

    /// @brief Constructor for serialization.
    PrimitiveConcept() : m_pos(-1) {}

    template<typename Archive>
    friend void boost::serialization::serialize(Archive& ar, PrimitiveConcept& t, const unsigned int version);
    template<class Archive>
    friend void boost::serialization::save_construct_data(Archive& ar, const PrimitiveConcept* t, const unsigned int version);
    template<class Archive>
    friend void boost::serialization::load_construct_data(Archive& ar, PrimitiveConcept* t, const unsigned int version);
    template<typename... Ts>
    friend class dlplan::utils::ReferenceCountedObjectFactory;

public:
    bool operator==(const Concept& other) const override {
        if (typeid(*this) == typeid(other)) {
            const auto& other_derived = static_cast<const PrimitiveConcept&>(other);
            return m_is_static == other_derived.m_is_static
                && m_predicate == other_derived.m_predicate
                && m_pos == other_derived.m_pos;
        }
        return false;
    }

    size_t hash() const {
        return dlplan::utils::hash_combine(m_is_static, m_predicate, m_pos);
    }

    ConceptDenotation evaluate(const State& state) const override {
        ConceptDenotation denotation(state.get_instance_info()->get_objects().size());
        compute_result(state, denotation);
        return denotation;
    }

    int compute_complexity() const override {
        return 1;
    }

    void compute_repr(std::stringstream& out) const override {
        out << "c_primitive" << "(" << m_predicate.get_name() << "," << std::to_string(m_pos) << ")";
    }

    int compute_evaluate_time_score() const override {
        return SCORE_LINEAR;
    }
};

}



namespace boost::serialization {
template<typename Archive>
void serialize(Archive& /* ar */ , dlplan::core::PrimitiveConcept& t, const unsigned int /* version */ )
{
    boost::serialization::base_object<dlplan::core::Concept>(t);
}

template<class Archive>
void save_construct_data(Archive& ar, const dlplan::core::PrimitiveConcept* t, const unsigned int /* version */ )
{
    ar << t->m_vocabulary_info;
    ar << t->m_index;
    ar << &t->m_predicate;
    ar << t->m_pos;
}

template<class Archive>
void load_construct_data(Archive& ar, dlplan::core::PrimitiveConcept* t, const unsigned int /* version */ )
{
    std::shared_ptr<dlplan::core::VocabularyInfo> vocabulary;
    int index;
    dlplan::core::Predicate* predicate;
    int pos;
    ar >> vocabulary;
    ar >> index;
    ar >> predicate;
    ar >> pos;
    ::new(t)dlplan::core::PrimitiveConcept(index, vocabulary, *predicate, pos);
    delete predicate;
}


template<typename Archive>
void serialize(Archive& /*ar*/, std::pair<const dlplan::core::PrimitiveConcept, std::weak_ptr<dlplan::core::PrimitiveConcept>>& /*t*/, const unsigned int /*version*/) {
}

template<class Archive>
void save_construct_data(Archive& ar, const std::pair<const dlplan::core::PrimitiveConcept, std::weak_ptr<dlplan::core::PrimitiveConcept>>* t, const unsigned int /*version*/) {
    ar << t->first;
    ar << t->second;
}

template<class Archive>
void load_construct_data(Archive& ar, std::pair<const dlplan::core::PrimitiveConcept, std::weak_ptr<dlplan::core::PrimitiveConcept>>* t, const unsigned int /*version*/) {
    dlplan::core::PrimitiveConcept* first;
    std::weak_ptr<dlplan::core::PrimitiveConcept>* second;
    ar >> const_cast<dlplan::core::PrimitiveConcept&>(t->first);
    ar >> t->second;
    ::new(t)std::pair<const dlplan::core::PrimitiveConcept, std::weak_ptr<dlplan::core::PrimitiveConcept>>(*first, *second);
    delete first;
    delete second;
}

}

BOOST_CLASS_EXPORT_GUID(dlplan::core::PrimitiveConcept, "dlplan::core::PrimitiveConcept")


namespace std {
    template<>
    struct less<std::shared_ptr<const dlplan::core::PrimitiveConcept>>
    {
        bool operator()(
            const std::shared_ptr<const dlplan::core::PrimitiveConcept>& left_concept,
            const std::shared_ptr<const dlplan::core::PrimitiveConcept>& right_concept) const {
            return *left_concept < *right_concept;
        }
    };

    template<>
    struct hash<dlplan::core::PrimitiveConcept>
    {
        std::size_t operator()(const dlplan::core::PrimitiveConcept& concept_) const {
            return concept_.hash();
        }
    };
}

#endif
