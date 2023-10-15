#ifndef DLPLAN_SRC_CORE_PARSER_EXPRESSIONS_CONCEPTS_ONE_OF_H_
#define DLPLAN_SRC_CORE_PARSER_EXPRESSIONS_CONCEPTS_ONE_OF_H_

#include "../concept.h"
#include "../../utils.h"

using namespace std::string_literals;


namespace dlplan::core::parser {

class OneOfConcept : public Concept {
private:
    static inline const std::string m_name = "c_one_of";

public:
    OneOfConcept(const std::string &name, std::vector<std::unique_ptr<Expression>> &&children)
    : Concept(name, std::move(children)) { }

    std::shared_ptr<const dlplan::core::Concept> parse_concept(SyntacticElementFactory& factory) const override {
        if (m_children.size() != 1) {
            throw std::runtime_error("OneOfConcept::parse_concept - number of children ("s + std::to_string(m_children.size()) + " != 1).");
        }
        const auto& constant_name = m_children[0]->get_name();
        return factory.make_one_of_concept(factory.get_vocabulary_info()->get_constant(constant_name));
    }

    static const std::string& get_name() {
        return m_name;
    }
};

}

#endif
