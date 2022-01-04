#ifndef DLPLAN_SRC_GENERATOR_RULES_CONCEPTS_NOT_H_
#define DLPLAN_SRC_GENERATOR_RULES_CONCEPTS_NOT_H_

#include "../rule.h"

namespace dlplan::generator::rules {

class NotConcept : public Rule {
public:
    NotConcept() : Rule("c_not") { }

    virtual void generate_impl(const States& states, int iteration, GeneratorData& data, utils::threadpool::ThreadPool& th) override {
        for (const auto& c : data.m_concept_iteration_data[iteration].get_elements()) {
            th.submit([&](){
                auto result = data.m_factory->make_not_concept(c);
                add_concept(*this, iteration, std::move(result), states, data);
            });
        }
    }
};

}

#endif
