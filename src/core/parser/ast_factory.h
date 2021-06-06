#ifndef DLP_SRC_CORE_PARSER_AST_FACTORY_H_
#define DLP_SRC_CORE_PARSER_AST_FACTORY_H_

#include <vector>

#include "types.h"


namespace dlp {
namespace core {
class VocabularyInfoImpl;
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
