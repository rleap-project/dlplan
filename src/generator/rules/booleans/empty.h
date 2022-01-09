#ifndef DLPLAN_SRC_GENERATOR_RULES_BOOLEANS_EMPTY_H_
#define DLPLAN_SRC_GENERATOR_RULES_BOOLEANS_EMPTY_H_

#include "../rule.h"


namespace dlplan::generator::rules {

class EmptyBoolean : public Rule {
public:
    EmptyBoolean() : Rule("b_empty") { }

    virtual void generate_impl(const States& states, int iteration, GeneratorData& data, utils::threadpool::ThreadPool& th, std::vector<utils::threadpool::ThreadPool::TaskFuture<void>>& tasks) override {
        tasks.push_back(th.submit([&](){
            for (const auto& c : data.m_concept_iteration_data[iteration].get_elements()) {
                add_boolean(*this, iteration, data.m_factory->make_empty_boolean(c), states, data);
            }
        }));
        tasks.push_back(th.submit([&](){
            for (const auto& r : data.m_role_iteration_data[iteration].get_elements()) {
                add_boolean(*this, iteration, data.m_factory->make_empty_boolean(r), states, data);
            }
        }));
    }
};

}

#endif
