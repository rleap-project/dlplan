#ifndef DLPLAN_INCLUDE_DLPLAN_CORE_ELEMENTS_NUMERICALS_COUNT_H_
#define DLPLAN_INCLUDE_DLPLAN_CORE_ELEMENTS_NUMERICALS_COUNT_H_

#include "../utils.h"
#include "../../../core.h"

#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/serialization/export.hpp>
#include <boost/serialization/base_object.hpp>
#include <boost/serialization/shared_ptr.hpp>

#include <sstream>
#include <memory>

using namespace std::string_literals;


namespace dlplan {
template<typename... Ts>
class ReferenceCountedObjectFactory;
}


namespace dlplan::core {

template<typename T>
class CountNumerical : public Numerical {
private:
    const std::shared_ptr<const T> m_element;

    template<typename DENOTATION_TYPE>
    void compute_result(const DENOTATION_TYPE& denot, int& result) const {
        result = denot.size();
    }

    int evaluate_impl(const State& state, DenotationsCaches& caches) const override {
        int denotation;
        compute_result(
            *m_element->evaluate(state, caches),
            denotation);
        return denotation;
    }

    NumericalDenotations evaluate_impl(const States& states, DenotationsCaches& caches) const override {
        NumericalDenotations denotations;
        denotations.reserve(states.size());
        auto element_denotations = m_element->evaluate(states, caches);
        for (size_t i = 0; i < states.size(); ++i) {
            int denotation;
            compute_result(
                *(*element_denotations)[i],
                denotation);
            denotations.push_back(denotation);
        }
        return denotations;
    }

    CountNumerical(ElementIndex index, std::shared_ptr<VocabularyInfo> vocabulary_info, std::shared_ptr<const T> element)
        : Numerical(index, vocabulary_info, element->is_static()), m_element(element) { }

    template<typename... Ts>
    friend class dlplan::ReferenceCountedObjectFactory;

public:
    bool are_equal_impl(const Numerical& other) const override {
        if (typeid(*this) == typeid(other)) {
            const auto& other_derived = static_cast<const CountNumerical&>(other);
            return m_is_static == other_derived.m_is_static
                && m_element == other_derived.m_element;
        }
        return false;
    }

    size_t hash_impl() const override {
        return hash_combine(m_is_static, m_element);
    }

    int evaluate(const State& state) const override {
        int result;
        compute_result(
            m_element->evaluate(state),
            result);
        return result;
    }

    int compute_complexity_impl() const override {
        return m_element->compute_complexity() + 1;
    }

    void str_impl(std::stringstream& out) const override {
        out << "n_count" << "(";
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
    struct less<std::shared_ptr<const dlplan::core::CountNumerical<T>>>
    {
        bool operator()(
            const std::shared_ptr<const dlplan::core::CountNumerical<T>>& left_numerical,
            const std::shared_ptr<const dlplan::core::CountNumerical<T>>& right_numerical) const {
            return *left_numerical < *right_numerical;
        }
    };

    template<typename T>
    struct hash<dlplan::core::CountNumerical<T>>
    {
        std::size_t operator()(const dlplan::core::CountNumerical<T>& numerical) const {
            return numerical.hash();
        }
    };
}

#endif