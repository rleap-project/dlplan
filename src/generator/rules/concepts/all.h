#ifndef DLPLAN_SRC_GENERATOR_RULES_CONCEPTS_ALL_H_
#define DLPLAN_SRC_GENERATOR_RULES_CONCEPTS_ALL_H_

#include "../rule.h"


namespace dlplan::generator::rules {

class AllConcept : public Rule {
private:
    std::vector<std::vector<core::Concept>> m_concepts_by_iteration;
    std::vector<std::vector<core::Role>> m_roles_by_iteration;

public:
    AllConcept() : Rule("c_all") { }

    virtual void generate_impl(const States& states, int iteration, GeneratorData& data, utils::threadpool::ThreadPool& th) override {
        // Copy missing data.
        for (int i = static_cast<int>(m_concepts_by_iteration.size()); i < iteration; ++i) {
            m_concepts_by_iteration.push_back(data.m_concept_iteration_data[i].get_elements());
        }
        for (int i = static_cast<int>(m_roles_by_iteration.size()); i < iteration; ++i) {
            m_roles_by_iteration.push_back(data.m_role_iteration_data[i].get_elements());
        }
        /* Submit a task for every element. */
        std::vector<utils::threadpool::ThreadPool::TaskFuture<std::pair<core::Concept,std::array<u_int32_t, 4>>>> tasks;
        for (int i = 1; i < iteration; ++i) {
            int j = iteration - i;
            for (const auto& r : m_roles_by_iteration[i]) {
                for (const auto& c : m_concepts_by_iteration[j]) {
                    tasks.push_back(
                        th.submit([](const States& states, const core::Role& r, const core::Concept& c, core::SyntacticElementFactory& factory) {
                                auto element = factory.make_all_concept(r, c);
                                auto denotation = evaluate<core::ConceptDenotation>(element, states);
                                auto hash = compute_hash(bitset_to_num_vec(denotation));
                                return std::make_pair(std::move(element),std::move(hash));
                            },
                            std::cref(states),
                            std::cref(r),
                            std::cref(c),
                            std::ref(*data.m_factory))
                    );
                }
            }
        }
        /* Wait for the result and add it. */
        for (auto& task : tasks) {
            auto result = task.get();
            if (data.m_concept_hash_table.insert(std::move(result.second))) {
                data.m_concept_iteration_data[iteration].push_back(std::move(result.first));
            }
        }
    }
};

}

#endif
