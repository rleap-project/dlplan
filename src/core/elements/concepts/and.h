#ifndef DLPLAN_SRC_CORE_ELEMENTS_CONCEPTS_AND_H_
#define DLPLAN_SRC_CORE_ELEMENTS_CONCEPTS_AND_H_

#include "../utils.h"
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
class AndConcept;
}


namespace boost::serialization {
    class access;
    template<typename Archive>
    void serialize(Archive& ar, dlplan::core::AndConcept& t, const unsigned int version);
    template<class Archive>
    void save_construct_data(Archive& ar, const dlplan::core::AndConcept* t, const unsigned int version);
    template<class Archive>
    void load_construct_data(Archive& ar, dlplan::core::AndConcept* t, const unsigned int version);

    template<typename Archive>
    void serialize(Archive& ar, std::pair<const dlplan::core::AndConcept, std::weak_ptr<dlplan::core::AndConcept>>& t, const unsigned int version);
    template<class Archive>
    void save_construct_data(Archive& ar, const std::pair<const dlplan::core::AndConcept, std::weak_ptr<dlplan::core::AndConcept>>* t, const unsigned int version);
    template<class Archive>
    void load_construct_data(Archive& ar, std::pair<const dlplan::core::AndConcept, std::weak_ptr<dlplan::core::AndConcept>>* t, const unsigned int version);
}


namespace dlplan::core {
class AndConcept : public Concept {
private:
    const std::shared_ptr<const Concept> m_concept_left;
    const std::shared_ptr<const Concept> m_concept_right;

    void compute_result(const ConceptDenotation& left_denot, const ConceptDenotation& right_denot, ConceptDenotation& result) const {
        result = left_denot;
        result &= right_denot;
    }

    ConceptDenotation evaluate_impl(const State& state, DenotationsCaches& caches) const override {
        ConceptDenotation denotation(state.get_instance_info()->get_objects().size());
        denotation.set();
        compute_result(
            *m_concept_left->evaluate(state, caches),
            *m_concept_right->evaluate(state, caches),
            denotation);
        return denotation;
    }

    ConceptDenotations evaluate_impl(const States& states, DenotationsCaches& caches) const override {
        ConceptDenotations denotations;
        denotations.reserve(states.size());
        auto concept_left_denotations = m_concept_left->evaluate(states, caches);
        auto concept_right_denotations = m_concept_right->evaluate(states, caches);
        for (size_t i = 0; i < states.size(); ++i) {
            ConceptDenotation denotation(states[i].get_instance_info()->get_objects().size());
            compute_result(
                *(*concept_left_denotations)[i],
                *(*concept_right_denotations)[i],
                denotation);
            denotations.push_back(caches.concept_denotation_cache.insert_denotation(std::move(denotation)));
        }
        return denotations;
    }

    AndConcept(ElementIndex index, std::shared_ptr<VocabularyInfo> vocabulary_info, std::shared_ptr<const Concept> concept_1, std::shared_ptr<const Concept> concept_2)
        : Concept(vocabulary_info, index, concept_1->is_static() && concept_2->is_static()),
        m_concept_left(concept_1->get_index() < concept_2->get_index() ? concept_1 : concept_2),
        m_concept_right(concept_1->get_index() < concept_2->get_index() ? concept_2 : concept_1) { }

    template<typename Archive>
    friend void boost::serialization::serialize(Archive& ar, AndConcept& t, const unsigned int version);
    template<class Archive>
    friend void boost::serialization::save_construct_data(Archive& ar, const AndConcept* t, const unsigned int version);
    template<class Archive>
    friend void boost::serialization::load_construct_data(Archive& ar, AndConcept* t, const unsigned int version);
    template<typename... Ts>
    friend class dlplan::utils::ReferenceCountedObjectFactory;

public:
    bool operator==(const Concept& other) const override {
        if (typeid(*this) == typeid(other)) {
            const auto& other_derived = static_cast<const AndConcept&>(other);
            return m_is_static == other_derived.m_is_static
                && m_concept_left == other_derived.m_concept_left
                && m_concept_right == other_derived.m_concept_right;
        }
        return false;
    }

