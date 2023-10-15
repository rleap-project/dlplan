#ifndef DLPLAN_SRC_CORE_PARSER_EXPRESSIONS_ROLES_TOP_H_
#define DLPLAN_SRC_CORE_PARSER_EXPRESSIONS_ROLES_TOP_H_

#include "../role.h"

using namespace std::string_literals;


namespace dlplan::core::parser {

class TopRole : public Role {
private:
    static inline const std::string m_name = "r_top";

public:
    TopRole(const std::string &name, std::vector<std::unique_ptr<Expression>> &&children)
    : Role(name, std::move(children)) { }

    std::shared_ptr<const dlplan::core::Role> parse_role(SyntacticElementFactory& factory) const override {
        if (m_children.size() != 0) {
            throw std::runtime_error("TopRole::parse_role - number of children ("s + std::to_string(m_children.size()) + " != 0).");
        }
        // 2. Construct element
        return factory.make_top_role();
    }

    static const std::string& get_name() {
        return m_name;
    }
};

}

#endif
