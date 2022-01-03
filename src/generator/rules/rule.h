#ifndef DLPLAN_SRC_GENERATOR_FEATURE_GENERATOR_RULES_RULE_H_
#define DLPLAN_SRC_GENERATOR_FEATURE_GENERATOR_RULES_RULE_H_

#include <string>
#include <iostream>
#include <mutex>

#include "../types.h"

#include "../../utils/threadpool.h"


namespace dlplan::generator {
class GeneratorData;
namespace rules {

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

    mutable std::mutex m_mutex;

protected:
    virtual void generate_impl(const States& states, int iteration, GeneratorData& data, utils::threadpool::ThreadPool& th) = 0;

    virtual void increment_instantiations() {
        std::lock_guard<std::mutex> hold(m_mutex);
        ++m_count_instantiations;
    }

public:
    Rule(const std::string& name) : m_name(name), m_count_instantiations(0), m_enabled(true) { }
    virtual ~Rule() = default;

    void generate(const States& states, int iteration, GeneratorData& data, utils::threadpool::ThreadPool& th) {
        std::lock_guard<std::mutex> hold(m_mutex);
        if (m_enabled) {
            generate_impl(states, iteration, data, th);
        }
    }

    void initialize() {
        std::lock_guard<std::mutex> hold(m_mutex);
        m_count_instantiations = 0;
    }

    void print_statistics() const {
        std::lock_guard<std::mutex> hold(m_mutex);
        if (m_enabled) {
            std::cout << "    " << m_name << ": " << m_count_instantiations << std::endl;
        }
    }

    void set_enabled(bool enabled) {
        std::lock_guard<std::mutex> hold(m_mutex);
        m_enabled = enabled;
    }
};

}
}

#endif
