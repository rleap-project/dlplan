#ifndef DLPLAN_SRC_CORE_ELEMENTS_BOOLEAN_EMPTY_H_
#define DLPLAN_SRC_CORE_ELEMENTS_BOOLEAN_EMPTY_H_

#include "../boolean.h"


namespace dlplan::core::element {


template<typename T>
class EmptyBoolean : public Boolean {
private:
    template<typename DENOTATION_TYPE>
    void compute_result(const DENOTATION_TYPE& denotation, bool& result) const {
        result = denotation.empty();
    }

protected:
    const std::shared_ptr<const T> m_element;

public:
    EmptyBoolean(const VocabularyInfo& vocabulary, std::shared_ptr<const T> element)
        : Boolean(vocabulary), m_element(element) {
    }

    bool evaluate(const State& state) const override {
        bool denotation;
        compute_result(m_element->evaluate(state), denotation);
        return denotation;
    }

    DENOTS<bool> evaluate(const States& states, DenotationsCaches& caches) const override {
        auto boolean_cache_entry = cache.m_boolean_denotation_cache.find(state, *this);
        auto& status = boolean_cache_entry->m_status;
        auto& denotation = boolean_cache_entry->m_denotation;
        if (status) {
            return &denotation;
        }
        compute_result(
            *m_element->evaluate(state, cache),
            denotation);
        status = true;
        return &denotation;
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
