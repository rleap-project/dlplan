#ifndef DLPLAN_SRC_GENERATOR_RULES_CONCEPTS_ONE_OF_H_
#define DLPLAN_SRC_GENERATOR_RULES_CONCEPTS_ONE_OF_H_

#include "../concept.h"

#include "../../../core/elements/concepts/one_of.h"


namespace dlplan::generator::rules {

class OneOfConcept : public Concept {
public:
    OneOfConcept() : Concept() { }

    virtual void submit_tasks_impl(const States& states, int, GeneratorData& data, utils::threadpool::ThreadPool& th) override {
        core::SyntacticElementFactory& factory = data.m_factory;
        for (const auto& constant : factory.get_vocabulary_info()->get_constants()) {
            m_tasks.push_back(th.submit(std::cref(m_task), std::cref(states), std::move(factory.make_one_of_concept(constant))));
        }
    }

    std::string get_name() const override {
        return core::element::OneOfConcept::get_name();
    }
};

}

#endif
