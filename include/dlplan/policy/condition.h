#ifndef DLPLAN_INCLUDE_DLPLAN_POLICY_CONDITION_H_
#define DLPLAN_INCLUDE_DLPLAN_POLICY_CONDITION_H_

#include "../policy.h"

#include <string>
#include <memory>

using namespace dlplan;


namespace dlplan {
template<typename... Ts>
class ReferenceCountedObjectFactory;
}


namespace dlplan::policy {

template<typename Element>
class NamedElementCondition : public BaseCondition {
protected:
    std::shared_ptr<const Element> m_named_element;

    NamedElementCondition(int identifier, const std::shared_ptr<const Element>& named_element)
        : BaseCondition(identifier), m_named_element(named_element) { }

public:
    virtual ~NamedElementCondition() = default;

    int compute_evaluate_time_score() const override {
        return m_named_element->compute_evaluate_time_score();
    }

    size_t hash_impl() const override {
        return hash_combine(m_named_element);
    }

    std::shared_ptr<const Element> get_named_element() const { return m_named_element; }
};


class PositiveBooleanCondition : public NamedElementCondition<NamedBoolean>, public std::enable_shared_from_this<PositiveBooleanCondition> {
private:
    PositiveBooleanCondition(int identifier, std::shared_ptr<const NamedBoolean> boolean);

    template<typename... Ts>
    friend class dlplan::ReferenceCountedObjectFactory;

public:
    bool are_equal_impl(const BaseCondition& other) const override;
    void str_impl(std::stringstream& out) const override;

    bool evaluate(const core::State& source_state) const override;
    bool evaluate(const core::State& source_state, core::DenotationsCaches& caches) const override;

    void accept(BaseConditionVisitor& visitor) const override;
};

class NegativeBooleanCondition : public NamedElementCondition<NamedBoolean>, public std::enable_shared_from_this<NegativeBooleanCondition> {
private:
    NegativeBooleanCondition(int identifier, std::shared_ptr<const NamedBoolean> boolean);

    template<typename... Ts>
    friend class dlplan::ReferenceCountedObjectFactory;

public:
    bool are_equal_impl(const BaseCondition& other) const override;
    void str_impl(std::stringstream& out) const override;

    bool evaluate(const core::State& source_state) const override;
    bool evaluate(const core::State& source_state, core::DenotationsCaches& caches) const override;

    void accept(BaseConditionVisitor& visitor) const override;
};

class GreaterNumericalCondition : public NamedElementCondition<NamedNumerical>, public std::enable_shared_from_this<GreaterNumericalCondition> {
private:
    GreaterNumericalCondition(int identifier, std::shared_ptr<const NamedNumerical> numerical);

    template<typename... Ts>
    friend class dlplan::ReferenceCountedObjectFactory;

public:
    bool are_equal_impl(const BaseCondition& other) const override;
    void str_impl(std::stringstream& out) const override;

    bool evaluate(const core::State& source_state) const override;
    bool evaluate(const core::State& source_state, core::DenotationsCaches& caches) const override;

    void accept(BaseConditionVisitor& visitor) const override;
};

class EqualNumericalCondition : public NamedElementCondition<NamedNumerical>, public std::enable_shared_from_this<EqualNumericalCondition> {
private:
    EqualNumericalCondition(int identifier, std::shared_ptr<const NamedNumerical> numerical);

    template<typename... Ts>
    friend class dlplan::ReferenceCountedObjectFactory;

public:
    bool are_equal_impl(const BaseCondition& other) const override;
    void str_impl(std::stringstream& out) const override;

    bool evaluate(const core::State& source_state) const override;
    bool evaluate(const core::State& source_state, core::DenotationsCaches& caches) const override;

    void accept(BaseConditionVisitor& visitor) const override;
};

class GreaterConceptCondition : public NamedElementCondition<NamedConcept>, public std::enable_shared_from_this<GreaterConceptCondition> {
private:
    GreaterConceptCondition(int identifier, std::shared_ptr<const NamedConcept> concept__);

    template<typename... Ts>
    friend class dlplan::ReferenceCountedObjectFactory;

public:
    bool are_equal_impl(const BaseCondition& other) const override;
    void str_impl(std::stringstream& out) const override;

    bool evaluate(const core::State& source_state) const override;
    bool evaluate(const core::State& source_state, core::DenotationsCaches& caches) const override;

