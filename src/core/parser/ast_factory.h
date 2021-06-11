#ifndef DLP_SRC_CORE_PARSER_AST_FACTORY_H_
#define DLP_SRC_CORE_PARSER_AST_FACTORY_H_

#include <vector>

#include "types.h"


namespace dlp::core {
class VocabularyInfo;
namespace parser {

class AST_Factory {
public:
    Expression_Ptr make_ast(const VocabularyInfo& vocabulary_info, const std::string &name, std::vector<Expression_Ptr> &&children);
};

}
}

#endif
