#ifndef DLPLAN_SRC_GENERATOR_RULES_CONCEPTS_DIFF_H_
#define DLPLAN_SRC_GENERATOR_RULES_CONCEPTS_DIFF_H_

#include "../rule.h"

namespace dlplan::generator::rules {

class DiffConcept : public Rule {
public:
    DiffConcept() : Rule("c_diff") { }

    virtual void generate_impl(const States& states, int iteration, GeneratorData& data, utils::threadpool::ThreadPool& th) override {
        for (int i = 1; i < iteration; ++i) {
            int j = iteration - i;
            data.m_concept_iteration_data[i].for_each(
                [&](const auto& c1){
                    data.m_concept_iteration_data[j].for_each(
                        [&](const auto& c2){
                            th.submit([&](){
                                auto result = data.m_factory->make_diff_concept(c1, c2);
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
