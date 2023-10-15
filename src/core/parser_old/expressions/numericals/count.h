#ifndef DLPLAN_SRC_CORE_PARSER_EXPRESSIONS_NUMERICAL_COUNT_H_
#define DLPLAN_SRC_CORE_PARSER_EXPRESSIONS_NUMERICAL_COUNT_H_

#include "../numerical.h"

using namespace std::string_literals;


namespace dlplan::core::parser {

class CountNumerical : public Numerical {
private:
    static inline const std::string m_name = "n_count";

public:
    CountNumerical(const std::string &name, std::vector<std::unique_ptr<Expression>> &&children)
    : Numerical(name, std::move(children)) { }

    std::shared_ptr<const dlplan::core::Numerical> parse_numerical(SyntacticElementFactory& factory) const override {
        if (m_children.size() != 1) {
            throw std::runtime_error("CountNumerical::parse_numerical - number of children ("s + std::to_string(m_children.size()) + " != 1).");
        }
        // 1. Parse children
        auto concept = m_children[0]->parse_concept(factory);
        if (concept) {
            return factory.make_count_numerical(concept);
        }
        auto role = m_children[0]->parse_role(factory);
        if (role) {
            return factory.make_count_numerical(role);
        }
        // 2. Construct element
        throw std::runtime_error("CountNumerical::parse_numerical - unable to construct children elements.");
    }

    static const std::string& get_name() {
        return m_name;
    }
};

}

#endif
