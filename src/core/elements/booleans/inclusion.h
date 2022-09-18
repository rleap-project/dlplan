#ifndef DLPLAN_SRC_CORE_ELEMENTS_BOOLEAN_INCLUSION_H_
#define DLPLAN_SRC_CORE_ELEMENTS_BOOLEAN_INCLUSION_H_

#include "../boolean.h"
#include "../concept.h"
#include "../role.h"
#include "../types.h"


namespace dlplan::core::element {

template<typename T>
class InclusionBoolean : public Boolean {
private:
    template<typename DENOTATION_TYPE>
    void compute_result(const DENOTATION_TYPE& denot_left, const DENOTATION_TYPE& denot_right, bool& result) const {
        result = denot_left.is_subset_of(denot_right);
    }

protected:
    const std::shared_ptr<const T> m_element_left;
    const std::shared_ptr<const T> m_element_right;

public:
    InclusionBoolean(const VocabularyInfo& vocabulary, std::shared_ptr<const T> element_left, std::shared_ptr<const T> element_right)
    : Boolean(vocabulary), m_element_left(element_left), m_element_right(element_right) {
    }

    bool evaluate(const State& state) const override {
        bool denotation;
        compute_result(
            m_element_left->evaluate(state),
            m_element_right->evaluate(state),
            denotation);
        return denotation;
    }

    const bool* evaluate(const State& state, GeneratorEvaluationCaches& cache) const override {
        auto boolean_cache_entry = cache.m_boolean_denotation_cache.find(state, *this);
        auto& status = boolean_cache_entry->m_status;
        auto& denotation = boolean_cache_entry->m_denotation;
        if (status) {
            return &denotation;
        }
        compute_result(
            *m_element_left->evaluate(state, cache),
            *m_element_right->evaluate(state, cache),
            denotation);
        status = true;
        return &denotation;
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