#ifndef DLPLAN_SRC_CORE_ELEMENTS_BOOLEAN_EMPTY_H_
#define DLPLAN_SRC_CORE_ELEMENTS_BOOLEAN_EMPTY_H_

#include "../boolean.h"


namespace dlplan::core::element {

template<typename T>
class EmptyBoolean : public Boolean {
protected:
    const T m_element;

public:
    EmptyBoolean(const VocabularyInfo& vocabulary, T element)
    : Boolean(vocabulary, "b_empty"), m_element(element) { }

    bool evaluate(const State& state) const override {
        return m_element->evaluate(state).get_const_data().none();
    }

    int compute_complexity() const override {
        return m_element->compute_complexity() + 1;
    }

    void compute_repr(std::stringstream& out) const {
        out << m_name << "(";
        m_element->compute_repr(out);
        out << ")";
    }
};

}

#endif
