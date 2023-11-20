#ifndef DLPLAN_SRC_CORE_ELEMENTS_BOOLEAN_INCLUSION_H_
#define DLPLAN_SRC_CORE_ELEMENTS_BOOLEAN_INCLUSION_H_

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
class InclusionBoolean;
}


namespace boost::serialization {
    template<typename Archive, typename T>
    void serialize(Archive& ar, dlplan::core::InclusionBoolean<T>& t, const unsigned int version);
    template<class Archive, typename T>
    void save_construct_data(Archive& ar, const dlplan::core::InclusionBoolean<T>* t, const unsigned int version);
    template<class Archive, typename T>
    void load_construct_data(Archive& ar, dlplan::core::InclusionBoolean<T>* t, const unsigned int version);

    template<typename Archive, typename T>
    void serialize(Archive& ar, std::pair<const dlplan::core::InclusionBoolean<T>, std::weak_ptr<dlplan::core::InclusionBoolean<T>>>& t, const unsigned int version);
    template<class Archive, typename T>
    void save_construct_data(Archive& ar, const std::pair<const dlplan::core::InclusionBoolean<T>, std::weak_ptr<dlplan::core::InclusionBoolean<T>>>* t, const unsigned int version);
    template<class Archive, typename T>
    void load_construct_data(Archive& ar, std::pair<const dlplan::core::InclusionBoolean<T>, std::weak_ptr<dlplan::core::InclusionBoolean<T>>>* t, const unsigned int version);
}


namespace dlplan::core {
template<typename T>
class InclusionBoolean : public Boolean {
private:
    const std::shared_ptr<const T> m_element_left;
    const std::shared_ptr<const T> m_element_right;

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

    InclusionBoolean(ElementIndex index, std::shared_ptr<VocabularyInfo> vocabulary_info, std::shared_ptr<const T> element_left, std::shared_ptr<const T> element_right)
    : Boolean(vocabulary_info, index, element_left->is_static() && element_right->is_static()),
      m_element_left(element_left),
      m_element_right(element_right) {
    }

    template<typename Archive, typename T_>
    friend void boost::serialization::serialize(Archive& ar, InclusionBoolean<T_>& t, const unsigned int version);
    template<class Archive, typename T_>
    friend void boost::serialization::save_construct_data(Archive& ar, const InclusionBoolean<T_>* t, const unsigned int version);
    template<class Archive, typename T_>
    friend void boost::serialization::load_construct_data(Archive& ar, InclusionBoolean<T_>* t, const unsigned int version);
    template<typename... Ts>
    friend class dlplan::utils::ReferenceCountedObjectFactory;

public:
    bool operator==(const Boolean& other) const override {
        if (typeid(*this) == typeid(other)) {
            const auto& other_derived = static_cast<const InclusionBoolean<T>&>(other);
            return m_is_static == other_derived.m_is_static
                && m_element_left == other_derived.m_element_left
                && m_element_right == other_derived.m_element_right;
        }
        return false;
    }

    size_t hash() const {
        return dlplan::utils::hash_combine(m_is_static, m_element_left, m_element_right);
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
    ::new(t)dlplan::core::InclusionBoolean<T>(index, vocabulary, element_left, element_right);
}


template<typename Archive, typename T>
void serialize(Archive& /*ar*/, std::pair<const dlplan::core::InclusionBoolean<T>, std::weak_ptr<dlplan::core::InclusionBoolean<T>>>& /*t*/, const unsigned int /*version*/) {
}

template<class Archive, typename T>
void save_construct_data(Archive& ar, const std::pair<const dlplan::core::InclusionBoolean<T>, std::weak_ptr<dlplan::core::InclusionBoolean<T>>>* t, const unsigned int /*version*/) {
    ar << t->first;
    ar << t->second;
}

template<class Archive, typename T>
void load_construct_data(Archive& ar, std::pair<const dlplan::core::InclusionBoolean<T>, std::weak_ptr<dlplan::core::InclusionBoolean<T>>>* t, const unsigned int /*version*/) {
    dlplan::core::InclusionBoolean<T>* first = nullptr;
    std::weak_ptr<dlplan::core::InclusionBoolean<T>>* second = nullptr;
    ar >> const_cast<dlplan::core::InclusionBoolean<T>&>(t->first);
    ar >> t->second;
    ::new(t)std::pair<const dlplan::core::InclusionBoolean<T>, std::weak_ptr<dlplan::core::InclusionBoolean<T>>>(*first, *second);
    delete first;
    delete second;
}

}

BOOST_CLASS_EXPORT_KEY2(dlplan::core::InclusionBoolean<dlplan::core::Concept>, "dlplan::core::InclusionBoolean<dlplan::core::Concept>")
BOOST_CLASS_EXPORT_KEY2(dlplan::core::InclusionBoolean<dlplan::core::Role>, "dlplan::core::InclusionBoolean<dlplan::core::Role>")


namespace std {
    template<typename T>
    struct less<std::shared_ptr<const dlplan::core::InclusionBoolean<T>>>
    {
        bool operator()(
            const std::shared_ptr<const dlplan::core::InclusionBoolean<T>>& left_boolean,
            const std::shared_ptr<const dlplan::core::InclusionBoolean<T>>& right_boolean) const {
            return *left_boolean < *right_boolean;
        }
    };

    template<typename T>
    struct hash<dlplan::core::InclusionBoolean<T>>
    {
        std::size_t operator()(const dlplan::core::InclusionBoolean<T>& boolean) const {
            return boolean.hash();
        }
    };
}


#endif