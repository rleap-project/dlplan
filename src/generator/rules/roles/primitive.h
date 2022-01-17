#ifndef DLPLAN_SRC_GENERATOR_RULES_ROLES_PRIMITIVE_H_
#define DLPLAN_SRC_GENERATOR_RULES_ROLES_PRIMITIVE_H_

#include "../role.h"


namespace dlplan::generator::rules {

class PrimitiveRole : public Role {
public:
    PrimitiveRole() : Role("r_primitive") { }

    virtual void submit_tasks_impl(const States& states, int, GeneratorData& data, utils::threadpool::ThreadPool& th) override {
        for (const auto& predicate : data.m_factory->get_vocabulary_info()->get_predicates()) {
            for (int pos1 = 0; pos1 < predicate.get_arity(); ++pos1) {
                for (int pos2 = pos1+1; pos2 < predicate.get_arity(); ++pos2) {
                    m_tasks.push_back(th.submit([](const States& states, const core::Predicate& predicate, int pos1, int pos2, core::SyntacticElementFactory& factory){
                        auto element = factory.make_primitive_role(predicate, pos1, pos2);
                        auto denotation = evaluate<core::RoleDenotation>(element, states);
                        auto hash = compute_hash(bitset_to_num_vec(denotation));
                        return std::make_pair(std::move(element),std::move(hash));
                    },
                    std::cref(states),
                    std::cref(predicate),
                    pos1,
                    pos2,
                    std::ref(*data.m_factory)));
                }
            }
        }
    }
};

}

#endif
