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

    void increment_instantiations() {
        std::lock_guard<std::mutex> hold(m_mutex);
        ++m_count_instantiations;
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
std::vector<int> bool_vec_to_num_vec(const std::vector<bool>& bool_vec) {
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
std::vector<int> bitset_to_num_vec(const std::vector<T>& denotation) {
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
 * After constructing a concept, compute hash and try to add it to the result.
 */
void add_concept(Rule& rule, int iteration, core::Concept&& result, const States& states, GeneratorData& data) {
    auto denotations = evaluate<core::ConceptDenotation>(result, states);
    auto flat = bitset_to_num_vec<core::ConceptDenotation>(denotations);
    if (data.m_concept_hash_table.insert(compute_hash(flat))) {
        data.m_concept_iteration_data[iteration+1].push_back(std::move(result));
        rule.increment_instantiations();
    }
}

}
}
}

#endif
