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

    bool evaluate(const State& state, DenotationsCaches& caches) const override {
        bool denotation;
        compute_result(
                *m_element->evaluate(state, caches),
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
        auto element_denotations = m_element->evaluate(states, caches);
        // compute denotations
        for (size_t i = 0; i < states.size(); ++i) {
            bool denotation;
            compute_result(
                *(*element_denotations)[i],
                denotation);
            denotations->push_back(denotation);
        }
        // register denotations and return it.
        auto result_denotations = caches.m_b_denots_cache.insert(std::move(denotations)).first->get();
        caches.m_b_denots_mapping.emplace(get_index(), result_denotations);
        return result_denotations;
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
