#ifndef DLPLAN_SRC_GENERATOR_RULES_CONCEPTS_AND_H_
#define DLPLAN_SRC_GENERATOR_RULES_CONCEPTS_AND_H_

#include "../rule.h"

namespace dlplan::generator::rules {

class AndConcept : public Rule {
public:
    AndConcept() : Rule("c_and") { }

    virtual void generate_impl(const States& states, int iteration, GeneratorData& data, utils::threadpool::ThreadPool& th) override {
        for (int i = 1; i < iteration; ++i) {
            int j = iteration - i;
            for (const auto& c1 : data.m_concept_iteration_data[i].get_elements()) {
                for (const auto& c2 : data.m_concept_iteration_data[j].get_elements()) {
                    th.submit([&](){
                        auto result = data.m_factory->make_and_concept(c1, c2);
                        add_concept(*this, iteration, std::move(result), states, data);
                    });
                }
            }
        }
    }
};

}

#endif
