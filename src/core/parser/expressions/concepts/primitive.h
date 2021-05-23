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
    virtual ConceptElement_Ptr make_concept_element_impl(std::shared_ptr<TaskInfo> task_info, ElementCache &cache) const override {
        if (m_children.size() != 1) {
            throw std::runtime_error("PrimitiveConceptExpression::make_concept_element - number of children ("s + std::to_string(m_children.size()) + " != 1).");
        }
        std::string predicate_name = strip_type_identifier(m_name);
        if (!task_info->exists_predicate_name(predicate_name)) {
            throw std::runtime_error("PrimitiveConceptExpression::make_concept_element - predicate ("s + predicate_name + ") is missing in TaskInfo.");
        }
        bool goal = is_goal_version(m_name);
        unsigned predicate_idx = task_info->predicate_idx(predicate_name);
        unsigned predicate_arity = task_info->predicate_arity(predicate_idx);
        unsigned object_idx = try_parse(m_children[0]->str());
        if (object_idx > predicate_arity) {
            throw std::runtime_error("PrimitiveConceptExpression::make_concept_element - object index does not match predicate arity ("s + std::to_string(object_idx) + " > " + std::to_string(predicate_arity) + ").");
        }
        return std::make_shared<PrimitiveConceptElement>(task_info, goal, predicate_idx, object_idx);
    }

public:
    PrimitiveConceptExpression(const std::string &name, std::vector<std::unique_ptr<Expression>> &&children)
    : ConceptExpression(name, std::move(children)) { }
};

}
}
}

#endif
