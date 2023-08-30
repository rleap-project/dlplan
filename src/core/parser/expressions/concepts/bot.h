#ifndef DLPLAN_SRC_CORE_PARSER_EXPRESSIONS_CONCEPTS_BOT_H_
#define DLPLAN_SRC_CORE_PARSER_EXPRESSIONS_CONCEPTS_BOT_H_

#include "../concept.h"

using namespace std::string_literals;


namespace dlplan::core::parser {

class BotConcept : public Concept {
private:
    static inline const std::string m_name = "c_bot";

public:
    BotConcept(const std::string &name, std::vector<std::unique_ptr<Expression>> &&children)
    : Concept(name, std::move(children)) { }

    std::shared_ptr<const dlplan::core::Concept> parse_concept(SyntacticElementFactory& factory) const override {
        if (m_children.size() != 0) {
            throw std::runtime_error("BotConcept::parse_concept - number of children ("s + std::to_string(m_children.size()) + " != 0).");
        }
        // 2. Construct element
        return factory.make_bot_concept();
    }

    static const std::string& get_name() {
        return m_name;
    }
};

}

#endif
