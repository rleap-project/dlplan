#ifndef DLPLAN_SRC_CORE_PARSER_EXPRESSIONS_CONCEPTS_PROJECTION_H_
#define DLPLAN_SRC_CORE_PARSER_EXPRESSIONS_CONCEPTS_PROJECTION_H_

#include "../concept.h"
#include "../../utils.h"

using namespace std::string_literals;


namespace dlplan::core::parser {

class ProjectionConcept : public Concept {
private:
    static inline const std::string m_name = "c_projection";

public:
    ProjectionConcept(const std::string &name, std::vector<std::unique_ptr<Expression>> &&children)
    : Concept(name, std::move(children)) { }

    std::shared_ptr<const dlplan::core::Concept> parse_concept(SyntacticElementFactory& factory) const override {
        if (m_children.size() != 2) {
            throw std::runtime_error("ProjectionConcept::parse_concept - number of children ("s + std::to_string(m_children.size()) + " != 1).");
        }
        auto role = m_children[0]->parse_role(factory);
        if (!role) {
            throw std::runtime_error("ProjectionConcept::parse_concept - child is a nullptr.");
        }
        int pos = try_parse_number(m_children[1]->get_name());
        if (pos < 0 || pos > 1) {
            throw std::runtime_error("ProjectionConcept::parse_concept - projection index out of range, should be 0 or 1 ("s + std::to_string(pos) + ")");
        }
        return factory.make_projection_concept(role, pos);
    }

    static const std::string& get_name() {
        return m_name;
    }
};

}

#endif
