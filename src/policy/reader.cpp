#include "reader.h"

#include <sstream>

#include "parser/parser.h"
#include "parser/expressions.h"

#include "../../include/dlplan/policy.h"


namespace dlplan::policy {

Policy PolicyReaderImpl::read(const std::string& data, core::SyntacticElementFactory& factory) const {
    return parser::Parser().parse(data)->parse_general_policy(factory);
}

}
