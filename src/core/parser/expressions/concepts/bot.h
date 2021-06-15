#ifndef DLPLAN_SRC_CORE_PARSER_EXPRESSIONS_CONCEPTS_BOT_H_
#define DLPLAN_SRC_CORE_PARSER_EXPRESSIONS_CONCEPTS_BOT_H_

#include "../concept.h"
#include "../../../elements/concepts/bot.h"

namespace dlplan::core::parser {

class BotConcept : public Concept {
protected:
    std::unique_ptr<element::Concept> parse_concept_impl(const VocabularyInfo& vocabulary, Caches &) const override {
        if (m_children.size() != 0) {
            throw std::runtime_error("BotConcept::parse_concept_impl - number of children ("s + std::to_string(m_children.size()) + " != 0).");
        }
        // 2. Construct element
        return std::make_unique<element::BotConcept>(vocabulary);
    }

public:
    BotConcept(const std::string &name, std::vector<std::unique_ptr<Expression>> &&children)
    : Concept(name, std::move(children)) { }
};

}

#endif
