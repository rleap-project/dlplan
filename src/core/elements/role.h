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
        auto cache_result = caches.get_role_denotation(*this);
        RoleDenotation result = cache_result.first;
        bool cache_status = cache_result.second;
        if (cache_status) {
            return result;
        } else {
            return evaluate(state, caches, result);
        }
    }
};

}

#endif
