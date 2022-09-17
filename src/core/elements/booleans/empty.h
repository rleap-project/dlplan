#ifndef DLPLAN_SRC_CORE_ELEMENTS_BOOLEAN_EMPTY_H_
#define DLPLAN_SRC_CORE_ELEMENTS_BOOLEAN_EMPTY_H_

#include "../boolean.h"


namespace dlplan::core::element {

template<typename T>
bool compute_result(T&& denotation) {
    return denotation.empty();
}

template<typename T>
class EmptyBoolean : public Boolean {
protected:
    const std::shared_ptr<const T> m_element;

public:
    EmptyBoolean(const VocabularyInfo& vocabulary, std::shared_ptr<const T> element)
        : Boolean(vocabulary), m_element(element) {
    }

    bool evaluate(const State& state) const override {
        return compute_result(m_element->evaluate(state));
    }

    bool evaluate(const State& state, EvaluationCaches& cache) const override {
        return compute_result(m_element->evaluate(state, cache));
    }

    int compute_complexity() const override {
        return m_element->compute_complexity() + 1;
    }

    void compute_repr(std::stringstream& out) const {
        out << get_name() << "(";
        m_element->compute_repr(out);
        out << ")";
    }

    static std::string get_name() {
        return "b_empty";
    }
};

}

#endif
