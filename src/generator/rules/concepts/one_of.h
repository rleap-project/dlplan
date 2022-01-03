#ifndef DLPLAN_SRC_GENERATOR_RULES_CONCEPTS_ONE_OF_H_
#define DLPLAN_SRC_GENERATOR_RULES_CONCEPTS_ONE_OF_H_

#include "../rule.h"

namespace dlplan::generator::rules {

class OneOfConcept : public Rule {
public:
    OneOfConcept() : Rule("c_one_of") { }

    virtual void generate_impl(const States& states, int iteration, GeneratorData& data, utils::threadpool::ThreadPool& th) override {
        for (const auto& constant : data.m_factory->get_vocabulary_info()->get_constants()) {
            th.submit([&](){
                auto result = data.m_factory->make_one_of_concept(constant);
                auto denotations = evaluate<core::ConceptDenotation>(result, states);
                auto flat = bitset_to_num_vec<core::ConceptDenotation>(denotations);
                if (data.m_concept_hash_table.insert(compute_hash(flat))) {
                    data.m_concept_iteration_data[iteration+1].push_back(std::move(result));
                    increment_instantiations();
                }
            });
        }
    }
};

}

#endif
