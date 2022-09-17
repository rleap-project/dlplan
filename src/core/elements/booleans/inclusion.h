#ifndef DLPLAN_SRC_CORE_ELEMENTS_BOOLEAN_INCLUSION_H_
#define DLPLAN_SRC_CORE_ELEMENTS_BOOLEAN_INCLUSION_H_

#include "../boolean.h"
#include "../concept.h"
#include "../role.h"
#include "../types.h"


namespace dlplan::core::element {

template<typename T>
static bool compute_result(T&& denot_left, T&& denot_right) {
    return denot_left.is_subset_of(denot_right);
}

template<typename T>
class InclusionBoolean : public Boolean {
protected:
    const std::shared_ptr<const T> m_element_left;
    const std::shared_ptr<const T> m_element_right;

public:
    InclusionBoolean(const VocabularyInfo& vocabulary, std::shared_ptr<const T> element_left, std::shared_ptr<const T> element_right)
    : Boolean(vocabulary), m_element_left(element_left), m_element_right(element_right) {
    }

    bool evaluate(const State& state) const override {
        return compute_result(m_element_left->evaluate(state), m_element_right->evaluate(state));
    }

    bool evaluate(const State& state, EvaluationCaches& cache) const override {
        return compute_result(m_element_left->evaluate(state, cache), m_element_right->evaluate(state, cache));
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

    static std::string get_name() {
        return "b_inclusion";
    }
};

}

#endif