#ifndef DLPLAN_SRC_CORE_ELEMENTS_ROLE_H_
#define DLPLAN_SRC_CORE_ELEMENTS_ROLE_H_

#include "element.h"


namespace dlplan::core::element {

class Role : public Element<RoleDenotation> {
public:
    Role(const VocabularyInfo& vocabulary, const std::string& name)
    : Element<RoleDenotation>(vocabulary, name) { }
    ~Role() override = default;

    virtual RoleDenotation evaluate(const State& state, EvaluationCaches& caches, RoleDenotation out) const = 0;

    RoleDenotation evaluate(const State& state, EvaluationCaches& caches) const override {
       return caches.try_retrieve_or_evaluate(state, *this);
    }
};

}

#endif
