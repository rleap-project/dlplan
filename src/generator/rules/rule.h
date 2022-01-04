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

class RuleStats {
private:
    mutable std::mutex m_mutex;

    int m_count;

public:
    RuleStats() : m_count(0) { }

    void initialize() {
        std::lock_guard<std::mutex> hold(m_mutex);
        m_count = 0;
    }

    void increment() {
        std::lock_guard<std::mutex> hold(m_mutex);
        ++m_count;
    }

    int get_count() const {
        std::lock_guard<std::mutex> hold(m_mutex);
        return m_count;
    }
};


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

    /**
     * Collect some statistics.
     */
    RuleStats m_stats;

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
            parse_results_of_tasks_impl(iteration, data);
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
 * Evaluates an element on a collection of states.
 */
template<typename D>
std::vector<D> evaluate(const core::Element<D>& element, const States& states) {
    std::vector<D> result;
    result.reserve(states.size());
    for (const auto& state : states) {
        result.push_back(element.evaluate(state));
    }
    result.shrink_to_fit();
    return result;
}

/**
 * Transform vector<bool> to vector<int> for hashing
 */
inline std::vector<int> bool_vec_to_num_vec(const std::vector<bool>& bool_vec) {
    std::vector<int> num_vec;
    num_vec.reserve(bool_vec.size());
    for (size_t i = 0; i < bool_vec.size(); ++i) {
        num_vec.push_back(bool_vec[i]);
    }
    return num_vec;
}

/**
 * Transform vector<bitset> to vector<int> for hashing
 */
template<typename T>
inline std::vector<int> bitset_to_num_vec(const std::vector<T>& denotation) {
    static_assert(sizeof(int) == sizeof(unsigned));
    size_t size = 0;
    for (const auto& b : denotation) {
        size += b.get_const_data().get_blocks().size();
    }
    std::vector<int> result;
    result.reserve(size);
    for (const auto& b : denotation) {
        result.insert(result.end(), b.get_const_data().get_blocks().begin(), b.get_const_data().get_blocks().end());
    }
    return result;
}


}
}
}

#endif
