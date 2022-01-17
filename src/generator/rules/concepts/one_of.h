#ifndef DLPLAN_SRC_GENERATOR_RULES_CONCEPTS_ONE_OF_H_
#define DLPLAN_SRC_GENERATOR_RULES_CONCEPTS_ONE_OF_H_

#include "../concept.h"


namespace dlplan::generator::rules {

class OneOfConcept : public Concept {
public:
    OneOfConcept() : Concept("c_one_of") { }

    virtual void submit_tasks_impl(const States& states, int, GeneratorData& data, utils::threadpool::ThreadPool& th) override {
        for (const auto& constant : data.m_factory->get_vocabulary_info()->get_constants()) {
            m_tasks.push_back(
                th.submit([](const States& states, const core::Constant& constant, core::SyntacticElementFactory& factory) {
                        auto element = factory.make_one_of_concept(constant);
                        auto denotation = evaluate<core::ConceptDenotation>(element, states);
                        auto hash = compute_hash(bitset_to_num_vec(denotation));
                        return std::make_pair(std::move(element),std::move(hash));
                    },
                    std::cref(states),
                    std::cref(constant),
                    std::ref(*data.m_factory))
            );
        }
    }
};

}

#endif