    void accept(BaseConditionVisitor& visitor) const override;
};

class EqualConceptCondition : public NamedElementCondition<NamedConcept>, public std::enable_shared_from_this<EqualConceptCondition> {
private:
    EqualConceptCondition(int identifier, std::shared_ptr<const NamedConcept> concept__);

    template<typename... Ts>
    friend class dlplan::ReferenceCountedObjectFactory;

public:
    bool are_equal_impl(const BaseCondition& other) const override;
    void str_impl(std::stringstream& out) const override;

    bool evaluate(const core::State& source_state) const override;
    bool evaluate(const core::State& source_state, core::DenotationsCaches& caches) const override;

    void accept(BaseConditionVisitor& visitor) const override;
};

/// @brief Defines an interface for visiting conditions.
class BaseConditionVisitor {
public:
    virtual void visit(const std::shared_ptr<const PositiveBooleanCondition>& condition) = 0;
    virtual void visit(const std::shared_ptr<const NegativeBooleanCondition>& condition) = 0;
    virtual void visit(const std::shared_ptr<const GreaterNumericalCondition>& condition) = 0;
    virtual void visit(const std::shared_ptr<const EqualNumericalCondition>& condition) = 0;
    virtual void visit(const std::shared_ptr<const GreaterConceptCondition>& condition) = 0;
    virtual void visit(const std::shared_ptr<const EqualConceptCondition>& condition) = 0;
};

}


namespace std {
    template<>
    struct less<std::shared_ptr<const dlplan::policy::PositiveBooleanCondition>>
    {
        bool operator()(
            const std::shared_ptr<const dlplan::policy::PositiveBooleanCondition>& left_condition,
            const std::shared_ptr<const dlplan::policy::PositiveBooleanCondition>& right_condition) const;
    };

    template<>
    struct less<std::shared_ptr<const dlplan::policy::NegativeBooleanCondition>>
    {
        bool operator()(
            const std::shared_ptr<const dlplan::policy::NegativeBooleanCondition>& left_condition,
            const std::shared_ptr<const dlplan::policy::NegativeBooleanCondition>& right_condition) const;
    };

    template<>
    struct less<std::shared_ptr<const dlplan::policy::GreaterNumericalCondition>>
    {
        bool operator()(
            const std::shared_ptr<const dlplan::policy::GreaterNumericalCondition>& left_condition,
            const std::shared_ptr<const dlplan::policy::GreaterNumericalCondition>& right_condition) const;
    };

    template<>
    struct less<std::shared_ptr<const dlplan::policy::EqualNumericalCondition>>
    {
        bool operator()(
            const std::shared_ptr<const dlplan::policy::EqualNumericalCondition>& left_condition,
            const std::shared_ptr<const dlplan::policy::EqualNumericalCondition>& right_condition) const;
    };

    template<>
    struct less<std::shared_ptr<const dlplan::policy::GreaterConceptCondition>>
    {
        bool operator()(
            const std::shared_ptr<const dlplan::policy::GreaterConceptCondition>& left_condition,
            const std::shared_ptr<const dlplan::policy::GreaterConceptCondition>& right_condition) const;
    };

    template<>
    struct less<std::shared_ptr<const dlplan::policy::EqualConceptCondition>>
    {
        bool operator()(
            const std::shared_ptr<const dlplan::policy::EqualConceptCondition>& left_condition,
            const std::shared_ptr<const dlplan::policy::EqualConceptCondition>& right_condition) const;
    };

    template<>
    struct hash<dlplan::policy::PositiveBooleanCondition>
    {
        std::size_t operator()(const dlplan::policy::PositiveBooleanCondition& condition) const;
    };

    template<>
    struct hash<dlplan::policy::NegativeBooleanCondition>
    {
        std::size_t operator()(const dlplan::policy::NegativeBooleanCondition& condition) const;
    };

    template<>
    struct hash<dlplan::policy::GreaterNumericalCondition>
    {
        std::size_t operator()(const dlplan::policy::GreaterNumericalCondition& condition) const;
    };

    template<>
    struct hash<dlplan::policy::EqualNumericalCondition>
    {
        std::size_t operator()(const dlplan::policy::EqualNumericalCondition& condition) const;
    };

    template<>
    struct hash<dlplan::policy::GreaterConceptCondition>
    {
        std::size_t operator()(const dlplan::policy::GreaterConceptCondition& condition) const;
    };

    template<>
    struct hash<dlplan::policy::EqualConceptCondition>
    {
        std::size_t operator()(const dlplan::policy::EqualConceptCondition& condition) const;
    };
}

#endif