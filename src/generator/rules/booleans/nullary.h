#ifndef DLPLAN_SRC_GENERATOR_RULES_BOOLEANS_NULLARY_H_
#define DLPLAN_SRC_GENERATOR_RULES_BOOLEANS_NULLARY_H_

#include "../boolean.h"


namespace dlplan::generator::rules {

class NullaryBoolean : public Boolean {
public:
    NullaryBoolean() : Boolean("b_nullary") { }

    virtual void submit_tasks_impl(const States& states, int iteration, GeneratorData& data, utils::threadpool::ThreadPool& th) override {
        for (const auto& predicate : data.m_factory->get_vocabulary_info()->get_predicates()) {
            if (predicate.get_arity() == 0) {
                m_tasks.push_back(th.submit([](const States& states, const core::Predicate& predicate, core::SyntacticElementFactory& factory){
                    auto element = factory.make_nullary_boolean(predicate);
                    auto denotation = evaluate<bool>(element, states);
                    auto hash = compute_hash(bool_vec_to_num_vec(denotation));
                    return std::make_pair(std::move(element),std::move(hash));
                },
                std::cref(states),
                std::cref(predicate),
                std::ref(*data.m_factory)));
            }
        }
    }
};

}

#endif
