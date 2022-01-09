#ifndef DLPLAN_SRC_GENERATOR_RULES_CONCEPTS_PRIMITIVE_H_
#define DLPLAN_SRC_GENERATOR_RULES_CONCEPTS_PRIMITIVE_H_

#include "../concept.h"


namespace dlplan::generator::rules {

class PrimitiveConcept : public Concept {
public:
    PrimitiveConcept() : Concept("c_primitive") { }

    virtual void submit_tasks_impl(const States& states, int iteration, GeneratorData& data, utils::threadpool::ThreadPool& th) override {
        for (const auto& predicate : data.m_factory->get_vocabulary_info()->get_predicates()) {
            for (int pos = 0; pos < predicate.get_arity(); ++pos) {
                m_tasks.push_back(th.submit([](const States& states, const core::Predicate& predicate, int pos, core::SyntacticElementFactory& factory){
                    auto element = factory.make_primitive_concept(predicate, pos);
                    auto denotation = evaluate<core::ConceptDenotation>(element, states);
                    auto hash = compute_hash(bitset_to_num_vec(denotation));
                    return std::make_pair(std::move(element),std::move(hash));
                },
                std::cref(states),
                std::cref(predicate),
                pos,
                std::ref(*data.m_factory)));
            }
        }
    }
};

}

#endif
