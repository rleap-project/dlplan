#ifndef DLPLAN_SRC_GENERATOR_RULES_CONCEPTS_PROJECTION_H_
#define DLPLAN_SRC_GENERATOR_RULES_CONCEPTS_PROJECTION_H_

#include "../rule.h"

namespace dlplan::generator::rules {

class ProjectionConcept : public Rule {
public:
    ProjectionConcept() : Rule("c_projection") { }

    virtual void generate_impl(const States& states, int iteration, GeneratorData& data, utils::threadpool::ThreadPool& th) override {
        for (int pos = 0; pos < 2; ++pos) {
            data.m_role_iteration_data[iteration].for_each(
                [&](const auto& r){
                    th.submit([&, pos](){
                        auto result = data.m_factory->make_projection_concept(r, pos);
                        add_concept(*this, iteration, std::move(result), states, data);
                    });
                }
            );
        }
    }
};

}

#endif
