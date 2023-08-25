#ifndef DLPLAN_SRC_CORE_PARSER_EXPRESSIONS_CONCEPTS_TOP_H_
#define DLPLAN_SRC_CORE_PARSER_EXPRESSIONS_CONCEPTS_TOP_H_

#include "../concept.h"

using namespace std::string_literals;


namespace dlplan::core::parser {

class TopConcept : public Concept {
public:
    TopConcept(const std::string &name, std::vector<std::unique_ptr<Expression>> &&children)
    : Concept(name, std::move(children)) { }

    std::shared_ptr<const dlplan::core::Concept> parse_concept(SyntacticElementFactory& factory) const override {
        if (m_children.size() != 0) {
            throw std::runtime_error("TopConcept::parse_concept - number of children ("s + std::to_string(m_children.size()) + " != 0).");
        }
        // 2. Construct element
        return factory.make_top_concept();
    }
};

}

#endif
