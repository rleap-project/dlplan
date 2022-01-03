#ifndef DLPLAN_SRC_GENERATOR_RULES_CONCEPTS_OR_H_
#define DLPLAN_SRC_GENERATOR_RULES_CONCEPTS_OR_H_

#include "../rule.h"

namespace dlplan::generator::rules {

class OrConcept : public Rule {
public:
    OrConcept() : Rule("c_or") { }

    virtual void generate_impl(const States& states, int iteration, GeneratorData& data, utils::threadpool::ThreadPool& th) override {
        for (int i = 1; i < iteration; ++i) {
            int j = iteration - i;
            data.m_concept_iteration_data[i].for_each(
                [&](const auto& c1){
                    data.m_concept_iteration_data[j].for_each(
                        [&](const auto& c2){
                            th.submit([&](){
                                auto result = data.m_factory->make_or_concept(c1, c2);
                                auto denotations = evaluate<core::ConceptDenotation>(result, states);
                                auto flat = bitset_to_num_vec<core::ConceptDenotation>(denotations);
                                if (data.m_concept_hash_table.insert(compute_hash(flat))) {
                                    data.m_concept_iteration_data[iteration].push_back(std::move(result));
                                    increment_instantiations();
                                }
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
