#ifndef DLPLAN_SRC_GENERATOR_RULES_CONCEPTS_NOT_H_
#define DLPLAN_SRC_GENERATOR_RULES_CONCEPTS_NOT_H_

#include "../rule.h"

namespace dlplan::generator::rules {

class NotConcept : public Rule {
public:
    NotConcept() : Rule("c_not") { }

    virtual void generate_impl(const States& states, int iteration, GeneratorData& data, utils::threadpool::ThreadPool& th) override {
        for (int i = 1; i < iteration; ++i) {
            data.m_concept_iteration_data[i].for_each(
                [&](const auto& c){
                    th.submit([&](){
                        auto result = data.m_factory->make_not_concept(c);
                        auto denotations = evaluate<core::ConceptDenotation>(result, states);
                        auto flat = bitset_to_num_vec<core::ConceptDenotation>(denotations);
                        if (data.m_concept_hash_table.insert(compute_hash(flat))) {
                            data.m_concept_iteration_data[iteration+1].push_back(std::move(result));
                            increment_instantiations();
                        }
                    });
                }
            );
        }
    }
};

}

#endif
