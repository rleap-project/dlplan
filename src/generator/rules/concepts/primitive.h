#ifndef DLPLAN_SRC_GENERATOR_RULES_CONCEPTS_PRIMITIVE_H_
#define DLPLAN_SRC_GENERATOR_RULES_CONCEPTS_PRIMITIVE_H_

#include "../concept.h"

#include "../../../core/elements/concepts/primitive.h"


namespace dlplan::generator::rules {

class PrimitiveConcept : public Concept {
public:
    PrimitiveConcept() : Concept() { }

    virtual void submit_tasks_impl(const States& states, int, GeneratorData& data, utils::threadpool::ThreadPool& th) override {
        core::SyntacticElementFactory& factory = data.m_factory;
        for (const auto& predicate : factory.get_vocabulary_info()->get_predicates()) {
            for (int pos = 0; pos < predicate.get_arity(); ++pos) {
                m_tasks.push_back(th.submit(std::cref(m_task), std::cref(states), std::move(factory.make_primitive_concept(predicate, pos))));
            }
        }
    }

    std::string get_name() const override {
        return core::element::PrimitiveConcept::get_name();
    }
};

}

#endif
