#ifndef DLPLAN_SRC_GENERATOR_RULES_CONCEPTS_SOME_H_
#define DLPLAN_SRC_GENERATOR_RULES_CONCEPTS_SOME_H_

#include "../rule.h"


namespace dlplan::generator::rules {

class SomeConcept : public Rule {
public:
    SomeConcept() : Rule("c_some") { }

    virtual void generate_impl(const States& states, int iteration, GeneratorData& data, utils::threadpool::ThreadPool& th) override {
        for (int i = 1; i < iteration; ++i) {
            int j = iteration - i;
            data.m_role_iteration_data[i].for_each(
                [&](const auto& r){
                    data.m_concept_iteration_data[j].for_each(
                        [&](const auto& c){
                            th.submit([&](){
                                auto result = data.m_factory->make_some_concept(r, c);
                                add_concept(*this, iteration, std::move(result), states, data);
                            });
                        }
                    );
                }
            );
        }
    }
};

}

#endif
