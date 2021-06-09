#ifndef DLP_SRC_CORE_ELEMENTS_NUMERICAL_COUNT_H_
#define DLP_SRC_CORE_ELEMENTS_NUMERICAL_COUNT_H_

#include "../numerical.h"


namespace dlp {
namespace core {
namespace element {

template<typename T>
class CountNumerical : public Numerical {
protected:
    const T m_element;

protected:
    virtual const int& evaluate_impl(const State& state) override {
        m_result = m_element->evaluate(state).size();
        return m_result;
    }

public:
    CountNumerical(const VocabularyInfo& vocabulary, T element) : Numerical(vocabulary, "n_count"), m_element(element) { }

    virtual int compute_complexity() const override {
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