#ifndef DLPLAN_SRC_GENERATOR_RULES_CONCEPTS_PRIMITIVE_H_
#define DLPLAN_SRC_GENERATOR_RULES_CONCEPTS_PRIMITIVE_H_

#include "../rule.h"


namespace dlplan::generator::rules {

class PrimitiveConcept : public Rule {
public:
    PrimitiveConcept() : Rule("c_primitive") { }

    virtual void generate_impl(const States& states, int iteration, GeneratorData& data, utils::threadpool::ThreadPool& th, std::vector<utils::threadpool::ThreadPool::TaskFuture<void>>& tasks) override {
        tasks.push_back(th.submit([&](){
            for (const auto& predicate : data.m_factory->get_vocabulary_info()->get_predicates()) {
                for (int pos = 0; pos < predicate.get_arity(); ++pos) {
                    add_concept(*this, iteration, data.m_factory->make_primitive_concept(predicate, pos), states, data);
                }
            }
        }));
    }
};

}

#endif
