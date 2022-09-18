#ifndef DLPLAN_SRC_GENERATOR_RULES_ROLES_PRIMITIVE_H_
#define DLPLAN_SRC_GENERATOR_RULES_ROLES_PRIMITIVE_H_

#include "../role.h"

#include "../../../core/elements/roles/primitive.h"


namespace dlplan::generator::rules {

class PrimitiveRole : public Role {
public:
    PrimitiveRole() : Role() { }

    virtual void submit_tasks_impl(const States& states, int, GeneratorData& data, core::element::GeneratorEvaluationCaches& caches, utils::threadpool::ThreadPool& th) override {
        core::SyntacticElementFactory& factory = data.m_factory;
        for (const auto& predicate : factory.get_vocabulary_info()->get_predicates()) {
            if (predicate.get_arity() == 2) {
                m_tasks.push_back(th.submit(std::cref(m_task), std::cref(states), std::move(factory.make_primitive_role(predicate, 0, 1)), std::ref(caches)));
            }
        }
    }

    std::string get_name() const override {
        return core::element::PrimitiveRole::get_name();
    }
};

}

#endif
