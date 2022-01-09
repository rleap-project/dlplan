#ifndef DLPLAN_SRC_GENERATOR_RULES_BOOLEANS_EMPTY_H_
#define DLPLAN_SRC_GENERATOR_RULES_BOOLEANS_EMPTY_H_

#include "../boolean.h"


namespace dlplan::generator::rules {

class EmptyBoolean : public Boolean {
public:
    EmptyBoolean() : Boolean("b_empty") { }

    virtual void submit_tasks_impl(const States& states, int iteration, GeneratorData& data, utils::threadpool::ThreadPool& th) override {
        for (const auto& c : data.m_concepts_by_iteration[iteration]) {
            m_tasks.push_back(
                th.submit([&](const core::Concept& c, core::SyntacticElementFactory& factory){
                    auto element = factory.make_empty_boolean(c);
                    auto denotation = evaluate<bool>(element, states);
                    auto hash = compute_hash(bool_vec_to_num_vec(denotation));
                    return std::make_pair(std::move(element),std::move(hash));
                },
                std::cref(c),
                std::ref(*data.m_factory))
            );
        }
        for (const auto& r : data.m_roles_by_iteration[iteration]) {
            m_tasks.push_back(
                th.submit([&](const core::Role& r, core::SyntacticElementFactory& factory){
                    auto element = factory.make_empty_boolean(r);
                    auto denotation = evaluate<bool>(element, states);
                    auto hash = compute_hash(bool_vec_to_num_vec(denotation));
                    return std::make_pair(std::move(element),std::move(hash));
                },
                std::cref(r),
                std::ref(*data.m_factory))
            );
        }
    }
};

}

#endif
