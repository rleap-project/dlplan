#include "driver.hpp"

#include <cassert>
#include <fstream>

#include "common/error_handler.hpp"
#include "stage_1_ast/parser.hpp"
//#include "stage_2_sketch/parser.hpp"


namespace dlplan::core::parsers::elements {

Driver::Driver(
    dlplan::core::SyntacticElementFactory& element_factory)
    : element_factory(element_factory) { }

std::shared_ptr<const Boolean> Driver::parse_boolean(
    const std::string& source,
    const std::string& filename) {

    iterator_type iter(source.begin());
    iterator_type const end(source.end());

    return parse_boolean(iter, end, filename);
}

std::shared_ptr<const Boolean> Driver::parse_boolean(
    iterator_type& iter,
    iterator_type end,
    const std::string& filename) {

    // Our error handler
    error_handler_type error_handler(iter, end, std::cerr, filename);

    // Stage 1 parse
    auto root_node = stage_1::parser::parse_ast(iter, end, error_handler);

    // Stage 2 parse
    //stage_2::Context context(domain_description, element_factory, policy_builder);
    //auto sketch = stage_2::parser::parse_sketch(context, error_handler, root_node);

    return nullptr;  // TODO
}

}
