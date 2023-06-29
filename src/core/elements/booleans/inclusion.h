#ifndef DLPLAN_SRC_CORE_ELEMENTS_BOOLEAN_INCLUSION_H_
#define DLPLAN_SRC_CORE_ELEMENTS_BOOLEAN_INCLUSION_H_

#include "../utils.h"

#include "../../../../include/dlplan/core.h"

#include <sstream>
#include <type_traits>

using namespace std::string_literals;


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

protected:
    const std::shared_ptr<const T> m_element_left;
    const std::shared_ptr<const T> m_element_right;

public:
    InclusionBoolean(std::shared_ptr<const VocabularyInfo> vocabulary_info, std::shared_ptr<const T> element_left, std::shared_ptr<const T> element_right)
    : Boolean(vocabulary_info, element_left->is_static() && element_right->is_static()),
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
       out << get_name() << "(";
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

    static std::string get_name() {
        return "b_inclusion";
    }
};

}

#endif