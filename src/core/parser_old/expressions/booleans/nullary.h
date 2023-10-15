#ifndef DLPLAN_SRC_CORE_PARSER_EXPRESSIONS_BOOLEANS_NULLARY_H_
#define DLPLAN_SRC_CORE_PARSER_EXPRESSIONS_BOOLEANS_NULLARY_H_

#include "../boolean.h"


using namespace std::string_literals;


namespace dlplan::core::parser {

class NullaryBoolean : public Boolean {
private:
    static inline const std::string m_name = "b_nullary";

public:
    NullaryBoolean(const std::string &name, std::vector<std::unique_ptr<Expression>> &&children)
    : Boolean(name, std::move(children)) { }

    std::shared_ptr<const dlplan::core::Boolean> parse_boolean(SyntacticElementFactory& factory) const override {
        if (m_children.size() != 1) {
            throw std::runtime_error("NullaryBoolean::parse_boolean - number of children ("s + std::to_string(m_children.size()) + " != 1).");
        }
        // 1. Parse children
        const auto& predicate_name = m_children[0]->get_name();
        // 2. Construct element
        return factory.make_nullary_boolean(factory.get_vocabulary_info()->get_predicate(predicate_name));
    }

    static const std::string& get_name() {
        return m_name;
    }
};

}

#endif
