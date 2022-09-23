#ifndef DLPLAN_SRC_CORE_ELEMENTS_NUMERICAL_COUNT_H_
#define DLPLAN_SRC_CORE_ELEMENTS_NUMERICAL_COUNT_H_

#include "../numerical.h"


namespace dlplan::core::element {

template<typename T>
class CountNumerical : public Numerical {
private:
    template<typename DENOTATION_TYPE>
    void compute_result(const DENOTATION_TYPE& denot, int& result) const {
        result = denot.count();
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

    const std::vector<int>& evaluate(const States& states, DenotationsCaches& caches) const override {
        // check if denotations is cached.
        auto cached = caches.m_n_denots_mapping.find(get_index());
        if (cached != caches.m_n_denots_mapping.end()) return cached->second;
        // allocate memory for new denotations
        NumericalDenotationsPtr denotations = std::make_unique<NumericalDenotations>();
        denotations->reserve(states.size());
        // get denotations of children
        auto element_denotations = m_element->evaluate(states, caches);
        // compute denotations
        for (size_t i = 0; i < states.size(); ++i) {
            int denotation;
            compute_result(
                element_denotations[i].get(),
                denotation);
            denotations->push_back(denotation);
        }
        // register denotations and return it.
        auto result_denotations = std::cref(*caches.m_n_denots_cache.insert(std::move(denotations)).first->get());
        caches.m_n_denots_mapping.emplace(get_index(), result_denotations);
        return result_denotations;
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