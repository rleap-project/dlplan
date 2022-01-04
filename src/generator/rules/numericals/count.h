#ifndef DLPLAN_SRC_GENERATOR_RULES_NUMERICALS_COUNT_H_
#define DLPLAN_SRC_GENERATOR_RULES_NUMERICALS_COUNT_H_

#include "../numerical.h"



namespace dlplan::generator::rules {

class CountNumerical : public Numerical {
public:
    CountNumerical() : Numerical("n_count") { }

    virtual void submit_tasks_impl(const States& states, int iteration, GeneratorData& data, utils::threadpool::ThreadPool& th) override {
        for (const auto& c : data.m_concepts_by_iteration[iteration]) {
            m_tasks.push_back(
                th.submit([&](const core::Concept& c, core::SyntacticElementFactory& factory){
                    auto element = factory.make_count(c);
                    auto denotation = evaluate<int>(element, states);
                    auto hash = compute_hash(denotation);
                    return std::make_pair(std::move(element),std::move(hash));
                },
                std::cref(c),
                std::ref(*data.m_factory))
            );
        }
        for (const auto& r : data.m_roles_by_iteration[iteration]) {
            m_tasks.push_back(
                th.submit([&](const core::Role& r, core::SyntacticElementFactory& factory){
                    auto element = factory.make_count(r);
                    auto denotation = evaluate<int>(element, states);
                    auto hash = compute_hash(denotation);
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
