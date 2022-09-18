#ifndef DLPLAN_SRC_GENERATOR_RULES_CONCEPTS_BOT_H_
#define DLPLAN_SRC_GENERATOR_RULES_CONCEPTS_BOT_H_

#include "../concept.h"

#include "../../../core/elements/concepts/bot.h"


namespace dlplan::generator::rules {

class BotConcept : public Concept {
public:
    BotConcept() : Concept() { }

    virtual void submit_tasks_impl(const States& states, int, GeneratorData& data, core::element::GeneratorEvaluationCaches& caches, utils::threadpool::ThreadPool& th) override {
        core::SyntacticElementFactory& factory = data.m_factory;
        m_tasks.push_back(th.submit(std::cref(m_task), std::cref(states), std::move(factory.make_bot_concept()), std::ref(caches)));
    }

    std::string get_name() const override {
        return core::element::BotConcept::get_name();
    }
};

}

#endif
