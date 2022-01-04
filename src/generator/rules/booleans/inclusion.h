#ifndef DLPLAN_SRC_GENERATOR_RULES_BOOLEANS_INCLUSION_H_
#define DLPLAN_SRC_GENERATOR_RULES_BOOLEANS_INCLUSION_H_

#include "../rule.h"


namespace dlplan::generator::rules {

class InclusionBoolean : public Rule {
public:
    InclusionBoolean() : Rule("b_inclusion") { }

    virtual void generate_impl(const States& states, int iteration, GeneratorData& data, utils::threadpool::ThreadPool& th) override {
        for (int i = 1; i < iteration; ++i) {
            int j = iteration - i;
            for (const auto& c1 : data.m_concept_iteration_data[i].get_elements()) {
                for (const auto& c2 : data.m_concept_iteration_data[j].get_elements()) {
                    th.submit([&](){
                        auto result = data.m_factory->make_concept_inclusion_boolean(c1, c2);
                        add_boolean(*this, iteration, std::move(result), states, data);
                    });
                }
            }
        }
        for (int i = 1; i < iteration; ++i) {
            int j = iteration - i;
            for (const auto& r1 : data.m_role_iteration_data[i].get_elements()) {
                for (const auto& r2 : data.m_role_iteration_data[j].get_elements()) {
                    th.submit([&](){
                        auto result = data.m_factory->make_role_inclusion_boolean(r1, r2);
                        add_boolean(*this, iteration, std::move(result), states, data);
                    });
                }
            }
        }
    }
};

}

#endif
