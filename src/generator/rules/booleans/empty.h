#ifndef DLPLAN_SRC_GENERATOR_RULES_BOOLEANS_EMPTY_H_
#define DLPLAN_SRC_GENERATOR_RULES_BOOLEANS_EMPTY_H_

#include "../rule.h"


namespace dlplan::generator::rules {

class EmptyBoolean : public Rule {
public:
    EmptyBoolean() : Rule("b_empty") { }

    virtual void generate_impl(const States& states, int iteration, GeneratorData& data, utils::threadpool::ThreadPool& th) override {
        for (const auto& c : data.m_concept_iteration_data[iteration].get_elements()) {
            th.submit([&](){
                auto result = data.m_factory->make_empty_boolean(c);
                add_boolean(*this, iteration, std::move(result), states, data);
            });
        }
        for (const auto& r : data.m_role_iteration_data[iteration].get_elements()) {
            th.submit([&](){
                auto result = data.m_factory->make_empty_boolean(r);
                add_boolean(*this, iteration, std::move(result), states, data);
            });
        }
    }
};

}

#endif
