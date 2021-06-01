#ifndef DLP_SRC_CORE_PARSER_AST_FACTORY_H_
#define DLP_SRC_CORE_PARSER_AST_FACTORY_H_

#include <memory>
#include <unordered_map>
#include <vector>

#include "expressions/expression.h"
#include "types.h"

namespace dlp {
namespace core {
namespace parser {


/**
 *
 */
class AST_Factory {
public:
    Expression_Ptr make_ast(const VocabularyInfoImpl& vocabulary_info, const std::string &name, std::vector<Expression_Ptr> &&children);
};

}
}
}

#endif
