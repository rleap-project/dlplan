#ifndef DLPLAN_SRC_CORE_ELEMENTS_BOOLEAN_EMPTY_H_
#define DLPLAN_SRC_CORE_ELEMENTS_BOOLEAN_EMPTY_H_

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
class EmptyBoolean;
}


namespace boost::serialization {
    template<typename Archive, typename T>
    void serialize(Archive& ar, dlplan::core::EmptyBoolean<T>& boolean, const unsigned int version);
    template<class Archive, typename T>
    void save_construct_data(Archive& ar, const dlplan::core::EmptyBoolean<T>* boolean, const unsigned int version);
    template<class Archive, typename T>
    void load_construct_data(Archive& ar, dlplan::core::EmptyBoolean<T>* boolean, const unsigned int version);
}


namespace dlplan::core {
template<typename T>
class EmptyBoolean : public Boolean {
private:
    template<typename DENOTATION_TYPE>
    void compute_result(const DENOTATION_TYPE& denotation, bool& result) const {
        result = denotation.empty();
    }

    bool
    evaluate_impl(const State& state, DenotationsCaches& caches) const override {
        bool denotation;
        compute_result(
                *m_element->evaluate(state, caches),
                denotation);
        return denotation;
    }

    BooleanDenotations
    evaluate_impl(const States& states, DenotationsCaches& caches) const override {
        BooleanDenotations denotations;
        auto element_denotations = m_element->evaluate(states, caches);
        for (size_t i = 0; i < states.size(); ++i) {
            bool denotation;
            compute_result(
                *(*element_denotations)[i],
                denotation);
            denotations.push_back(denotation);
        }
        return denotations;
    }

    template<typename Archive, typename T_>
    friend void boost::serialization::serialize(Archive& ar, EmptyBoolean<T_>& boolean, const unsigned int version);
    template<class Archive, typename T_>
    friend void boost::serialization::save_construct_data(Archive & ar, const EmptyBoolean<T_>* boolean, const unsigned int version);
    template<class Archive, typename T_>
    friend void boost::serialization::load_construct_data(Archive & ar, EmptyBoolean<T_>* boolean, const unsigned int version);

protected:
    const std::shared_ptr<const T> m_element;

public:
    EmptyBoolean(std::shared_ptr<VocabularyInfo> vocabulary_info, ElementIndex index, std::shared_ptr<const T> element)
        : Boolean(vocabulary_info, index, element->is_static()), m_element(element) {
    }

    bool evaluate(const State& state) const override {
        bool denotation;
        compute_result(m_element->evaluate(state), denotation);
        return denotation;
    }

    int compute_complexity() const override {
        return m_element->compute_complexity() + 1;
    }

    void compute_repr(std::stringstream& out) const override {
        out << "b_empty" << "(";
        m_element->compute_repr(out);
        out << ")";
    }

    int compute_evaluate_time_score() const override {
        int score = m_element->compute_evaluate_time_score();
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
void serialize(Archive& /* ar */ , dlplan::core::EmptyBoolean<T>& t, const unsigned int /* version */ )
{
    boost::serialization::base_object<dlplan::core::Boolean>(t);
}

template<class Archive, typename T>
void save_construct_data(Archive& ar, const dlplan::core::EmptyBoolean<T>* t, const unsigned int /* version */ )
{
    ar << t->m_vocabulary_info;
    ar << t->m_index;
    ar << t->m_element;
}

template<class Archive, typename T>
void load_construct_data(Archive& ar, dlplan::core::EmptyBoolean<T>* t, const unsigned int /* version */ )
{
    std::shared_ptr<dlplan::core::VocabularyInfo> vocabulary;
    int index;
    std::shared_ptr<const T> element;
    ar >> vocabulary;
    ar >> index;
    ar >> element;
    ::new(t)dlplan::core::EmptyBoolean<T>(vocabulary, index, element);
}

}

BOOST_CLASS_EXPORT_GUID(dlplan::core::EmptyBoolean<dlplan::core::Concept>, "dlplan::core::EmptyBoolean<dlplan::core::Concept>")
BOOST_CLASS_EXPORT_GUID(dlplan::core::EmptyBoolean<dlplan::core::Role>, "dlplan::core::EmptyBoolean<dlplan::core::Role>")

#endif
