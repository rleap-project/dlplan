#ifndef DLPLAN_SRC_GENERATOR_RULES_CONCEPTS_TOP_H_
#define DLPLAN_SRC_GENERATOR_RULES_CONCEPTS_TOP_H_

#include "../concept.h"

#include "../../../core/elements/concepts/top.h"


namespace dlplan::generator::rules {

class TopConcept : public Concept {
public:
    TopConcept() : Concept() { }

    virtual void submit_tasks_impl(const States& states, int, GeneratorData& data, utils::threadpool::ThreadPool& th) override {
        core::SyntacticElementFactory& factory = data.m_factory;
        m_tasks.push_back(th.submit(std::cref(m_task), std::cref(states), std::move(factory.make_top_concept())));
    }

    std::string get_name() const override {
        return core::element::TopConcept::get_name();
    }
};

}

#endif
