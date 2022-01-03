#ifndef DLPLAN_SRC_GENERATOR_RULES_CONCEPTS_BOT_H_
#define DLPLAN_SRC_GENERATOR_RULES_CONCEPTS_BOT_H_

#include "../rule.h"

namespace dlplan::generator::rules {

class BotConcept : public Rule {
public:
    BotConcept() : Rule("c_bot") { }

    virtual void generate_impl(const States& states, int iteration, GeneratorData& data, utils::threadpool::ThreadPool& th) override {
        th.submit([&](){
            auto result = data.m_factory->make_bot_concept();
            auto denotations = evaluate<core::ConceptDenotation>(result, states);
            auto flat = bitset_to_num_vec<core::ConceptDenotation>(denotations);
            if (data.m_concept_hash_table.insert(compute_hash(flat))) {
                data.m_concept_iteration_data[iteration+1].push_back(std::move(result));
                increment_instantiations();
            }
        });
    }
};

}

#endif
