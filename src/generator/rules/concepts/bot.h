#ifndef DLPLAN_SRC_GENERATOR_RULES_CONCEPTS_BOT_H_
#define DLPLAN_SRC_GENERATOR_RULES_CONCEPTS_BOT_H_

#include "../concept.h"


namespace dlplan::generator::rules {

class BotConcept : public Concept {
public:
    BotConcept() : Concept("c_bot") { }

    virtual void submit_tasks_impl(const States& states, int iteration, GeneratorData& data, utils::threadpool::ThreadPool& th) override {
        m_tasks.push_back(th.submit([](const States& states, core::SyntacticElementFactory& factory) {
                auto element = factory.make_bot_concept();
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
