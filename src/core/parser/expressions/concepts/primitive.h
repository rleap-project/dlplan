#ifndef DLP_SRC_CORE_PARSER_EXPRESSIONS_CONCEPTS_PRIMITIVE_H_
#define DLP_SRC_CORE_PARSER_EXPRESSIONS_CONCEPTS_PRIMITIVE_H_

#include "../concept.h"
#include "../../utils.h"
#include "../../../elements/concepts/primitive.h"


namespace dlp {
namespace core {
namespace parser {

class PrimitiveConceptExpression : public ConceptExpression {
protected:
    virtual element::ConceptElement_Ptr make_concept_element_impl(const InstanceInfoImpl& info, ElementCache &cache) const override {
        if (m_children.size() != 1) {
            throw std::runtime_error("PrimitiveConceptExpression::make_concept_element - number of children ("s + std::to_string(m_children.size()) + " != 1).");
        }
        if (!info.exists_predicate_name(m_name)) {
            throw std::runtime_error("PrimitiveConceptExpression::make_concept_element - predicate ("s + m_name + ") is missing in TaskInfo.");
        }
        unsigned predicate_idx = info.predicate_idx(m_name);
        unsigned predicate_arity = info.predicate_arity(predicate_idx);
        unsigned object_idx = try_parse(m_children[0]->str());
        if (object_idx > predicate_arity) {
            throw std::runtime_error("PrimitiveConceptExpression::make_concept_element - object index does not match predicate arity ("s + std::to_string(object_idx) + " > " + std::to_string(predicate_arity) + ").");
        }
        return std::make_shared<element::PrimitiveConceptElement>(&info, predicate_idx, object_idx);
    }

public:
    PrimitiveConceptExpression(const std::string &name, std::vector<std::unique_ptr<Expression>> &&children)
    : ConceptExpression(name, std::move(children)) { }
};

}
}
}

#endif
