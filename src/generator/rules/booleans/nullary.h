#ifndef DLPLAN_SRC_GENERATOR_RULES_BOOLEANS_NULLARY_H_
#define DLPLAN_SRC_GENERATOR_RULES_BOOLEANS_NULLARY_H_

#include "../rule.h"


namespace dlplan::generator::rules {

class NullaryBoolean : public Rule {
public:
    NullaryBoolean() : Rule("b_nullary") { }

    virtual void generate_impl(const States& states, int iteration, GeneratorData& data, utils::threadpool::ThreadPool& th, std::vector<utils::threadpool::ThreadPool::TaskFuture<void>>& tasks) override {
        tasks.push_back(th.submit([](const States* const states, int iteration, Rule* const rule, GeneratorData* const data, utils::threadpool::ThreadPool* const th, std::vector<utils::threadpool::ThreadPool::TaskFuture<void>>* const tasks){
            const std::vector<core::Predicate>& predicates = data->m_factory->get_vocabulary_info()->get_predicates();
            for (const auto& predicate : predicates) {
                if (predicate.get_arity() == 0) {
                    add_boolean(*rule, iteration, data->m_factory->make_nullary_boolean(predicate), *states, *data);
                }
            }
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
