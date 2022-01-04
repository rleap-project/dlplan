#ifndef DLPLAN_SRC_GENERATOR_RULES_CONCEPTS_TOP_H_
#define DLPLAN_SRC_GENERATOR_RULES_CONCEPTS_TOP_H_

#include "../rule.h"

namespace dlplan::generator::rules {

class TopConcept : public Rule {
public:
    TopConcept() : Rule("c_top") { }

    virtual void generate_impl(const States& states, int iteration, GeneratorData& data, utils::threadpool::ThreadPool& th) override {
        th.submit([&](){
            auto result = data.m_factory->make_top_concept();
            add_concept(*this, iteration, std::move(result), states, data);
        });
    }
};

}

#endif
