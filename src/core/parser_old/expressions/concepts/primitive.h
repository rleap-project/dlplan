#ifndef DLPLAN_SRC_CORE_PARSER_EXPRESSIONS_CONCEPTS_PRIMITIVE_H_
#define DLPLAN_SRC_CORE_PARSER_EXPRESSIONS_CONCEPTS_PRIMITIVE_H_

#include "../concept.h"
#include "../../utils.h"

using namespace std::string_literals;


namespace dlplan::core::parser {

class PrimitiveConcept : public Concept {
private:
    static inline const std::string m_name = "c_primitive";

public:
    PrimitiveConcept(const std::string &name, std::vector<std::unique_ptr<Expression>> &&children)
    : Concept(name, std::move(children)) { }

    std::shared_ptr<const dlplan::core::Concept> parse_concept(SyntacticElementFactory& factory) const override {
        if (m_children.size() != 2) {
            throw std::runtime_error("PrimitiveConcept::parse_concept - number of children ("s + std::to_string(m_children.size()) + " != 2).");
        }
        // 1. Parse children
        const auto& predicate_name = m_children[0]->get_name();
        int pos = try_parse_number(m_children[1]->get_name());
        // 2. Construct element
        return factory.make_primitive_concept(factory.get_vocabulary_info()->get_predicate(predicate_name), pos);
    }

    static const std::string& get_name() {
        return m_name;
    }
};

}

#endif
