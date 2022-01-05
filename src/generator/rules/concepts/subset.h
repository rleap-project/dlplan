#ifndef DLPLAN_SRC_GENERATOR_RULES_CONCEPTS_SUBSET_H_
#define DLPLAN_SRC_GENERATOR_RULES_CONCEPTS_SUBSET_H_

#include "../concept.h"


namespace dlplan::generator::rules {

class SubsetConcept : public Concept {
public:
    SubsetConcept() : Concept("c_subset") { }

    virtual void submit_tasks_impl(const States& states, int iteration, GeneratorData& data, utils::threadpool::ThreadPool& th) override {
        if (iteration == 2) {
            for (int i = 1; i < iteration; ++i) {
                int j = iteration - i;
                for (const auto& r1 : data.m_roles_by_iteration[i]) {
                    for (const auto& r2 : data.m_roles_by_iteration[j]) {
                        m_tasks.push_back(
                            th.submit([](const States& states, const core::Role& r1, const core::Role& r2, core::SyntacticElementFactory& factory) {
                                    auto element = factory.make_subset_concept(r1, r2);
                                    auto denotation = evaluate<core::ConceptDenotation>(element, states);
                                    auto hash = compute_hash(bitset_to_num_vec(denotation));
                                    return std::make_pair(std::move(element),std::move(hash));
                                },
                                std::cref(states),
                                std::cref(r1),
                                std::cref(r2),
                                std::ref(*data.m_factory))
                        );
                    }
                }
            }));
        }
    }
};

}

#endif
