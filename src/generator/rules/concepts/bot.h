#ifndef DLPLAN_SRC_GENERATOR_RULES_CONCEPTS_BOT_H_
#define DLPLAN_SRC_GENERATOR_RULES_CONCEPTS_BOT_H_

#include "../rule.h"

namespace dlplan::generator::rules {

class BotConcept : public Rule {
public:
    BotConcept() : Rule("c_bot") { }

    virtual void generate_impl(const States& states, int iteration, GeneratorData& data, utils::threadpool::ThreadPool& th, std::vector<utils::threadpool::ThreadPool::TaskFuture<void>>& tasks) override {
        tasks.push_back(th.submit([](const States* const states, int iteration, Rule* const rule, GeneratorData* const data, utils::threadpool::ThreadPool* const th, std::vector<utils::threadpool::ThreadPool::TaskFuture<void>>* const tasks){
            add_concept(*rule, iteration, data->m_factory->make_bot_concept(), *states, *data);
        },
            &states,
            iteration,
            this,
            &data,
            &th,
            &tasks));
    }
};

}

#endif
