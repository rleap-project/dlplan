#include "reader.h"

#include "parser/parser.h"
#include "parser/expressions.h"

#include "../../include/dlplan/policy.h"


namespace dlplan::policy {

std::shared_ptr<const Policy> PolicyReaderImpl::read(const std::string& data, PolicyBuilder& builder, core::SyntacticElementFactory& factory) const {
    return parser::Parser().parse(data)->parse_policy(builder, factory);
}

}
