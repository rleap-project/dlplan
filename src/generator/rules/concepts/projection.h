#ifndef DLPLAN_SRC_GENERATOR_RULES_CONCEPTS_PROJECTION_H_
#define DLPLAN_SRC_GENERATOR_RULES_CONCEPTS_PROJECTION_H_

#include "../concept.h"


namespace dlplan::generator::rules {

class ProjectionConcept : public Concept {
public:
    ProjectionConcept() : Concept("c_projection") { }

    virtual void submit_tasks_impl(const States& states, int iteration, GeneratorData& data, utils::threadpool::ThreadPool& th) override {
        for (const auto& r : data.m_roles_by_iteration[iteration]) {
            for (int pos = 0; pos < 2; ++pos) {
                m_tasks.push_back(
                    th.submit([](const States& states, const core::Role& r, int pos, core::SyntacticElementFactory& factory) {
                            auto element = factory.make_projection_concept(r, pos);
                            auto denotation = evaluate<core::ConceptDenotation>(element, states);
                            auto hash = compute_hash(bitset_to_num_vec(denotation));
                            return std::make_pair(std::move(element),std::move(hash));
                        },
                        std::cref(states),
                        std::cref(r),
                        pos,
                        std::ref(*data.m_factory))
                );
            }
        }));
    }
};

}

#endif
