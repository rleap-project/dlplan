#ifndef DLPLAN_SRC_CORE_ELEMENTS_CONCEPTS_NOT_H_
#define DLPLAN_SRC_CORE_ELEMENTS_CONCEPTS_NOT_H_

#include <sstream>
#include <memory>

#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/serialization/export.hpp>
#include <boost/serialization/shared_ptr.hpp>

#include "src/core/elements/utils.h"
#include "include/dlplan/core.h"

using namespace std::string_literals;


namespace dlplan::core {
class NotConcept;
}


namespace boost::serialization {
    template<typename Archive>
    void serialize(Archive& ar, dlplan::core::NotConcept& concept, const unsigned int version);
    template<class Archive>
    void save_construct_data(Archive& ar, const dlplan::core::NotConcept* concept, const unsigned int version);
    template<class Archive>
    void load_construct_data(Archive& ar, dlplan::core::NotConcept* concept, const unsigned int version);
}


namespace dlplan::core {
class NotConcept : public Concept {
private:
    void compute_result(const ConceptDenotation& denot, ConceptDenotation& result) const {
        result = denot;
        ~result;
    }

    ConceptDenotation evaluate_impl(const State& state, DenotationsCaches& caches) const override {
        ConceptDenotation denotation(state.get_instance_info()->get_objects().size());
        compute_result(
            *m_concept->evaluate(state, caches),
            denotation);
        return denotation;
    }

    ConceptDenotations evaluate_impl(const States& states, DenotationsCaches& caches) const override {
        ConceptDenotations denotations;
        denotations.reserve(states.size());
        // get denotations of children
        auto concept_denotations = m_concept->evaluate(states, caches);
        for (size_t i = 0; i < states.size(); ++i) {
            ConceptDenotation denotation(states[i].get_instance_info()->get_objects().size());
            compute_result(
                *(*concept_denotations)[i],
                denotation);
            denotations.push_back(caches.concept_denotation_cache.insert_denotation(std::move(denotation)));
        }
        return denotations;
    }

    template<typename Archive>
    friend void boost::serialization::serialize(Archive& ar, NotConcept& concept, const unsigned int version);
    template<class Archive>
    friend void boost::serialization::save_construct_data(Archive& ar, const NotConcept* concept, const unsigned int version);
    template<class Archive>
    friend void boost::serialization::load_construct_data(Archive& ar, NotConcept* concept, const unsigned int version);

protected:
    const std::shared_ptr<const Concept> m_concept;

public:
    NotConcept(std::shared_ptr<VocabularyInfo> vocabulary_info, ElementIndex index, std::shared_ptr<const Concept> concept)
    : Concept(vocabulary_info, index, concept->is_static()), m_concept(concept){
        if (!concept) {
            throw std::runtime_error("NotConcept::NotConcept - child is a nullptr");
        }
    }

    ConceptDenotation evaluate(const State& state) const override {
        ConceptDenotation denotation(state.get_instance_info()->get_objects().size());
        compute_result(
            m_concept->evaluate(state),
            denotation);
        return denotation;
    }

    int compute_complexity() const override {
        return m_concept->compute_complexity() + 1;
    }

    void compute_repr(std::stringstream& out) const override {
        out << "c_not" << "(";
        m_concept->compute_repr(out);
        out << ")";
    }

    int compute_evaluate_time_score() const override {
        return m_concept->compute_evaluate_time_score() + SCORE_LINEAR;
    }
};

}


namespace boost::serialization {
template<typename Archive>
void serialize(Archive& /* ar */ , dlplan::core::NotConcept& t, const unsigned int /* version */ )
{
    boost::serialization::base_object<dlplan::core::Concept>(t);
}

template<class Archive>
void save_construct_data(Archive& ar, const dlplan::core::NotConcept* t, const unsigned int /* version */ )
{
    ar << t->m_vocabulary_info;
    ar << t->m_index;
    ar << t->m_concept;
}

template<class Archive>
void load_construct_data(Archive& ar, dlplan::core::NotConcept* t, const unsigned int /* version */ )
{
    std::shared_ptr<dlplan::core::VocabularyInfo> vocabulary;
    int index;
    std::shared_ptr<const dlplan::core::Concept> concept;
    ar >> vocabulary;
    ar >> index;
    ar >> concept;
    ::new(t)dlplan::core::NotConcept(vocabulary, index, concept);
}

}

BOOST_CLASS_EXPORT_GUID(dlplan::core::NotConcept, "dlplan::core::NotConcept")

#endif
