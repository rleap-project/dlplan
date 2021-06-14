#ifndef DLPLAN_SRC_CORE_CACHE_H_
#define DLPLAN_SRC_CORE_CACHE_H_

#include <unordered_map>
#include <iostream>

#include "elements/types.h"


namespace dlplan::core {

/*
Andrès:

Hey! Just some basic info on normal forms for description logic.
It seems that DL people often normalise KBs using negation normal forms (NNFs) or Tseitin’s transformations.
The latter requires only a linear increase in the size of the formulas but only preserves equisatisfiability,
so I don't think it'll do for this application.
NNFs are equivalence-preserving and the transformation of a concept into NNF can also be done in linear time.
You just push negation inwards, make use of de Morgan’s laws and the duality between existential and universal restrictions,
and between at-most and at-least number restrictions (<r.C, > n r.C). Basically, you follow the translation on the top of p.6 here:

https://arxiv.org/pdf/1202.0914.pdf

What's not so nice is that NNFs are not canonical,
so you can't detect if a concept C would be a duplicate by simply transforming it into NNF
and checking if C already occurs in the set of previously generated concepts.
If you insist on using canonical forms to check for duplicates,
then the NNFs can be transformed into CNF or DNF using distributivity.
But as you may know, this can be impractical, as the transformation may cause an exponential blowup in the size of the formulas.
*/
class ElementCache {
private:
    /**
     * One cache for each template instantiated element.
     */
    std::unordered_map<std::string, element::Concept_Ptr> m_concept_element_cache;
    std::unordered_map<std::string, element::Role_Ptr> m_role_element_cache;
    std::unordered_map<std::string, element::Numerical_Ptr> m_numerical_element_cache;
    std::unordered_map<std::string, element::Boolean_Ptr> m_boolean_element_cache;

public:
    ElementCache();

    std::unordered_map<std::string, element::Concept_Ptr>& concept_element_cache();
    std::unordered_map<std::string, element::Role_Ptr>& role_element_cache();
    std::unordered_map<std::string, element::Numerical_Ptr>& numerical_element_cache();
    std::unordered_map<std::string, element::Boolean_Ptr>& boolean_element_cache();

    void on_destroy(const core::Concept& concept);
    void on_destroy(const core::Role& role);
    void on_destroy(const core::Numerical& numerical);
    void on_destroy(const core::Boolean& boolean);
};


}

#endif