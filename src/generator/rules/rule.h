#ifndef DLPLAN_SRC_GENERATOR_FEATURE_GENERATOR_RULES_RULE_H_
#define DLPLAN_SRC_GENERATOR_FEATURE_GENERATOR_RULES_RULE_H_

#include "../../../include/dlplan/core.h"

#include <string>
#include <iostream>


namespace dlplan::generator {
class GeneratorData;
namespace rules {

class Rule {
protected:
    /**
     * Whether this rule is enabled.
     */
    bool m_enabled;

    /**
     * Collect some statistics.
     */
    int m_count;

protected:
    virtual void generate_impl(const core::States& states, int target_complexity, GeneratorData& data, core::DenotationsCaches& caches) = 0;

public:
    Rule() : m_enabled(true), m_count(0) { }
    virtual ~Rule() = default;

    void initialize() {
        m_count = 0;
    }

    /**
     * Submits tasks to threadpool.
     */
    void generate(const core::States& states, int target_complexity, GeneratorData& data, core::DenotationsCaches& caches) {
        if (m_enabled) {
            generate_impl(states, target_complexity, data, caches);
        }
    }

    void print_statistics() const {
        if (m_enabled) {
            std::cout << "    " << get_name() << ": " << m_count << std::endl;
        }
    }

    void set_enabled(bool enabled) {
        m_enabled = enabled;
    }

    virtual std::string get_name() const = 0;

    void increment_generated() {
        ++m_count;
    }
};

}
}

#endif
