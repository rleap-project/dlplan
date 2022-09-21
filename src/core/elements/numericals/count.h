#ifndef DLPLAN_SRC_CORE_ELEMENTS_NUMERICAL_COUNT_H_
#define DLPLAN_SRC_CORE_ELEMENTS_NUMERICAL_COUNT_H_

#include "../numerical.h"


namespace dlplan::core::element {

template<typename T>
class CountNumerical : public Numerical {
private:
    template<typename DENOTATION_TYPE>
    void compute_result(const DENOTATION_TYPE& denot, int& result) const {
        result = denot.size();
    }

protected:
    const T m_element;

public:
    CountNumerical(const VocabularyInfo& vocabulary, T element)
    : Numerical(vocabulary), m_element(element) { }

    int evaluate(const State& state) const override {
        int result;
        compute_result(
            m_element->evaluate(state),
            result);
        return result;
    }

    DENOTS<int> evaluate(const States& states, DenotationsCaches& caches) const override {
        auto numerical_cache_entry = cache.m_numerical_denotation_cache.find(state, *this);
        auto& status = numerical_cache_entry->m_status;
        auto& denotation = numerical_cache_entry->m_denotation;
        if (status) {
            return &denotation;
        }
        compute_result(
            *m_element->evaluate(state, cache),
            denotation);
        status = true;
        /*std::stringstream ss;
        compute_repr(ss);
        std::cout << "evaluation: " << ss.str() << " " << denotation << std::endl;*/
        return &denotation;
    }

    int compute_complexity() const override {
        return m_element->compute_complexity() + 1;
    }

    void compute_repr(std::stringstream& out) const override {
        out << get_name() << "(";
        m_element->compute_repr(out);
        out << ")";
    }

    static std::string get_name() {
        return "n_count";
    }
};

}

#endif