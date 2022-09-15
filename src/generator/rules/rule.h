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
     * Whether this rule is enabled.
     */
    bool m_enabled;

    /**
     * Whether this rule is executed in lookahead mode.
     */
    bool m_lookahead;

    /**
     * Collect some statistics.
     */
    int m_count;

protected:
    virtual void submit_tasks_impl(const States& states, int target_complexity, GeneratorData& data, utils::threadpool::ThreadPool& th) = 0;

    virtual void parse_results_of_tasks_impl(GeneratorData& data) = 0;

public:
    Rule() : m_enabled(true), m_lookahead(false), m_count(0) { }
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
    void submit_tasks(const States& states, int target_complexity, GeneratorData& data, utils::threadpool::ThreadPool& th) {
        if (m_enabled) {
            submit_tasks_impl(states, target_complexity, data, th);
        }
    }

    /**
     * Parses the result of each task.
     */
    void parse_results_of_tasks(GeneratorData& data) {
        if (m_enabled) {
            return parse_results_of_tasks_impl(data);
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

    void set_lookahead(bool lookahead) {
        m_lookahead = lookahead;
    }

    virtual std::string get_name() const = 0;
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
        const auto data = concept.evaluate(state).to_canonical_data_representation();
        result.push_back(data.size());
        result.insert(result.end(), data.begin(), data.end());
    }
    return result;
}

inline std::vector<int> evaluate_role(const core::Role& role, const States& states) {
    std::vector<int> result;
    for (const auto& state : states) {
        const auto data = role.evaluate(state).to_canonical_data_representation();
        result.push_back(data.size());
        result.insert(result.end(), data.begin(), data.end());
    }
    return result;
}

}
}
}

#endif
