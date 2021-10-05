#ifndef DLPLAN_SRC_GENERATOR_FEATURE_GENERATOR_RULES_RULE_H_
#define DLPLAN_SRC_GENERATOR_FEATURE_GENERATOR_RULES_RULE_H_

#include <string>
#include <iostream>

#include "../types.h"

#include "../feature_generator_data.h"


namespace dlplan::generator::rules {

class Rule {
protected:
    /**
     * The name is used for pretty printing and should reflect the concrete element.
     */
    const std::string m_name;

    /**
     * Whether the construction of this specific element is enabled or not.
     */
    const bool m_enabled;

    /**
     * Count the number of instantiations of this specific element.
     */
    int m_count_instantiations = 0;

protected:
    virtual void generate_impl(const States&, int, FeatureGeneratorData&) {
        throw std::runtime_error("Rule::generate_impl: error calling function of abstract base class");
    }

public:
    Rule(const std::string& name, bool enabled) : m_name(name), m_enabled(enabled) { }

    virtual void generate(const States& states, int iteration, FeatureGeneratorData& data) { 
        if (m_enabled) 
            generate_impl(states, iteration, data);
        
    }

    void print_statistics() const {
        if (m_enabled)
            std::cout << "    " << m_name << ": " << m_count_instantiations << std::endl;
    }
};

}

#endif
