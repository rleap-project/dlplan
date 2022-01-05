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
    RuleStats m_stats;

protected:
    virtual void generate_impl(const States& states, int iteration, GeneratorData& data, utils::threadpool::ThreadPool& th, std::vector<utils::threadpool::ThreadPool::TaskFuture<void>>& tasks) = 0;

public:
    Rule(const std::string& name) : m_name(name), m_enabled(true) { }
    virtual ~Rule() = default;

    void generate(const States& states, int iteration, GeneratorData& data, utils::threadpool::ThreadPool& th, std::vector<utils::threadpool::ThreadPool::TaskFuture<void>>& tasks) {
        if (m_enabled) {
            generate_impl(states, iteration, data, th, tasks);
        }
    }

    void print_statistics() const {
        if (m_enabled) {
            std::cout << "    " << m_name << ": " << m_stats.get_count() << std::endl;
        }
    }

    void set_enabled(bool enabled) {
        m_enabled = enabled;
    }

    RuleStats& get_stats() {
        return m_stats;
    }
};

/**
 * Evaluates an element on a collection of states.
 */
template<typename D>
std::vector<D> evaluate(core::Element<D>& element, const States& states) {
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

/**
 * After constructing an element, compute hash and try to add it to the result.
 */
inline void add_concept(Rule& rule, int iteration, core::Concept&& result, const States& states, GeneratorData& data) {
    auto denotations = evaluate<core::ConceptDenotation>(result, states);
    auto flat = bitset_to_num_vec<core::ConceptDenotation>(denotations);
    if (data.m_concept_hash_table.insert(compute_hash(flat))) {
        data.m_result_data.add_repr(result.compute_repr());
        // TODO(dominik): the following crashes
        //std::cout << &data.m_concept_iteration_data[iteration+1]<< " " << data.m_concept_iteration_data[iteration+1].size() << std::endl;
        data.m_concept_iteration_data[iteration+1].push_back(std::move(result));
        rule.get_stats().increment();
    }
}

inline void add_role(Rule& rule, int iteration, core::Role&& result, const States& states, GeneratorData& data) {
    auto denotations = evaluate<core::RoleDenotation>(result, states);
    auto flat = bitset_to_num_vec<core::RoleDenotation>(denotations);
    if (data.m_role_hash_table.insert(compute_hash(flat))) {
        data.m_result_data.add_repr(result.compute_repr());
        data.m_role_iteration_data[iteration+1].push_back(std::move(result));
        rule.get_stats().increment();
    }
}

inline void add_boolean(Rule& rule, int iteration, core::Boolean&& result, const States& states, GeneratorData& data) {
    auto denotations = evaluate<bool>(result, states);
    auto flat = bool_vec_to_num_vec(denotations);
    if (data.m_boolean_and_numerical_hash_table.insert(compute_hash(flat))) {
        data.m_result_data.add_repr(result.compute_repr());
        data.m_boolean_iteration_data[iteration+1].push_back(std::move(result));
        rule.get_stats().increment();
    }
}

inline void add_numerical(Rule& rule, int iteration, core::Numerical&& result, const States& states, GeneratorData& data) {
    auto denotations = evaluate<int>(result, states);
    if (data.m_boolean_and_numerical_hash_table.insert(compute_hash(denotations))) {
        data.m_result_data.add_repr(result.compute_repr());
        data.m_numerical_iteration_data[iteration+1].push_back(std::move(result));
        rule.get_stats().increment();
    }
}

}
}
}

#endif
