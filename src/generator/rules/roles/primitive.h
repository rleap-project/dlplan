#ifndef DLPLAN_SRC_GENERATOR_RULES_ROLES_PRIMITIVE_H_
#define DLPLAN_SRC_GENERATOR_RULES_ROLES_PRIMITIVE_H_

#include "../rule.h"


namespace dlplan::generator::rules {

class PrimitiveRole : public Rule {
public:
    PrimitiveRole() : Rule("r_primitive") { }

    virtual void generate_impl(const States& states, int iteration, GeneratorData& data, utils::threadpool::ThreadPool& th, std::vector<utils::threadpool::ThreadPool::TaskFuture<void>>& tasks) override {
        tasks.push_back(th.submit([](const States* const states, int iteration, Rule* const rule, GeneratorData* const data, utils::threadpool::ThreadPool* const th, std::vector<utils::threadpool::ThreadPool::TaskFuture<void>>* const tasks){
            for (const auto& predicate : data->m_factory->get_vocabulary_info()->get_predicates()) {
                for (int pos1 = 0; pos1 < predicate.get_arity(); ++pos1) {
                    for (int pos2 = pos1 + 1; pos2 < predicate.get_arity(); ++pos2) {
                        add_role(*rule, iteration, data->m_factory->make_primitive_role(predicate, pos1, pos2), *states, *data);
                    }
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
