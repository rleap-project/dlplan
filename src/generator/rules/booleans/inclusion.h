#ifndef DLPLAN_SRC_GENERATOR_RULES_BOOLEANS_INCLUSION_H_
#define DLPLAN_SRC_GENERATOR_RULES_BOOLEANS_INCLUSION_H_

#include "../boolean.h"


namespace dlplan::generator::rules {

class InclusionBoolean : public Boolean {
public:
    InclusionBoolean() : Boolean("b_inclusion") { }

    virtual void submit_tasks_impl(const States& states, int iteration, GeneratorData& data, utils::threadpool::ThreadPool& th) override {
        for (int i = 1; i < iteration; ++i) {
            int j = iteration - i;
            for (const auto& c1 : data.m_concepts_by_iteration[i]) {
                for (const auto& c2 : data.m_concepts_by_iteration[j]) {
                    m_tasks.push_back(
                        th.submit([](const States& states, const core::Concept& c1, const core::Concept& c2, core::SyntacticElementFactory& factory) {
                                auto element = factory.make_concept_inclusion_boolean(c1, c2);
                                auto denotation = evaluate<bool>(element, states);
                                auto hash = compute_hash(bool_vec_to_num_vec(denotation));
                                return std::make_pair(std::move(element),std::move(hash));
                            },
                            std::cref(states),
                            std::cref(c1),
                            std::cref(c2),
                            std::ref(*data.m_factory))
                    );
                }
            }
        }
        for (int i = 1; i < iteration; ++i) {
            int j = iteration - i;
            for (const auto& r1 : data.m_roles_by_iteration[i]) {
                for (const auto& r2 : data.m_roles_by_iteration[j]) {
                    m_tasks.push_back(
                        th.submit([](const States& states, const core::Role& r1, const core::Role& r2, core::SyntacticElementFactory& factory) {
                                auto element = factory.make_role_inclusion_boolean(r1, r2);
                                auto denotation = evaluate<bool>(element, states);
                                auto hash = compute_hash(bool_vec_to_num_vec(denotation));
                                return std::make_pair(std::move(element),std::move(hash));
                            },
                            std::cref(states),
                            std::cref(r1),
                            std::cref(r2),
                            std::ref(*data.m_factory))
                    );
                }
            }
        }
    }
};

}

#endif
