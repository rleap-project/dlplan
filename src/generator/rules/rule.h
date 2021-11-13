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
     * Count the number of instantiations of this specific element.
     */
    int m_count_instantiations = 0;

public:
    Rule(const std::string& name) : m_name(name) { }

    virtual void generate(const States&, int, FeatureGeneratorData&) {
        throw std::runtime_error("Rule::generate: error calling function of abstract base class");
    }

    void initialize() {
        m_count_instantiations = 0;
    }

    void print_statistics() const {
        std::cout << "    " << m_name << ": " << m_count_instantiations << std::endl;
    }
};

}

#endif
