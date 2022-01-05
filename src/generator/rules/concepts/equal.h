#ifndef DLPLAN_SRC_GENERATOR_RULES_CONCEPTS_EQUAL_H_
#define DLPLAN_SRC_GENERATOR_RULES_CONCEPTS_EQUAL_H_

#include "../rule.h"

namespace dlplan::generator::rules {

class EqualConcept : public Rule {
private:
    std::vector<std::vector<core::Role>> m_roles_by_iteration;

public:
    EqualConcept() : Rule("c_equal") { }

    virtual void generate_impl(const States& states, int iteration, GeneratorData& data, utils::threadpool::ThreadPool& th, std::vector<utils::threadpool::ThreadPool::TaskFuture<void>>& tasks) override {
        // Copy missing data.
        for (int i = static_cast<int>(m_roles_by_iteration.size()); i < iteration; ++i) {
            m_roles_by_iteration.push_back(data.m_role_iteration_data[i].get_elements());
        }
        // Use 1 thread for the rule.
        if (iteration == 2) {
            tasks.push_back(th.submit([&](){
                for (int i = 1; i < iteration; ++i) {
                    int j = iteration - i;
                    for (const auto& r1 : m_roles_by_iteration[i]) {
                        for (const auto& r2 : m_roles_by_iteration[j]) {
                            auto result = data.m_factory->make_equal_concept(r1, r2);
                            add_concept(*this, iteration, std::move(result), states, data);
                        }
                    }
                }
            }));
        }
    }
};

}

#endif
