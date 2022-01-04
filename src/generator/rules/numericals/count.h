#ifndef DLPLAN_SRC_GENERATOR_RULES_NUMERICALS_COUNT_H_
#define DLPLAN_SRC_GENERATOR_RULES_NUMERICALS_COUNT_H_

#include "../rule.h"


namespace dlplan::generator::rules {

class CountNumerical : public Rule {
public:
    CountNumerical() : Rule("n_count") { }

    virtual void generate_impl(const States& states, int iteration, GeneratorData& data, utils::threadpool::ThreadPool& th) override {
        data.m_concept_iteration_data[iteration].for_each(
            [&](const auto& c){
                th.submit([&](){
                    auto result = data.m_factory->make_count(c);
                    add_numerical(*this, iteration, std::move(result), states, data);
                });
            }
        );
        data.m_role_iteration_data[iteration].for_each(
            [&](const auto& r){
                th.submit([&](){
                    auto result = data.m_factory->make_count(r);
                    add_numerical(*this, iteration, std::move(result), states, data);
                });
            }
        );
    }
};

}

#endif
