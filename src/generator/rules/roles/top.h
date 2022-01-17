#ifndef DLPLAN_SRC_GENERATOR_RULES_ROLES_TOP_H_
#define DLPLAN_SRC_GENERATOR_RULES_ROLES_TOP_H_

#include "../role.h"

namespace dlplan::generator::rules {

class TopRole : public Role {
public:
    TopRole() : Role("r_top") { }

    virtual void submit_tasks_impl(const States& states, int, GeneratorData& data, utils::threadpool::ThreadPool& th) override {
        m_tasks.push_back(th.submit([](const States& states, core::SyntacticElementFactory& factory) {
                auto element = factory.make_top_role();
                auto denotation = evaluate<core::RoleDenotation>(element, states);
                auto hash = compute_hash(bitset_to_num_vec(denotation));
                return std::make_pair(std::move(element),std::move(hash));
            },
            std::cref(states),
            std::ref(*data.m_factory)
        ));
    }
};

}

#endif
