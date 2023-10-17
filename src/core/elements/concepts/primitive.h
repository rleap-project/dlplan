#ifndef DLPLAN_SRC_CORE_ELEMENTS_CONCEPTS_PRIMITIVE_H_
#define DLPLAN_SRC_CORE_ELEMENTS_CONCEPTS_PRIMITIVE_H_

#include <sstream>

#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/serialization/export.hpp>
#include <boost/serialization/base_object.hpp>
#include <boost/serialization/shared_ptr.hpp>

#include "src/core/elements/utils.h"
#include "src/utils/collections.h"
#include "include/dlplan/core.h"

using namespace std::string_literals;


namespace dlplan::core {
class PrimitiveConcept;
}


namespace boost::serialization {
    template<typename Archive>
    void serialize(Archive& ar, dlplan::core::PrimitiveConcept& concept, const unsigned int version);
    template<class Archive>
    void save_construct_data(Archive& ar, const dlplan::core::PrimitiveConcept* concept, const unsigned int version);
    template<class Archive>
    void load_construct_data(Archive& ar, dlplan::core::PrimitiveConcept* concept, const unsigned int version);
}


namespace dlplan::core {
class PrimitiveConcept : public Concept {
private:
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

    template<typename Archive>
    friend void boost::serialization::serialize(Archive& ar, PrimitiveConcept& concept, const unsigned int version);
    template<class Archive>
    friend void boost::serialization::save_construct_data(Archive& ar, const PrimitiveConcept* concept, const unsigned int version);
    template<class Archive>
    friend void boost::serialization::load_construct_data(Archive& ar, PrimitiveConcept* concept, const unsigned int version);

protected:
    const Predicate m_predicate;
    const int m_pos;

public:
    PrimitiveConcept(std::shared_ptr<VocabularyInfo> vocabulary_info, ElementIndex index, const Predicate& predicate, int pos)
    : Concept(vocabulary_info, index, predicate.is_static()), m_predicate(predicate), m_pos(pos) {
        if (m_pos >= m_predicate.get_arity()) {
            throw std::runtime_error("PrimitiveConcept::PrimitiveConcept - object index does not match predicate arity ("s + std::to_string(m_pos) + " > " + std::to_string(predicate.get_arity()) + ").");
        }
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
    ::new(t)dlplan::core::PrimitiveConcept(vocabulary, index, *predicate, pos);
    delete predicate;
}

}

BOOST_CLASS_EXPORT_GUID(dlplan::core::PrimitiveConcept, "dlplan::core::PrimitiveConcept")

#endif
