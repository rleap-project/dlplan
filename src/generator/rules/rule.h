#ifndef DLPLAN_SRC_GENERATOR_FEATURE_GENERATOR_RULES_RULE_H_
#define DLPLAN_SRC_GENERATOR_FEATURE_GENERATOR_RULES_RULE_H_

#include <string>
#include <iostream>
#include <mutex>
#include <cassert>

#include "../generator_data.h"
#include "../types.h"

#include "../../core/elements/element.h"
#include "../../core/elements/boolean.h"
#include "../../core/elements/numerical.h"
#include "../../core/elements/concept.h"
#include "../../core/elements/role.h"


namespace dlplan {
namespace core {
    class Concept;
    class Role;
    class Numerical;
    class Boolean;
    class SyntacticElementFactory;
}
namespace generator {
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
    virtual void generate_impl(const States& states, int target_complexity, GeneratorData& data, core::DenotationsCaches& caches) = 0;

public:
    Rule() : m_enabled(true), m_count(0) { }
    virtual ~Rule() = default;

    void initialize() {
        m_count = 0;
    }

    /**
     * Submits tasks to threadpool.
     */
    void generate(const States& states, int target_complexity, GeneratorData& data, core::DenotationsCaches& caches) {
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
}

#endif
