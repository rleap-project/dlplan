#ifndef DLPLAN_SRC_GENERATOR_RULES_CONCEPTS_ONE_OF_H_
#define DLPLAN_SRC_GENERATOR_RULES_CONCEPTS_ONE_OF_H_

#include "../concept.h"


namespace dlplan::generator::rules {

class OneOfConcept : public Concept {
public:
    OneOfConcept() : Concept("c_one_of") { }

    virtual void submit_tasks_impl(const States& states, int, GeneratorData& data, utils::threadpool::ThreadPool& th) override {
        core::SyntacticElementFactory factory = *data.m_factory;
        for (const auto& constant : data.m_factory->get_vocabulary_info()->get_constants()) {
            m_tasks.push_back(th.submit(m_task, std::cref(states),factory.make_one_of_concept(constant)));
        }
    }
};

}

#endif
