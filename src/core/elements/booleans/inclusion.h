#ifndef DLPLAN_SRC_CORE_ELEMENTS_BOOLEAN_INCLUSION_H_
#define DLPLAN_SRC_CORE_ELEMENTS_BOOLEAN_INCLUSION_H_

#include "../boolean.h"
#include "../concept.h"
#include "../role.h"
#include "../types.h"


namespace dlplan::core::element {

class ConceptInclusionBoolean : public Boolean {
protected:
    const Concept_Ptr m_concept_left;
    const Concept_Ptr m_concept_right;

public:
    ConceptInclusionBoolean(const VocabularyInfo& vocabulary, Concept_Ptr concept_left, Concept_Ptr concept_right)
    : Boolean(vocabulary, "b_inclusion"), m_concept_left(concept_left), m_concept_right(concept_right) { }

    bool evaluate(const State& state) const override {
        const ConceptDenotation l = m_concept_left->evaluate(state);
        const ConceptDenotation r = m_concept_right->evaluate(state);
        return l.get_const_data().is_subset_of(r.get_const_data());
    }

    int compute_complexity() const override {
        return m_concept_left->compute_complexity() + m_concept_right->compute_complexity() + 1;
    }

    void compute_repr(std::stringstream& out) const override {
       out << m_name << "(";
       m_concept_left->compute_repr(out);
       out << ",";
       m_concept_right->compute_repr(out) ;
       out << ")";
    }
};

class RoleInclusionBoolean : public Boolean {
protected:
    const Role_Ptr m_role_left;
    const Role_Ptr m_role_right;

public:
    RoleInclusionBoolean(const VocabularyInfo& vocabulary, Role_Ptr role_left, Role_Ptr role_right)
    : Boolean(vocabulary, "b_inclusion"), m_role_left(role_left), m_role_right(role_right) { }

    bool evaluate(const State& state) const override {
        const RoleDenotation l = m_role_left->evaluate(state);
        const RoleDenotation r = m_role_right->evaluate(state);
        return l.get_const_data().is_subset_of(r.get_const_data());
    }

    int compute_complexity() const override {
        return m_role_left->compute_complexity() + m_role_right->compute_complexity() + 1;
    }

    void compute_repr(std::stringstream& out) const override {
        out << m_name << "(";
        m_role_left->compute_repr(out);
        out << ",";
        m_role_right->compute_repr(out);
        out << ")";
    }
};

}

#endif