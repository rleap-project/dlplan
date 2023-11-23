#ifndef DLPLAN_INCLUDE_DLPLAN_CORE_ELEMENTS_BOOLEANS_INCLUSION_H_
#define DLPLAN_INCLUDE_DLPLAN_CORE_ELEMENTS_BOOLEANS_INCLUSION_H_

#include "../utils.h"
#include "../../../core.h"

#include <sstream>
#include <memory>

using namespace std::string_literals;


namespace dlplan {
template<typename... Ts>
class ReferenceCountedObjectFactory;
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
    : Boolean(index, vocabulary_info, element_left->is_static() && element_right->is_static()),
      m_element_left(element_left),
      m_element_right(element_right) {
    }

    template<typename... Ts>
    friend class dlplan::ReferenceCountedObjectFactory;

public:
    bool are_equal_impl(const Boolean& other) const override {
        if (typeid(*this) == typeid(other)) {
            const auto& other_derived = static_cast<const InclusionBoolean<T>&>(other);
            return m_is_static == other_derived.m_is_static
                && m_element_left == other_derived.m_element_left
                && m_element_right == other_derived.m_element_right;
        }
        return false;
    }

    size_t hash_impl() const override {
        return hash_combine(m_is_static, m_element_left, m_element_right);
    }

    bool evaluate(const State& state) const override {
        bool denotation;
        compute_result(
            m_element_left->evaluate(state),
            m_element_right->evaluate(state),
            denotation);
        return denotation;
    }

    int compute_complexity_impl() const override {
        return m_element_left->compute_complexity() + m_element_right->compute_complexity() + 1;
    }

    void str_impl(std::stringstream& out) const override {
       out << "b_inclusion" << "(";
       m_element_left->str(out);
       out << ",";
       m_element_right->str(out) ;
       out << ")";
    }

    int compute_evaluate_time_score_impl() const override {
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