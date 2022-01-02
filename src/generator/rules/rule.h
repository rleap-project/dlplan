#ifndef DLPLAN_SRC_GENERATOR_FEATURE_GENERATOR_RULES_RULE_H_
#define DLPLAN_SRC_GENERATOR_FEATURE_GENERATOR_RULES_RULE_H_

#include <string>
#include <iostream>

#include "../types.h"
#include "../feature_generator_data.h"

#include "../../utils/threadpool.h"


namespace dlplan::generator::rules {

class Rule {
protected:
    /**
     * The name is used for pretty printing and should reflect the concrete element.
     */
    std::string m_name;

    /**
     * Count the number of instantiations of this specific element.
     */
    int m_count_instantiations = 0;

    /**
     * Whether this rule is enabled.
     */
    bool m_enabled;

protected:
    virtual void generate_impl(const States& states, int iteration, FeatureGeneratorData& data, utils::threadpool::ThreadPool& th) = 0;

public:
    Rule(const std::string& name) : m_name(name), m_count_instantiations(0), m_enabled(true) { }
    virtual ~Rule() = default;

    void generate(const States& states, int iteration, FeatureGeneratorData& data, utils::threadpool::ThreadPool& th) {
        if (m_enabled) {
            generate_impl(states, iteration, data, th);
        }
    }

    void initialize() {
        m_count_instantiations = 0;
    }

    void print_statistics() const {
        if (m_enabled) {
            std::cout << "    " << m_name << ": " << m_count_instantiations << std::endl;
        }
    }

    void set_enabled(bool enabled) {
        m_enabled = enabled;
    }
};

}

#endif
