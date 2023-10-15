#ifndef SRC_PARSERS_EXTENDED_SKETCH_COMMON_ERROR_HANDLER_HPP_
#define SRC_PARSERS_EXTENDED_SKETCH_COMMON_ERROR_HANDLER_HPP_

#include <map>

#include "src/core/parsers/common/config.hpp"


namespace dlplan::core::parsers::elements
{
    namespace x3 = boost::spirit::x3;

    ////////////////////////////////////////////////////////////////////////////
    //  Our error handler
    ////////////////////////////////////////////////////////////////////////////

    struct error_handler_base
    {
        error_handler_base();

        template <typename Iterator, typename Exception, typename Context>
        x3::error_handler_result on_error(
            Iterator& first, Iterator const& last
          , Exception const& x, Context const& context);

        std::map<std::string, std::string> id_map;
    };

    ////////////////////////////////////////////////////////////////////////////
    // Implementation
    ////////////////////////////////////////////////////////////////////////////

    inline error_handler_base::error_handler_base()
    {
        id_map["name"] = "name";
        id_map["quoted_string"] = "quoted_string";
        id_map["name_entry"] = "name_entry";
        id_map["memory_state_definition"] = "memory_state_definition";
        id_map["memory_state_reference"] = "memory_state_reference";
        id_map["memory_states_entry"] = "memory_states_entry";
        id_map["initial_memory_state_entry"] = "initial_memory_state_entry";
        id_map["register_definition"] = "register_definition";
        id_map["register_reference"] = "register_reference";
        id_map["registers_entry"] = "registers_entry";
        id_map["boolean_definition"] = "boolean_definition";
        id_map["boolean_reference"] = "boolean_reference";
        id_map["booleans_entry"] = "booleans_entry";
        id_map["numerical_definition"] = "numerical_definition";
        id_map["numerical_reference"] = "numerical_reference";
        id_map["numericals_entry"] = "numericals_entry";
        id_map["concept_definition"] = "concept_definition";
        id_map["concept_reference"] = "concept_reference";
        id_map["concepts_entry"] = "concepts_entry";
        id_map["memory_condition_entry"] = "memory_condition_entry";
        id_map["memory_effect_entry"] = "memory_effect_entry";
        id_map["positive_boolean_condition_entry"] = "positive_boolean_condition_entry";
        id_map["negative_boolean_condition_entry"] = "negative_boolean_condition_entry";
        id_map["greater_numerical_condition_entry"] = "greater_numerical_condition_entry";
        id_map["equal_numerical_condition_entry"] = "equal_numerical_condition_entry";
        id_map["positive_boolean_effect_entry"] = "positive_boolean_effect_entry";
        id_map["negative_boolean_effect_entry"] = "negative_boolean_effect_entry";
        id_map["unchanged_boolean_effect_entry"] = "unchanged_boolean_effect_entry";
        id_map["increment_numerical_effect_entry"] = "increment_numerical_effect_entry";
        id_map["decrement_numerical_effect_entry"] = "decrement_numerical_effect_entry";
        id_map["unchanged_numerical_effect_entry"] = "unchanged_numerical_effect_entry";
        id_map["feature_condition_entry"] = "feature_condition_entry";
        id_map["feature_effect_entry"] = "feature_effect_entry";
        id_map["load_rule_entry"] = "load_rule_entry";
        id_map["module_reference"] = "module_reference";
        id_map["call_rule_entry"] = "call_rule_entry";
        id_map["action_reference"] = "action_reference";
        id_map["action_rule_entry"] = "action_rule_entry";
        id_map["search_rule_entry"] = "search_rule_entry";
        id_map["rule_entry"] = "rule_entry";
        id_map["rules"] = "rules";
        id_map["extended_sketch"] = "extended_sketch";
    }

    template <typename Iterator, typename Exception, typename Context>
    inline x3::error_handler_result
    error_handler_base::on_error(
        Iterator&, Iterator const&
      , Exception const& x, Context const& context)
    {
        std::string which = x.which();
        auto iter = id_map.find(which);
        if (iter != id_map.end())
            which = iter->second;

        std::string message = "Error! Expecting: " + which + " here:";
        auto& error_handler = x3::get<error_handler_tag>(context).get();
        error_handler(x.where(), message);
        return x3::error_handler_result::fail;
    }
}

#endif
