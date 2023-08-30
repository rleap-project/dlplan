#ifndef DLPLAN_SRC_CORE_PARSER_EXPRESSIONS_CONCEPTS_AND_H_
#define DLPLAN_SRC_CORE_PARSER_EXPRESSIONS_CONCEPTS_AND_H_

#include "../concept.h"
#include "../../utils.h"

using namespace std::string_literals;


namespace dlplan::core::parser {

class AndConcept : public Concept {
private:
    static inline const std::string m_name = "c_and";

public:
    AndConcept(const std::string &name, std::vector<std::unique_ptr<Expression>> &&children)
    : Concept(name, sort_children_lexicographically(std::move(children))) { }

    std::shared_ptr<const dlplan::core::Concept> parse_concept(SyntacticElementFactory& factory) const override {
        if (m_children.size() != 2) {
            throw std::runtime_error("AndConcept::parse_concept - number of children ("s + std::to_string(m_children.size()) + " != 2).");
        }
        // 1. Parse children
        auto concept_left = m_children[0]->parse_concept(factory);
        auto concept_right = m_children[1]->parse_concept(factory);
        if (!(concept_left && concept_right)) {
            throw std::runtime_error("AndConcept::parse_concept - children are not of type Concept.");
        }
        // 2. Construct element
        return factory.make_and_concept(concept_left, concept_right);
    }

    static const std::string& get_name() {
        return m_name;
    }
};

}

#endif
