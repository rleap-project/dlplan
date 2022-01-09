#ifndef DLPLAN_SRC_GENERATOR_RULES_CONCEPTS_NOT_H_
#define DLPLAN_SRC_GENERATOR_RULES_CONCEPTS_NOT_H_

#include "../concept.h"


namespace dlplan::generator::rules {

class NotConcept : public Concept {
public:
    NotConcept() : Concept("c_not") { }

    virtual void submit_tasks_impl(const States& states, int iteration, GeneratorData& data, utils::threadpool::ThreadPool& th) override {
        for (const auto& c : data.m_concepts_by_iteration[iteration]) {
            m_tasks.push_back(
                th.submit([](const States& states, const core::Concept& c, core::SyntacticElementFactory& factory) {
                        auto element = factory.make_not_concept(c);
                        auto denotation = evaluate<core::ConceptDenotation>(element, states);
                        auto hash = compute_hash(bitset_to_num_vec(denotation));
                        return std::make_pair(std::move(element),std::move(hash));
                    },
                    std::cref(states),
                    std::cref(c),
                    std::ref(*data.m_factory))
            );
        }
    }
};

}

#endif
