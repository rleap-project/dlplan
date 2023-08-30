#ifndef DLPLAN_SRC_CORE_PARSER_EXPRESSIONS_CONCEPTS_NOT_H_
#define DLPLAN_SRC_CORE_PARSER_EXPRESSIONS_CONCEPTS_NOT_H_

#include "../concept.h"

using namespace std::string_literals;


namespace dlplan::core::parser {

class NotConcept : public Concept {
private:
    static inline const std::string m_name = "c_not";

public:
    NotConcept(const std::string &name, std::vector<std::unique_ptr<Expression>> &&children)
    : Concept(name, std::move(children)) { }

    std::shared_ptr<const dlplan::core::Concept> parse_concept(SyntacticElementFactory& factory) const override {
        if (m_children.size() != 1) {
            throw std::runtime_error("NotConcept::parse_concept - number of children ("s + std::to_string(m_children.size()) + " != 1).");
        }
        // 1. Parse children
        auto concept = m_children[0]->parse_concept(factory);
        if (!concept) {
            throw std::runtime_error("NotConcept::parse_concept - child is not of type Concept.");
        }
        // 2. Construct element
        return factory.make_not_concept(concept);
    }

    static const std::string& get_name() {
        return m_name;
    }
};

}

#endif
