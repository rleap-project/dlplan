#ifndef DLPLAN_SRC_GENERATOR_RULES_CONCEPTS_TOP_H_
#define DLPLAN_SRC_GENERATOR_RULES_CONCEPTS_TOP_H_

#include "../concept.h"


namespace dlplan::generator::rules {

class TopConcept : public Concept {
public:
    TopConcept() : Concept("c_top") { }

    virtual void submit_tasks_impl(const States& states, int iteration, GeneratorData& data, utils::threadpool::ThreadPool& th) override {
        m_tasks.push_back(th.submit([](const States& states, core::SyntacticElementFactory& factory) {
                auto element = factory.make_top_concept();
                auto denotation = evaluate<core::ConceptDenotation>(element, states);
                auto hash = compute_hash(bitset_to_num_vec(denotation));
                return std::make_pair(std::move(element),std::move(hash));
            },
            std::cref(states),
            std::ref(*data.m_factory)
        ));
    }
};

}

#endif
