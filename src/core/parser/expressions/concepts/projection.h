#ifndef DLPLAN_SRC_CORE_PARSER_EXPRESSIONS_CONCEPTS_PROJECTION_H_
#define DLPLAN_SRC_CORE_PARSER_EXPRESSIONS_CONCEPTS_PROJECTION_H_

#include "../concept.h"
#include "../../utils.h"
#include "../../../elements/concepts/projection.h"


namespace dlplan::core::parser {

class ProjectionConcept : public Concept {
protected:
    std::unique_ptr<dlplan::core::Concept> parse_concept_impl(std::shared_ptr<const VocabularyInfo> vocabulary_info, Caches &caches) const override {
        if (m_children.size() != 2) {
            throw std::runtime_error("ProjectionConcept::parse_concept_impl - number of children ("s + std::to_string(m_children.size()) + " != 1).");
        }
        std::shared_ptr<const dlplan::core::Role> role = m_children[0]->parse_role(vocabulary_info, caches);
        if (!role) {
            throw std::runtime_error("ProjectionConcept::parse_concept_impl - child is a nullptr.");
        }
        int pos = try_parse_number(m_children[1]->get_name());
        if (pos < 0 || pos > 1) {
            throw std::runtime_error("ProjectionConcept::parse_concept_impl - projection index out of range, should be 0 or 1 ("s + std::to_string(pos) + ")");
        }
        return std::make_unique<dlplan::core::ProjectionConcept>(vocabulary_info, role, pos);
    }

public:
    ProjectionConcept(const std::string &name, std::vector<std::unique_ptr<Expression>> &&children)
    : Concept(name, std::move(children)) { }
};

}

#endif