    size_t hash() const {
        return dlplan::utils::hash_combine(m_is_static, m_concept_left, m_concept_right);
    }

    ConceptDenotation evaluate(const State& state) const override {
        ConceptDenotation result(state.get_instance_info()->get_objects().size());
        compute_result(
            m_concept_left->evaluate(state),
            m_concept_right->evaluate(state),
            result);
        return result;
    }

    int compute_complexity() const override {
        return m_concept_left->compute_complexity() + m_concept_right->compute_complexity() + 1;
    }

    void compute_repr(std::stringstream& out) const override {
        out << "c_and" << "(";
        m_concept_left->compute_repr(out);
        out << ",";
        m_concept_right->compute_repr(out);
        out << ")";
    }

    int compute_evaluate_time_score() const override {
        return m_concept_left->compute_evaluate_time_score() + m_concept_right->compute_evaluate_time_score() + SCORE_LINEAR;
    }
};

}


namespace boost::serialization {
template<typename Archive>
void serialize(Archive& /* ar */ , dlplan::core::AndConcept& t, const unsigned int /* version */ )
{
    boost::serialization::base_object<dlplan::core::Concept>(t);
}

template<class Archive>
void save_construct_data(Archive& ar, const dlplan::core::AndConcept* t, const unsigned int /* version */ )
{
    ar << t->m_vocabulary_info;
    ar << t->m_index;
    ar << t->m_concept_left;
    ar << t->m_concept_right;
}

template<class Archive>
void load_construct_data(Archive& ar, dlplan::core::AndConcept* t, const unsigned int /* version */ )
{
    std::shared_ptr<dlplan::core::VocabularyInfo> vocabulary;
    int index;
    std::shared_ptr<const dlplan::core::Concept> concept_left;
    std::shared_ptr<const dlplan::core::Concept> concept_right;
    ar >> vocabulary;
    ar >> index;
    ar >> concept_left;
    ar >> concept_right;
    ::new(t)dlplan::core::AndConcept(index, vocabulary, concept_left, concept_right);
}


template<typename Archive>
void serialize(Archive& /*ar*/, std::pair<const dlplan::core::AndConcept, std::weak_ptr<dlplan::core::AndConcept>>& /*t*/, const unsigned int /*version*/) {
}

template<class Archive>
void save_construct_data(Archive& ar, const std::pair<const dlplan::core::AndConcept, std::weak_ptr<dlplan::core::AndConcept>>* t, const unsigned int /*version*/) {
    ar << t->first;
    ar << t->second;
}

template<class Archive>
void load_construct_data(Archive& ar, std::pair<const dlplan::core::AndConcept, std::weak_ptr<dlplan::core::AndConcept>>* t, const unsigned int /*version*/) {
    dlplan::core::AndConcept* first;
    std::weak_ptr<dlplan::core::AndConcept>* second;
    ar >> const_cast<dlplan::core::AndConcept&>(t->first);
    ar >> t->second;
    ::new(t)std::pair<const dlplan::core::AndConcept, std::weak_ptr<dlplan::core::AndConcept>>(*first, *second);
    delete first;
    delete second;
}

}

BOOST_CLASS_EXPORT_GUID(dlplan::core::AndConcept, "dlplan::core::AndConcept")


namespace std {
    template<>
    struct less<std::shared_ptr<const dlplan::core::AndConcept>>
    {
        bool operator()(
            const std::shared_ptr<const dlplan::core::AndConcept>& left_concept,
            const std::shared_ptr<const dlplan::core::AndConcept>& right_concept) const {
            return *left_concept < *right_concept;
        }
    };

    template<>
    struct hash<dlplan::core::AndConcept>
    {
        std::size_t operator()(const dlplan::core::AndConcept& concept_) const {
            return concept_.hash();
        }
    };
}

#endif
