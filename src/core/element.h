#ifndef DLPLAN_SRC_CORE_ELEMENT_H_
#define DLPLAN_SRC_CORE_ELEMENT_H_

#include "elements/element.h"


namespace dlplan::core {

/**
 * The ElementRoot stores information from which the underlying Element was constructed.
 */
template<typename T>
class ElementImpl {
public:
    ElementImpl() = default;
    virtual ~ElementImpl() = default;

    virtual T evaluate(const Element<T>* parent, const State& state) const = 0;

    /*
        Andrés:

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
};

}

#endif
