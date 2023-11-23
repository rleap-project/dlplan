#ifndef DLPLAN_INCLUDE_DLPLAN_CORE_ELEMENTS_BOOLEANS_EMPTY_H_
#define DLPLAN_INCLUDE_DLPLAN_CORE_ELEMENTS_BOOLEANS_EMPTY_H_

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

    EmptyBoolean(ElementIndex index, std::shared_ptr<VocabularyInfo> vocabulary_info, std::shared_ptr<const T> element)
        : Boolean(index, vocabulary_info, element->is_static()), m_element(element) {
    }

    template<typename... Ts>
    friend class dlplan::ReferenceCountedObjectFactory;

public:
    bool are_equal_impl(const Boolean& other) const override {
        if (typeid(*this) == typeid(other)) {
            const auto& other_derived = static_cast<const EmptyBoolean<T>&>(other);
            return m_is_static == other_derived.m_is_static
                && m_element == other_derived.m_element;
        }
        return false;
    }

    size_t hash_impl() const override {
        return hash_combine(m_is_static, m_element);
    }

    bool evaluate(const State& state) const override {
        bool denotation;
        compute_result(m_element->evaluate(state), denotation);
        return denotation;
    }

    int compute_complexity_impl() const override {
        return m_element->compute_complexity() + 1;
    }

    void str_impl(std::stringstream& out) const override {
        out << "b_empty" << "(";
        m_element->str(out);
        out << ")";
    }

    int compute_evaluate_time_score_impl() const override {
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


#endif
