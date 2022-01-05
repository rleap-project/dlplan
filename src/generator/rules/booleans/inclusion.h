#ifndef DLPLAN_SRC_GENERATOR_RULES_BOOLEANS_INCLUSION_H_
#define DLPLAN_SRC_GENERATOR_RULES_BOOLEANS_INCLUSION_H_

#include "../rule.h"


namespace dlplan::generator::rules {

class InclusionBoolean : public Rule {
private:
    std::vector<std::vector<core::Concept>> m_concepts_by_iteration;
    std::vector<std::vector<core::Role>> m_roles_by_iteration;

public:
    InclusionBoolean() : Rule("b_inclusion") { }

    virtual void generate_impl(const States& states, int iteration, GeneratorData& data, utils::threadpool::ThreadPool& th, std::vector<utils::threadpool::ThreadPool::TaskFuture<void>>& tasks) override {
        // Copy missing data.
        for (int i = static_cast<int>(m_concepts_by_iteration.size()); i < iteration; ++i) {
            m_concepts_by_iteration.push_back(data.m_concept_iteration_data[i].get_elements());
        }
        for (int i = static_cast<int>(m_roles_by_iteration.size()); i < iteration; ++i) {
            m_roles_by_iteration.push_back(data.m_role_iteration_data[i].get_elements());
        }
        // Use 1 thread for the rule.
        tasks.push_back(th.submit([&](){
            for (int i = 1; i < iteration; ++i) {
                int j = iteration - i;
                for (const auto& c1 : m_concepts_by_iteration[i]) {
                    for (const auto& c2 : m_concepts_by_iteration[j]) {
                        auto result = data.m_factory->make_concept_inclusion_boolean(c1, c2);
                        add_boolean(*this, iteration, std::move(result), states, data);
                    }
                }
            }
        }));
        tasks.push_back(th.submit([&](){
            for (int i = 1; i < iteration; ++i) {
                int j = iteration - i;
                for (const auto& r1 : m_roles_by_iteration[i]) {
                    for (const auto& r2 : m_roles_by_iteration[j]) {
                        auto result = data.m_factory->make_role_inclusion_boolean(r1, r2);
                        add_boolean(*this, iteration, std::move(result), states, data);
                    }
                }
            }
        }));
    }
};

}

#endif
