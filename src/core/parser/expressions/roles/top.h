#ifndef DLP_SRC_CORE_PARSER_EXPRESSIONS_ROLES_TOP_H_
#define DLP_SRC_CORE_PARSER_EXPRESSIONS_ROLES_TOP_H_

#include "../role.h"
#include "../../../elements/roles/top.h"


namespace dlp {
namespace core {
namespace parser {

class TopRole : public Role {
protected:
    virtual element::Role_Ptr parse_role_impl(const VocabularyInfo& vocabulary, ElementCache &) const override {
        if (m_children.size() != 0) {
            throw std::runtime_error("RestrictRole::parse_role_impl - number of children ("s + std::to_string(m_children.size()) + " != 0).");
        }
        // 2. Construct element
        return std::make_shared<element::TopRole>(vocabulary);
    }

public:
    TopRole(const std::string &name, std::vector<std::unique_ptr<Expression>> &&children)
    : Role(name, std::move(children)) { }
};

}
}
}

#endif
