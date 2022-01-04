#ifndef DLPLAN_SRC_GENERATOR_RULES_CONCEPTS_ONE_OF_H_
#define DLPLAN_SRC_GENERATOR_RULES_CONCEPTS_ONE_OF_H_

#include "../rule.h"

namespace dlplan::generator::rules {

class OneOfConcept : public Rule {
public:
    OneOfConcept() : Rule("c_one_of") { }

    virtual void generate_impl(const States& states, int iteration, GeneratorData& data, utils::threadpool::ThreadPool& th) override {
        th.submit([&](){
            for (const auto& constant : data.m_factory->get_vocabulary_info()->get_constants()) {
                auto result = data.m_factory->make_one_of_concept(constant);
                add_concept(*this, iteration, std::move(result), states, data);
            }
        });
    }
};

}

#endif
