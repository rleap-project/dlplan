#ifndef DLPLAN_SRC_CORE_PARSER_EXPRESSIONS_CONCEPTS_SOME_H_
#define DLPLAN_SRC_CORE_PARSER_EXPRESSIONS_CONCEPTS_SOME_H_

#include "../concept.h"

using namespace std::string_literals;


namespace dlplan::core::parser {

class SomeConcept : public Concept {
protected:
    std::shared_ptr<const dlplan::core::Concept> parse_concept(SyntacticElementFactory& factory) const override {
        if (m_children.size() != 2) {
            throw std::runtime_error("SomeConcept::parse_concept - number of children ("s + std::to_string(m_children.size()) + " != 2).");
        }
        // 1. Parse children
        auto role = m_children[0]->parse_role(factory);
        auto concept = m_children[1]->parse_concept(factory);
        if (!(role && concept)) {
            throw std::runtime_error("SomeConcept::parse_concept - at least one children is a nullptr");
        }
        // 2. Construct element
        return factory.make_some_concept(role, concept);
    }

public:
    SomeConcept(const std::string &name, std::vector<std::unique_ptr<Expression>> &&children)
    : Concept(name, std::move(children)) { }
};

}

#endif
