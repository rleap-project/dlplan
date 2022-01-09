#ifndef DLPLAN_SRC_GENERATOR_RULES_ROLES_TRANSITIVE_CLOSURE_H_
#define DLPLAN_SRC_GENERATOR_RULES_ROLES_TRANSITIVE_CLOSURE_H_

#include "../role.h"

namespace dlplan::generator::rules {

class TransitiveClosureRole : public Role {
public:
    TransitiveClosureRole() : Role("r_transitive_closure") { }

    virtual void submit_tasks_impl(const States& states, int iteration, GeneratorData& data, utils::threadpool::ThreadPool& th) override {
        if (iteration == 1) {
            for (const auto& c : data.m_roles_by_iteration[iteration]) {
                m_tasks.push_back(
                    th.submit([](const States& states, const core::Role& r, core::SyntacticElementFactory& factory) {
                            auto element = factory.make_transitive_closure(r);
                            auto denotation = evaluate<core::RoleDenotation>(element, states);
                            auto hash = compute_hash(bitset_to_num_vec(denotation));
                            return std::make_pair(std::move(element),std::move(hash));
                        },
                        std::cref(states),
                        std::cref(c),
                        std::ref(*data.m_factory))
                );
            }
        }
    }
};

}

#endif
