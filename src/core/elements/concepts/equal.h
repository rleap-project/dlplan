#ifndef DLPLAN_SRC_CORE_ELEMENTS_CONCEPTS_EQUAL_H_
#define DLPLAN_SRC_CORE_ELEMENTS_CONCEPTS_EQUAL_H_

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
class EqualConcept;
}


namespace boost::serialization {
    template<typename Archive>
    void serialize(Archive& ar, dlplan::core::EqualConcept& concept, const unsigned int version);
    template<class Archive>
    void save_construct_data(Archive& ar, const dlplan::core::EqualConcept* concept, const unsigned int version);
    template<class Archive>
    void load_construct_data(Archive& ar, dlplan::core::EqualConcept* concept, const unsigned int version);
}


namespace dlplan::core {
class EqualConcept : public Concept {
private:
    void compute_result(const RoleDenotation& left_denot, const RoleDenotation& right_denot, ConceptDenotation& result) const {
        // find counterexample [(a,b) in R and (a,b) not in S] or [(a,b) not in R and (a,b) in S]
        result.set();
        for (const auto& pair : left_denot.to_vector()) {
            if (!right_denot.contains(pair)) result.erase(pair.first);
        }
        for (const auto& pair : right_denot.to_vector()) {
            if (!left_denot.contains(pair)) result.erase(pair.first);
        }
    }

    ConceptDenotation evaluate_impl(const State& state, DenotationsCaches& caches) const override {
        ConceptDenotation denotation(state.get_instance_info()->get_objects().size());
        compute_result(
            *m_role_left->evaluate(state, caches),
            *m_role_right->evaluate(state, caches),
            denotation);
        return denotation;
    }

    ConceptDenotations evaluate_impl(const States& states, DenotationsCaches& caches) const override {
        ConceptDenotations denotations;
        denotations.reserve(states.size());
        auto role_left_denotations = m_role_left->evaluate(states, caches);
        auto role_right_denotations = m_role_right->evaluate(states, caches);
        for (size_t i = 0; i < states.size(); ++i) {
            ConceptDenotation denotation(states[i].get_instance_info()->get_objects().size());
            compute_result(
                *(*role_left_denotations)[i],
                *(*role_right_denotations)[i],
                denotation);
            // register denotation and append it to denotations.
            denotations.push_back(caches.concept_denotation_cache.insert_denotation(std::move(denotation)));
        }
        return denotations;
    }

    template<typename Archive>
    friend void boost::serialization::serialize(Archive& ar, EqualConcept& concept, const unsigned int version);
    template<class Archive>
    friend void boost::serialization::save_construct_data(Archive& ar, const EqualConcept* concept, const unsigned int version);
    template<class Archive>
    friend void boost::serialization::load_construct_data(Archive& ar, EqualConcept* concept, const unsigned int version);

protected:
    const std::shared_ptr<const Role> m_role_left;
    const std::shared_ptr<const Role> m_role_right;

public:
    EqualConcept(std::shared_ptr<VocabularyInfo> vocabulary_info, ElementIndex index, std::shared_ptr<const Role> role_left, std::shared_ptr<const Role> role_right)
    : Concept(vocabulary_info, index, role_left->is_static() && role_right->is_static()),
      m_role_left(role_left), m_role_right(role_right) {
        if (!(role_left && role_right)) {
            throw std::runtime_error("EqualConcept::EqualConcept - at least one child is a nullptr");
        }
    }

    ConceptDenotation evaluate(const State& state) const override {
        auto denotation = ConceptDenotation(state.get_instance_info()->get_objects().size());
        compute_result(
            m_role_left->evaluate(state),
            m_role_right->evaluate(state),
            denotation);
        return denotation;
    }

    int compute_complexity() const override {
        return m_role_left->compute_complexity() + m_role_right->compute_complexity() + 1;
    }

    void compute_repr(std::stringstream& out) const override {
        out << "c_equal" << "(";
        m_role_left->compute_repr(out);
        out << ",";
        m_role_right->compute_repr(out);
        out << ")";
    }

    int compute_evaluate_time_score() const override {
        return m_role_left->compute_evaluate_time_score() + m_role_right->compute_evaluate_time_score() + SCORE_QUADRATIC;
    }
};

}


namespace boost::serialization {
template<typename Archive>
void serialize(Archive& /* ar */ , dlplan::core::EqualConcept& t, const unsigned int /* version */ )
{
    boost::serialization::base_object<dlplan::core::Concept>(t);
}

template<class Archive>
void save_construct_data(Archive& ar, const dlplan::core::EqualConcept* t, const unsigned int /* version */ )
{
    ar << t->m_vocabulary_info;
    ar << t->m_index;
    ar << t->m_role_left;
    ar << t->m_role_right;
}

template<class Archive>
void load_construct_data(Archive& ar, dlplan::core::EqualConcept* t, const unsigned int /* version */ )
{
    std::shared_ptr<dlplan::core::VocabularyInfo> vocabulary;
    int index;
    std::shared_ptr<const dlplan::core::Role> role_left;
    std::shared_ptr<const dlplan::core::Role> role_right;
    ar >> vocabulary;
    ar >> index;
    ar >> role_left;
    ar >> role_right;
    ::new(t)dlplan::core::EqualConcept(vocabulary, index, role_left, role_right);
}

}

BOOST_CLASS_EXPORT_GUID(dlplan::core::EqualConcept, "dlplan::core::EqualConcept")

#endif
