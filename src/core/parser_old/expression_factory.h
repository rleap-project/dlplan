#ifndef DLPLAN_SRC_CORE_PARSER_EXPRESSIONS_FACTORY_H_
#define DLPLAN_SRC_CORE_PARSER_EXPRESSIONS_FACTORY_H_

#include <vector>
#include <memory>


namespace dlplan::core {
class VocabularyInfo;
namespace parser {
class Expression;

class ExpressionFactory {
public:
    std::unique_ptr<Expression> make_expression(
        const std::string& name,
        std::vector<std::unique_ptr<Expression>> &&children);
};

}
}

#endif
