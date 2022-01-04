#ifndef DLPLAN_SRC_GENERATOR_RULES_ROLES_PRIMITIVE_H_
#define DLPLAN_SRC_GENERATOR_RULES_ROLES_PRIMITIVE_H_

#include "../rule.h"


namespace dlplan::generator::rules {

class PrimitiveRole : public Rule {
public:
    PrimitiveRole() : Rule("r_primitive") { }

    virtual void generate_impl(const States& states, int iteration, GeneratorData& data, utils::threadpool::ThreadPool& th) override {
        th.submit([&](){
            for (const auto& predicate : data.m_factory->get_vocabulary_info()->get_predicates()) {
                for (int pos1 = 0; pos1 < predicate.get_arity(); ++pos1) {
                    for (int pos2 = pos1 + 1; pos2 < predicate.get_arity(); ++pos2) {
                        auto result = data.m_factory->make_primitive_role(predicate, pos1, pos2);
                        add_role(*this, iteration, std::move(result), states, data);
                    }
                }
            }
        });
    }
};

}

#endif
