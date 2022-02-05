#ifndef DLPLAN_SRC_GENERATOR_RULES_CONCEPTS_BOT_H_
#define DLPLAN_SRC_GENERATOR_RULES_CONCEPTS_BOT_H_

#include "../concept.h"


namespace dlplan::generator::rules {

class BotConcept : public Concept {
public:
    BotConcept() : Concept("c_bot") { }

    virtual void submit_tasks_impl(const States& states, int, GeneratorData& data, utils::threadpool::ThreadPool& th) override {
        core::SyntacticElementFactory factory = *data.m_factory;
        m_tasks.push_back(th.submit(m_task, std::cref(states),factory.make_bot_concept()));
    }
};

}

#endif
