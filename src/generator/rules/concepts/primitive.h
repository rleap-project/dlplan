#ifndef DLPLAN_SRC_GENERATOR_RULES_CONCEPTS_PRIMITIVE_H_
#define DLPLAN_SRC_GENERATOR_RULES_CONCEPTS_PRIMITIVE_H_

#include "../rule.h"


namespace dlplan::generator::rules {

class PrimitiveConcept : public Rule {
public:
    PrimitiveConcept() : Rule("c_primitive") { }

    virtual void generate_impl(const States& states, int iteration, GeneratorData& data, utils::threadpool::ThreadPool& th) override {
        /* Submit a task for every element. */
        std::vector<utils::threadpool::ThreadPool::TaskFuture<std::pair<core::Concept, std::array<u_int32_t, 4>>>> tasks;
        for (const auto& predicate : data.m_factory->get_vocabulary_info()->get_predicates()) {
            for (int pos = 0; pos < predicate.get_arity(); ++pos) {
                tasks.push_back(th.submit([](const States& states, const core::Predicate& predicate, int pos, core::SyntacticElementFactory& factory){
                    auto element = factory.make_primitive_concept(predicate, pos);
                    auto denotation = evaluate<core::ConceptDenotation>(element, states);
                    auto hash = compute_hash(bitset_to_num_vec(denotation));
                    return std::make_pair(std::move(element),std::move(hash));
                },
                std::cref(states),
                std::cref(predicate),
                pos,
                std::ref(data.m_factory)));
            }
        }
        /* Wait for the result and add it. */
        for (auto& task : tasks) {
            auto result = task.get();
            if (data.m_concept_hash_table.insert(std::move(result.second))) {
                data.m_concept_iteration_data[iteration].push_back(std::move(result.first));
            }
        }
    }
};

}

#endif
