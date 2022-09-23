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

    BooleanDenotations* evaluate(const States& states, DenotationsCaches& caches) const override {
        // check if denotations is cached.
        auto cached = caches.m_b_denots_mapping.find(get_index());
        if (cached != caches.m_b_denots_mapping.end()) return cached->second;
        // allocate memory for new denotations
        auto denotations = std::make_unique<BooleanDenotations>();
        denotations->reserve(states.size());
        // get denotations of children
        auto element_left_denotations = m_element_left->evaluate(states, caches);
        auto element_right_denotations = m_element_right->evaluate(states, caches);
        // compute denotations
        for (size_t i = 0; i < states.size(); ++i) {
            bool denotation;
            compute_result(
                *(*element_left_denotations)[i],
                *(*element_right_denotations)[i],
                denotation);
            denotations->push_back(denotation);
        }
        // register denotations and return it.
        auto result_denotations = caches.m_b_denots_cache.insert(std::move(denotations)).first->get();
        caches.m_b_denots_mapping.emplace(get_index(), result_denotations);
        return result_denotations;
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