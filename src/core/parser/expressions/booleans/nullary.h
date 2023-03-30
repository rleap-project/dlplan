#ifndef DLPLAN_SRC_CORE_PARSER_EXPRESSIONS_BOOLEANS_NULLARY_H_
#define DLPLAN_SRC_CORE_PARSER_EXPRESSIONS_BOOLEANS_NULLARY_H_

#include "../boolean.h"
#include "../../utils.h"
#include "../../../elements/booleans/nullary.h"


namespace dlplan::core::parser {

class NullaryBoolean : public Boolean {
protected:
    std::unique_ptr<element::Boolean> parse_boolean_impl(const VocabularyInfo& vocabulary, Caches &) const override {
        if (m_children.size() != 1) {
            throw std::runtime_error("NullaryBoolean::parse_boolean_impl - number of children ("s + std::to_string(m_children.size()) + " != 1).");
        }
        // 1. Parse children
        const std::string& predicate_name = m_children[0]->get_name();
        // 2. Construct element
        return std::make_unique<element::NullaryBoolean>(vocabulary, vocabulary.get_predicate(predicate_name));
    }

public:
    NullaryBoolean(const std::string &name, std::vector<std::unique_ptr<Expression>> &&children)
    : Boolean(name, std::move(children)) { }
};

}

#endif
