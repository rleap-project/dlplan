#ifndef DLP_SRC_CORE_ELEMENTS_BOOLEAN_EMPTY_H_
#define DLP_SRC_CORE_ELEMENTS_BOOLEAN_EMPTY_H_

#include "../boolean.h"


namespace dlp {
namespace core {
namespace element {

template<typename T>
class EmptyBoolean : public Boolean {
protected:
    const T m_element;

protected:
    virtual const bool& evaluate_impl(const StateImpl& state) override {
        m_result = m_element->evaluate(state).empty();
        return m_result;
    }

public:
    EmptyBoolean(const VocabularyInfoImpl& vocabulary, T element) : Boolean(vocabulary, "b_empty"), m_element(element) { }

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