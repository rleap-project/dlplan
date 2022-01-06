#ifndef DLPLAN_SRC_GENERATOR_RULES_CONCEPTS_TOP_H_
#define DLPLAN_SRC_GENERATOR_RULES_CONCEPTS_TOP_H_

#include "../rule.h"

namespace dlplan::generator::rules {

class TopConcept : public Rule {
public:
    TopConcept() : Rule("c_top") { }

    virtual void generate_impl(const States& states, int iteration, GeneratorData& data, utils::threadpool::ThreadPool& th, std::vector<utils::threadpool::ThreadPool::TaskFuture<void>>& tasks) override {
        tasks.push_back(th.submit([&](){
            add_concept(*this, iteration, data.m_factory->make_top_concept(), states, data);
        }));
    }
};

}

#endif
