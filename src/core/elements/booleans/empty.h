#ifndef DLPLAN_SRC_CORE_ELEMENTS_BOOLEAN_EMPTY_H_
#define DLPLAN_SRC_CORE_ELEMENTS_BOOLEAN_EMPTY_H_

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

template<typename T>
class EmptyBoolean;
}


namespace boost::serialization {
    template<typename Archive, typename T>
    void serialize(Archive& ar, dlplan::core::EmptyBoolean<T>& t, const unsigned int version);
    template<class Archive, typename T>
    void save_construct_data(Archive& ar, const dlplan::core::EmptyBoolean<T>* t, const unsigned int version);
    template<class Archive, typename T>
    void load_construct_data(Archive& ar, dlplan::core::EmptyBoolean<T>* t, const unsigned int version);
}


namespace dlplan::core {
template<typename T>
class EmptyBoolean : public Boolean {
private:
    const std::shared_ptr<const T> m_element;

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
    friend void boost::serialization::serialize(Archive& ar, EmptyBoolean<T_>& t, const unsigned int version);
    template<class Archive, typename T_>
    friend void boost::serialization::save_construct_data(Archive & ar, const EmptyBoolean<T_>* t, const unsigned int version);
    template<class Archive, typename T_>
    friend void boost::serialization::load_construct_data(Archive & ar, EmptyBoolean<T_>* t, const unsigned int version);

    EmptyBoolean(ElementIndex index, std::shared_ptr<VocabularyInfo> vocabulary_info, std::shared_ptr<const T> element)
        : Boolean(vocabulary_info, index, element->is_static()), m_element(element) {
    }

    template<typename... Ts>
    friend class dlplan::utils::ReferenceCountedObjectFactory;

public:
    bool operator==(const Boolean& other) const {
        if (typeid(*this) == typeid(other)) {
            const auto& other_derived = static_cast<const EmptyBoolean<T>&>(other);
            return m_element == other_derived.m_element;
        }
        return false;
    }

    bool operator!=(const Boolean& other) const {
        return !(*this == other);
    }

    bool operator<(const EmptyBoolean<T>& other) const {
        return m_index < other.m_index;
    }

    size_t hash() const {
        return std::hash<std::shared_ptr<const T>>()(m_element);
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
void serialize(Archive& ar, dlplan::core::EmptyBoolean<T>& t, const unsigned int /* version */ )
{
    boost::serialization::base_object<dlplan::core::Boolean>(t);
}

template<class Archive, typename T>
void save_construct_data(Archive& ar, const dlplan::core::EmptyBoolean<T>* t, const unsigned int /* version */ )
{
    ar << t->m_index;
    ar << t->m_vocabulary_info;
    ar << t->m_element;
}

template<class Archive, typename T>
void load_construct_data(Archive& ar, dlplan::core::EmptyBoolean<T>* t, const unsigned int /* version */ )
{
    int index;
    std::shared_ptr<dlplan::core::VocabularyInfo> vocabulary;
    std::shared_ptr<const T> element;
    ar >> index;
    ar >> vocabulary;
    ar >> element;
    ::new(t)dlplan::core::EmptyBoolean<T>(index, vocabulary, element);
}

}

namespace std {
    template<typename T>
    struct less<std::shared_ptr<const dlplan::core::EmptyBoolean<T>>>
    {
        bool operator()(
            const std::shared_ptr<const dlplan::core::EmptyBoolean<T>>& left_boolean,
            const std::shared_ptr<const dlplan::core::EmptyBoolean<T>>& right_boolean) const {
            return *left_boolean < *right_boolean;
        }
    };

    template<typename T>
    struct hash<dlplan::core::EmptyBoolean<T>>
    {
        std::size_t operator()(const dlplan::core::EmptyBoolean<T>& boolean) const {
            return boolean.hash();
        }
    };
}

BOOST_CLASS_EXPORT_GUID(dlplan::core::EmptyBoolean<dlplan::core::Concept>, "dlplan::core::EmptyBoolean<dlplan::core::Concept>")
BOOST_CLASS_EXPORT_GUID(dlplan::core::EmptyBoolean<dlplan::core::Role>, "dlplan::core::EmptyBoolean<dlplan::core::Role>")

#endif
