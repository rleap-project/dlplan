#ifndef DLPLAN_SRC_CORE_PARSER_EXPRESSIONS_BOOLEAN_EMPTY_H_
#define DLPLAN_SRC_CORE_PARSER_EXPRESSIONS_BOOLEAN_EMPTY_H_

#include "../boolean.h"
#include "../../../elements/booleans/empty.h"

namespace dlplan::core::parser {

class EmptyBoolean : public Boolean {
protected:
    std::unique_ptr<dlplan::core::Boolean> parse_boolean_impl(std::shared_ptr<const VocabularyInfo> vocabulary_info, Caches &cache) const override {
        if (m_children.size() != 1) {
            throw std::runtime_error("EmptyBoolean::parse_boolean_impl - number of children ("s + std::to_string(m_children.size()) + " != 1).");
        }
        // 1. Parse and construct children
        std::shared_ptr<const dlplan::core::Concept> concept_element = m_children[0]->parse_concept(vocabulary_info, cache);
        if (concept_element) {
            return std::make_unique<dlplan::core::EmptyBoolean<dlplan::core::Concept>>(vocabulary_info, concept_element);
        }
        std::shared_ptr<const dlplan::core::Role> role_element = m_children[0]->parse_role(vocabulary_info, cache);
        if (role_element) {
            return std::make_unique<dlplan::core::EmptyBoolean<dlplan::core::Role>>(vocabulary_info, role_element);
        }
        // 2. If unsuccessful then throw a runtime error.
        throw std::runtime_error("EmptyBoolean::parse_boolean_impl - unable to construct children elements.");
    }

public:
    EmptyBoolean(const std::string &name, std::vector<std::unique_ptr<Expression>> &&children)
    : Boolean(name, std::move(children)) { }
};

}

#endif
