#ifndef DLP_SRC_CORE_ELEMENTS_NUMERICAL_COUNT_H_
#define DLP_SRC_CORE_ELEMENTS_NUMERICAL_COUNT_H_

#include "../numerical.h"
#include "../concept.h"
#include "../role.h"


namespace dlp {
namespace core {
namespace element {

template<typename T>
class CountNumericalElement : public NumericalElement {
protected:
    T m_element;

protected:
    virtual const int& evaluate_impl(const StateImpl& state) override {
        m_result = m_element->evaluate(state).size();
        return m_result;
    }

public:
    CountNumericalElement(T element) : NumericalElement("n_count"), m_element(element) { }

    virtual unsigned compute_complexity() const override {
        return m_element->compute_complexity() + 1;
    }

    virtual std::string compute_repr() const override {
        std::stringstream ss;
        ss << m_name << "(" << m_element->compute_repr() << ")";
        return ss.str();
    }
};

}
}
}

#endif