#ifndef DLPLAN_SRC_GENERATOR_FEATURE_GENERATOR_RULES_RULE_H_
#define DLPLAN_SRC_GENERATOR_FEATURE_GENERATOR_RULES_RULE_H_

#include <string>
#include <iostream>
#include <mutex>

#include "../generator_data.h"
#include "../types.h"

#include "../../utils/threadpool.h"


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
     * The name is used for pretty printing and should reflect the concrete element.
     */
    std::string m_name;

    /**
     * Whether this rule is enabled.
     */
    bool m_enabled;

    /**
     * Collect some statistics.
     */
    int m_count;

protected:
    virtual void submit_tasks_impl(const States& states, int iteration, GeneratorData& data, utils::threadpool::ThreadPool& th) = 0;

    virtual void parse_results_of_tasks_impl(int iteration, GeneratorData& data) = 0;

public:
    Rule(const std::string& name) : m_name(name), m_enabled(true), m_count(0) { }
    virtual ~Rule() = default;

    void initialize() {
        m_count = 0;
    }

    /**
     * Destruct the threadpool tasks.
     */
    virtual void cleanup() = 0;

    /**
     * Submits tasks to threadpool.
     */
    void submit_tasks(const States& states, int iteration, GeneratorData& data, utils::threadpool::ThreadPool& th) {
        if (m_enabled) {
            submit_tasks_impl(states, iteration, data, th);
        }
    }

    /**
     * Parses the result of each task.
     */
    void parse_results_of_tasks(int iteration, GeneratorData& data) {
        if (m_enabled) {
            return parse_results_of_tasks_impl(iteration, data);
        }
    }

    void print_statistics() const {
        if (m_enabled) {
            std::cout << "    " << m_name << ": " << m_count << std::endl;
        }
    }

    void set_enabled(bool enabled) {
        m_enabled = enabled;
    }
};

/**
 * Evaluate each element on set of states.
 * The result is hashable data.
 */
inline std::vector<int> evaluate_boolean(const core::Boolean& boolean, const States& states) {
    std::vector<int> result;
    result.reserve(states.size());
    for (const auto& state : states) {
        result.push_back(static_cast<int>(boolean.evaluate(state)));
    }
    return result;
}

inline std::vector<int> evaluate_numerical(const core::Numerical& numerical, const States& states) {
    std::vector<int> result;
    result.reserve(states.size());
    for (const auto& state : states) {
        result.push_back(numerical.evaluate(state));
    }
    return result;
}

inline std::vector<int> evaluate_concept(const core::Concept& concept, const States& states) {
    std::vector<int> result;
    result.reserve(states.size());
    for (const auto& state : states) {
        const auto concept_denot = concept.evaluate(state).to_sorted_vector();
        result.push_back(concept_denot.size());
        result.insert(result.end(), concept_denot.begin(), concept_denot.end());
    }
    return result;
}

inline std::vector<int> evaluate_role(const core::Role& role, const States& states) {
    std::vector<int> result;
    for (const auto& state : states) {
        const auto role_denot = role.evaluate(state).to_sorted_vector();
        result.push_back(role_denot.size());
        for (const auto& pair : role_denot) {
            result.push_back(pair.first);
            result.push_back(pair.second);
        }
    }
    return result;
}

}
}
}

#endif
