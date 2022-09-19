#ifndef DLPLAN_SRC_GENERATOR_RULES_BOOLEANS_NULLARY_H_
#define DLPLAN_SRC_GENERATOR_RULES_BOOLEANS_NULLARY_H_

#include "../boolean.h"
#include "../../../core/elements/booleans/nullary.h"


namespace dlplan::generator::rules {

class NullaryBoolean : public Boolean {
public:
    NullaryBoolean() : Boolean() { }

    virtual void submit_tasks_impl(const States& states, int, GeneratorData& data, core::element::GeneratorEvaluationCaches& caches, utils::threadpool::ThreadPool& th) override {
        core::SyntacticElementFactory& factory = data.m_factory;
        for (const auto& predicate : factory.get_vocabulary_info()->get_predicates_ref()) {
            if (predicate.get_arity() == 0) {
                m_tasks.push_back(th.submit(std::cref(m_task), std::cref(states), std::move(factory.make_nullary_boolean(predicate)), std::ref(caches)));
            }
        }
    }

    std::string get_name() const override {
        return core::element::NullaryBoolean::get_name();
    }
};

}

#endif
