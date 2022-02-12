#ifndef DLPLAN_SRC_CORE_ELEMENTS_BOOLEAN_INCLUSION_H_
#define DLPLAN_SRC_CORE_ELEMENTS_BOOLEAN_INCLUSION_H_

#include "../boolean.h"
#include "../concept.h"
#include "../role.h"
#include "../types.h"


namespace dlplan::core::element {

template<typename T>
class InclusionBoolean : public Boolean {
protected:
    const std::shared_ptr<const T> m_element_left;
    const std::shared_ptr<const T> m_element_right;

public:
    InclusionBoolean(const VocabularyInfo& vocabulary, std::shared_ptr<const T> element_left, std::shared_ptr<const T> element_right)
    : Boolean(vocabulary, "b_inclusion"), m_element_left(element_left), m_element_right(element_right) {
    }

    bool evaluate(const State& state) const override {
        const auto l = m_element_left->evaluate(state);
        const auto r = m_element_right->evaluate(state);
        return l.get_data().is_subset_of(r.get_data());
    }

    int compute_complexity() const override {
        return m_element_left->compute_complexity() + m_element_right->compute_complexity() + 1;
    }

    void compute_repr(std::stringstream& out) const override {
       out << m_name << "(";
       m_element_left->compute_repr(out);
       out << ",";
       m_element_right->compute_repr(out) ;
       out << ")";
    }
};

}

#endif