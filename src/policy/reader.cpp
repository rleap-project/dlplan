#include "reader.h"

#include <sstream>

#include "parser/parser.h"
#include "parser/expressions.h"

#include "../include/dlplan/policy.h"


namespace dlplan::policy {

Policy PolicyReaderImpl::read(const std::string& data) const {
    return parser::Parser().parse(data)->parse_general_policy();
}

}
