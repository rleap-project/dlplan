#ifndef DLPLAN_SRC_CORE_ELEMENTS_NUMERICAL_COUNT_H_
#define DLPLAN_SRC_CORE_ELEMENTS_NUMERICAL_COUNT_H_

#include "../numerical.h"


namespace dlplan::core::element {

template<typename T>
class CountNumerical : public Numerical {
protected:
    const T m_element;

public:
    CountNumerical(const VocabularyInfo& vocabulary, T element)
    : Numerical(vocabulary, "n_count"), m_element(element) { }

    int evaluate(const State& state, PerElementEvaluationCache& caches) const override {
        return m_element->evaluate(state, caches).get_data().count();
    }

    int compute_complexity() const override {
        return m_element->compute_complexity() + 1;
    }

    void compute_repr(std::stringstream& out) const override {
        out << m_name << "(";
        m_element->compute_repr(out);
        out << ")";
    }
};

}

#endif