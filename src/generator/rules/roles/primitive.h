#ifndef DLPLAN_SRC_GENERATOR_RULES_ROLES_PRIMITIVE_H_
#define DLPLAN_SRC_GENERATOR_RULES_ROLES_PRIMITIVE_H_

#include "../role.h"


namespace dlplan::generator::rules {

class PrimitiveRole : public Role {
public:
    PrimitiveRole() : Role("r_primitive") { }

    virtual void submit_tasks_impl(const States& states, int, GeneratorData& data, utils::threadpool::ThreadPool& th) override {
        core::SyntacticElementFactory factory = *data.m_factory;
        for (const auto& predicate : data.m_factory->get_vocabulary_info()->get_predicates()) {
            for (int pos1 = 0; pos1 < predicate.get_arity(); ++pos1) {
                for (int pos2 = pos1+1; pos2 < predicate.get_arity(); ++pos2) {
                    m_tasks.push_back(th.submit(std::cref(m_task), std::cref(states), std::move(factory.make_primitive_role(predicate, pos1, pos2))));
                }
            }
        }
    }
};

}

#endif
