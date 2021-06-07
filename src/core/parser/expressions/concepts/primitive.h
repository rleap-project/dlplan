#ifndef DLP_SRC_CORE_PARSER_EXPRESSIONS_CONCEPTS_PRIMITIVE_H_
#define DLP_SRC_CORE_PARSER_EXPRESSIONS_CONCEPTS_PRIMITIVE_H_

#include "../concept.h"
#include "../../utils.h"
#include "../../../elements/concepts/primitive.h"


namespace dlp {
namespace core {
namespace parser {

class PrimitiveConcept : public Concept {
protected:
    virtual element::Concept_Ptr parse_concept_impl(const VocabularyInfo& vocabulary, ElementCache &cache) const override {
        if (m_children.size() != 1) {
            throw std::runtime_error("PrimitiveConcept::parse_concept_impl - number of children ("s + std::to_string(m_children.size()) + " != 1).");
        }
        // 1. Parse children
        unsigned pos = try_parse_number(m_children[0]->get_name());
        // 2. Construct element
        return std::make_shared<element::PrimitiveConcept>(vocabulary, m_name, pos);
    }

public:
    PrimitiveConcept(const std::string &name, std::vector<std::unique_ptr<Expression>> &&children)
    : Concept(name, std::move(children)) { }
};

}
}
}

#endif
