#ifndef DLPLAN_SRC_GENERATOR_RULES_CONCEPTS_DIFF_H_
#define DLPLAN_SRC_GENERATOR_RULES_CONCEPTS_DIFF_H_

#include "../concept.h"


namespace dlplan::generator::rules {

class DiffConcept : public Concept {

public:
    DiffConcept() : Concept("c_diff") { }

    virtual void submit_tasks_impl(const States& states, int iteration, GeneratorData& data, utils::threadpool::ThreadPool& th) override {
       for (int i = 1; i < iteration; ++i) {
            int j = iteration - i;
            for (const auto& c1 : data.m_concepts_by_iteration[i]) {
                for (const auto& c2 : data.m_concepts_by_iteration[j]) {
                    m_tasks.push_back(
                        th.submit([](const States& states, const core::Concept& c1, const core::Concept& c2, core::SyntacticElementFactory& factory) {
                                auto element = factory.make_diff_concept(c1, c2);
                                auto denotation = evaluate<core::ConceptDenotation>(element, states);
                                auto hash = compute_hash(bitset_to_num_vec(denotation));
                                return std::make_pair(std::move(element),std::move(hash));
                            },
                            std::cref(states),
                            std::cref(c1),
                            std::cref(c2),
                            std::ref(*data.m_factory))
                    );
                }
            );
        }
    }
};

}

#endif
