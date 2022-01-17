#ifndef DLPLAN_SRC_POLICY_RULE_H_
#define DLPLAN_SRC_POLICY_RULE_H_

#include <unordered_set>
#include <vector>
#include <memory>


namespace dlplan::policy {
class PolicyRoot;
template<typename T>
class Feature;
class BaseCondition;
class BaseEffect;

class RuleImpl {
private:
    std::shared_ptr<const PolicyRoot> m_root;

    std::vector<std::shared_ptr<const BaseCondition>> m_conditions;
    std::vector<std::shared_ptr<const BaseEffect>> m_effects;

public:
    // Do we want to pass conditions and effects as ordered sets?
    // How does this work with python bindings then?
    RuleImpl(
        std::shared_ptr<const PolicyRoot> root,
        std::unordered_set<std::shared_ptr<const BaseCondition>>&& conditions,
        std::unordered_set<std::shared_ptr<const BaseEffect>>&& effects);

    bool is_condition_satisfied() const;
    bool is_effect_satisfied() const;

    std::string str() const;

    std::string compute_repr() const;

    std::shared_ptr<const PolicyRoot> get_root() const;
};

}

#endif
