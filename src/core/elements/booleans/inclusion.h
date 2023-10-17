#ifndef DLPLAN_SRC_CORE_ELEMENTS_BOOLEAN_INCLUSION_H_
#define DLPLAN_SRC_CORE_ELEMENTS_BOOLEAN_INCLUSION_H_

#include <sstream>
#include <memory>

#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/serialization/export.hpp>
#include <boost/serialization/base_object.hpp>
#include <boost/serialization/shared_ptr.hpp>

#include "src/core/elements/utils.h"
#include "include/dlplan/core.h"

using namespace std::string_literals;


namespace dlplan::core {
template<typename T>
class InclusionBoolean;
}


namespace boost::serialization {
    template<typename Archive, typename T>
    void serialize(Archive& ar, dlplan::core::InclusionBoolean<T>& boolean, const unsigned int version);
    template<class Archive, typename T>
    void save_construct_data(Archive& ar, const dlplan::core::InclusionBoolean<T>* boolean, const unsigned int version);
    template<class Archive, typename T>
    void load_construct_data(Archive& ar, dlplan::core::InclusionBoolean<T>* boolean, const unsigned int version);
}


namespace dlplan::core {
template<typename T>
class InclusionBoolean : public Boolean {
private:
    template<typename DENOTATION_TYPE>
    void compute_result(const DENOTATION_TYPE& denot_left, const DENOTATION_TYPE& denot_right, bool& result) const {
        result = denot_left.is_subset_of(denot_right);
    }

    bool
    evaluate_impl(const State& state, DenotationsCaches& caches) const override {
        bool denotation;
        compute_result(
                *m_element_left->evaluate(state, caches),
                *m_element_right->evaluate(state, caches),
                denotation);
        return denotation;
    }

    BooleanDenotations
    evaluate_impl(const States& states, DenotationsCaches& caches) const override {
        BooleanDenotations denotations;
        auto element_left_denotations = m_element_left->evaluate(states, caches);
        auto element_right_denotations = m_element_left->evaluate(states, caches);
        for (size_t i = 0; i < states.size(); ++i) {
            bool denotation;
            compute_result(
                *(*element_left_denotations)[i],
                *(*element_right_denotations)[i],
                denotation);
            denotations.push_back(denotation);
        }
        return denotations;
    }

    template<typename Archive, typename T_>
    friend void boost::serialization::serialize(Archive& ar, InclusionBoolean<T_>& boolean, const unsigned int version);
    template<class Archive, typename T_>
    friend void boost::serialization::save_construct_data(Archive& ar, const InclusionBoolean<T_>* boolean, const unsigned int version);
    template<class Archive, typename T_>
    friend void boost::serialization::load_construct_data(Archive& ar, InclusionBoolean<T_>* boolean, const unsigned int version);

protected:
    const std::shared_ptr<const T> m_element_left;
    const std::shared_ptr<const T> m_element_right;

public:
    InclusionBoolean(std::shared_ptr<VocabularyInfo> vocabulary_info, ElementIndex index, std::shared_ptr<const T> element_left, std::shared_ptr<const T> element_right)
    : Boolean(vocabulary_info, index, element_left->is_static() && element_right->is_static()),
      m_element_left(element_left),
      m_element_right(element_right) {
    }

    bool evaluate(const State& state) const override {
        bool denotation;
        compute_result(
            m_element_left->evaluate(state),
            m_element_right->evaluate(state),
            denotation);
        return denotation;
    }

    int compute_complexity() const override {
        return m_element_left->compute_complexity() + m_element_right->compute_complexity() + 1;
    }

    void compute_repr(std::stringstream& out) const override {
       out << "b_inclusion" << "(";
       m_element_left->compute_repr(out);
       out << ",";
       m_element_right->compute_repr(out) ;
       out << ")";
    }

    int compute_evaluate_time_score() const override {
        int score = m_element_left->compute_evaluate_time_score() + m_element_right->compute_evaluate_time_score();
        if (std::is_same<T, Concept>::value) {
            score += SCORE_LINEAR;
        } else if (std::is_same<T, Role>::value) {
            score += SCORE_QUADRATIC;
        } else {
            throw std::runtime_error("Inclusion::compute_evaluate_time_score - unknown template parameter.");
        }
        return score;
    }
};

}


namespace boost::serialization {
template<typename Archive, typename T>
void serialize(Archive& /* ar */ , dlplan::core::InclusionBoolean<T>& t, const unsigned int /* version */ )
{
    boost::serialization::base_object<dlplan::core::Boolean>(t);
}

template<class Archive, typename T>
void save_construct_data(Archive& ar, const dlplan::core::InclusionBoolean<T>* t, const unsigned int /* version */ )
{
    ar << t->m_vocabulary_info;
    ar << t->m_index;
    ar << t->m_element_left;
    ar << t->m_element_right;
}

template<class Archive, typename T>
void load_construct_data(Archive& ar, dlplan::core::InclusionBoolean<T>* t, const unsigned int /* version */ )
{
    std::shared_ptr<dlplan::core::VocabularyInfo> vocabulary;
    int index;
    std::shared_ptr<const T> element_left;
    std::shared_ptr<const T> element_right;
    ar >> vocabulary;
    ar >> index;
    ar >> element_left;
    ar >> element_right;
    ::new(t)dlplan::core::InclusionBoolean<T>(vocabulary, index, element_left, element_right);
}

}

BOOST_CLASS_EXPORT_GUID(dlplan::core::InclusionBoolean<dlplan::core::Concept>, "dlplan::core::InclusionBoolean<dlplan::core::Concept>")
BOOST_CLASS_EXPORT_GUID(dlplan::core::InclusionBoolean<dlplan::core::Role>, "dlplan::core::InclusionBoolean<dlplan::core::Role>")

#endif