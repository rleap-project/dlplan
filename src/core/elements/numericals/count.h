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
    virtual int evaluate_impl(const StateImpl& state) override {
        m_element->evaluate(state);
        return m_element->size();
    }

public:
    CountNumericalElement(std::shared_ptr<InstanceInfoImpl> parent, T element) : NumericalElement(parent, "n_count"), m_element(element) { }

    virtual unsigned complexity() const override {
        return m_element->complexity() + 1;
    }

    virtual std::string repr() const override {
        std::stringstream ss;
        ss << m_name << "(" << m_element->repr() << ")";
        return ss.str();
    }
};

}
}
}

#endif