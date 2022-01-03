#ifndef DLPLAN_SRC_GENERATOR_RULES_ROLES_PRIMITIVE_H_
#define DLPLAN_SRC_GENERATOR_RULES_ROLES_PRIMITIVE_H_

#include "../rule.h"

namespace dlplan::generator::rules {

class PrimitiveRole : public Rule {
public:
    PrimitiveRole() : Rule("r_primitive") { }

    virtual void generate_impl(const States& states, int iteration, GeneratorData& data, utils::threadpool::ThreadPool& th) override {
        const std::vector<core::Predicate>& predicates = data.m_factory->get_vocabulary_info()->get_predicates();
        for (const auto& predicate : predicates) {
            for (int pos1 = 0; pos1 < predicate.get_arity(); ++pos1) {
                for (int pos2 = pos1 + 1; pos2 < predicate.get_arity(); ++pos2) {
                    th.submit([&, pos1, pos2](){
                        auto role = data.m_factory->make_primitive_role(predicate, pos1, pos2);
                        auto denotations = evaluate<core::RoleDenotation>(role, states);
                        auto flat = bitset_to_num_vec<core::RoleDenotation>(denotations);
                        if (data.m_role_hash_table.insert(compute_hash(flat))) {
                            data.m_role_iteration_data[1].push_back(std::move(role));
                            increment_instantiations();
                        }
                    });
                }
            }
        }
    }
};

}

#endif
