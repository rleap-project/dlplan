#ifndef DLP_SRC_CORE_PARSER_EXPRESSIONS_CONCEPTS_ONE_OF_H_
#define DLP_SRC_CORE_PARSER_EXPRESSIONS_CONCEPTS_ONE_OF_H_

#include "../concept.h"
#include "../../../elements/concepts/one_of.h"
#include "../utils.h"

namespace dlp {
namespace core {
namespace parser {

class OneOfConcept : public Concept {
protected:
    virtual element::Concept_Ptr parse_concept_impl(const VocabularyInfoImpl& vocabulary, ElementCache &cache) const override {
        if (m_children.size() != 1) {
            throw std::runtime_error("OneOfConcept::parse_concept_impl - number of children ("s + std::to_string(m_children.size()) + " != 1).");
        }
        // 1. Parse children
        unsigned object_idx = try_parse_number(m_children[0]->get_name());
        // 2. Construct element
        return std::make_shared<element::OneOfConcept>(vocabulary, object_idx);
    }

public:
    OneOfConcept(const std::string &name, std::vector<std::unique_ptr<Expression>> &&children)
    : Concept(name, std::move(children)) { }
};

}
}
}

#endif
