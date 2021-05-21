#ifndef DLP_SRC_ELEMENT_FACTORY_CONCEPT_ELEMENTS_CONCEPT_ELEMENT_H_
#define DLP_SRC_ELEMENT_FACTORY_CONCEPT_ELEMENTS_CONCEPT_ELEMENT_H_

#include <memory>

#include "../element.h"


namespace dlp {

class ConceptElement : public Element {
public:
    ConceptElement();

    virtual std::size_t hash_value() const override;
};

}

#endif
