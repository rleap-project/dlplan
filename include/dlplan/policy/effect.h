#ifndef DLPLAN_INCLUDE_DLPLAN_POLICY_EFFECT_H_
#define DLPLAN_INCLUDE_DLPLAN_POLICY_EFFECT_H_

#include "../policy.h"

#include <utility>
#include <string>
#include <memory>


namespace dlplan {
template<typename... Ts>
class ReferenceCountedObjectFactory;
}


namespace dlplan::policy {
template<typename Element>
class NamedElementEffect : public BaseEffect {
protected:
    std::shared_ptr<const Element> m_named_element;

    NamedElementEffect(int identifier, const std::shared_ptr<const Element>& named_element)
        : BaseEffect(identifier), m_named_element(named_element) { }

    template<typename... Ts>
    friend class dlplan::ReferenceCountedObjectFactory;

public:
    virtual ~NamedElementEffect() = default;

    int compute_evaluate_time_score() const override {
        return m_named_element->compute_evaluate_time_score();
    }

    size_t hash_impl() const override {
        return hash_combine(m_named_element);
    }

    std::shared_ptr<const Element> get_named_element() const { return m_named_element; }
};


class PositiveBooleanEffect : public NamedElementEffect<NamedBoolean>, public std::enable_shared_from_this<PositiveBooleanEffect> {
private:
    PositiveBooleanEffect(int identifier, std::shared_ptr<const NamedBoolean> boolean);

    template<typename... Ts>
    friend class dlplan::ReferenceCountedObjectFactory;

public:
    bool are_equal_impl(const BaseEffect& other) const override;
    void str_impl(std::stringstream& out) const override;

    bool evaluate(const core::State& source_state, const core::State& target_state) const override;
    bool evaluate(const core::State& source_state, const core::State& target_state, core::DenotationsCaches& caches) const override;
    void accept(BaseEffectVisitor& visitor) const override;
};

class NegativeBooleanEffect : public NamedElementEffect<NamedBoolean>, public std::enable_shared_from_this<NegativeBooleanEffect> {
private:
    NegativeBooleanEffect(int identifier, std::shared_ptr<const NamedBoolean> boolean);

    template<typename... Ts>
    friend class dlplan::ReferenceCountedObjectFactory;

public:
    bool are_equal_impl(const BaseEffect& other) const override;
    void str_impl(std::stringstream& out) const override;

    bool evaluate(const core::State& source_state, const core::State& target_state) const override;
    bool evaluate(const core::State& source_state, const core::State& target_state, core::DenotationsCaches& caches) const override;
    void accept(BaseEffectVisitor& visitor) const override;
};

class UnchangedBooleanEffect : public NamedElementEffect<NamedBoolean>, public std::enable_shared_from_this<UnchangedBooleanEffect> {
private:
    UnchangedBooleanEffect(int identifier, std::shared_ptr<const NamedBoolean> boolean);

    template<typename... Ts>
    friend class dlplan::ReferenceCountedObjectFactory;

public:
    bool are_equal_impl(const BaseEffect& other) const override;
    void str_impl(std::stringstream& out) const override;

    bool evaluate(const core::State& source_state, const core::State& target_state) const override;
    bool evaluate(const core::State& source_state, const core::State& target_state, core::DenotationsCaches& caches) const override;
    void accept(BaseEffectVisitor& visitor) const override;
};

class IncrementNumericalEffect : public NamedElementEffect<NamedNumerical>, public std::enable_shared_from_this<IncrementNumericalEffect> {
private:
    IncrementNumericalEffect(int identifier, std::shared_ptr<const NamedNumerical> numerical);

    template<typename... Ts>
    friend class dlplan::ReferenceCountedObjectFactory;

public:
    bool are_equal_impl(const BaseEffect& other) const override;
    void str_impl(std::stringstream& out) const override;

    bool evaluate(const core::State& source_state, const core::State& target_state) const override;
    bool evaluate(const core::State& source_state, const core::State& target_state, core::DenotationsCaches& caches) const override;
    void accept(BaseEffectVisitor& visitor) const override;
};

class IncrementOrUnchangedNumericalEffect : public NamedElementEffect<NamedNumerical>, public std::enable_shared_from_this<IncrementOrUnchangedNumericalEffect> {
private:
    IncrementOrUnchangedNumericalEffect(int identifier, std::shared_ptr<const NamedNumerical> numerical);

    template<typename... Ts>
    friend class dlplan::ReferenceCountedObjectFactory;

public:
    bool are_equal_impl(const BaseEffect& other) const override;
    void str_impl(std::stringstream& out) const override;

    bool evaluate(const core::State& source_state, const core::State& target_state) const override;
    bool evaluate(const core::State& source_state, const core::State& target_state, core::DenotationsCaches& caches) const override;
    void accept(BaseEffectVisitor& visitor) const override;
};

class DecrementNumericalEffect : public NamedElementEffect<NamedNumerical>, public std::enable_shared_from_this<DecrementNumericalEffect> {
private:
    DecrementNumericalEffect(int identifier, std::shared_ptr<const NamedNumerical> numerical);

    template<typename... Ts>
    friend class dlplan::ReferenceCountedObjectFactory;

public:
    bool are_equal_impl(const BaseEffect& other) const override;
    void str_impl(std::stringstream& out) const override;

    bool evaluate(const core::State& source_state, const core::State& target_state) const override;
    bool evaluate(const core::State& source_state, const core::State& target_state, core::DenotationsCaches& caches) const override;
    void accept(BaseEffectVisitor& visitor) const override;
};

class DecrementOrUnchangedNumericalEffect : public NamedElementEffect<NamedNumerical>, public std::enable_shared_from_this<DecrementOrUnchangedNumericalEffect> {
private:
    DecrementOrUnchangedNumericalEffect(int identifier, std::shared_ptr<const NamedNumerical> numerical);

    template<typename... Ts>
    friend class dlplan::ReferenceCountedObjectFactory;

public:
    bool are_equal_impl(const BaseEffect& other) const override;
    void str_impl(std::stringstream& out) const override;

    bool evaluate(const core::State& source_state, const core::State& target_state) const override;
    bool evaluate(const core::State& source_state, const core::State& target_state, core::DenotationsCaches& caches) const override;
    void accept(BaseEffectVisitor& visitor) const override;
};

class UnchangedNumericalEffect : public NamedElementEffect<NamedNumerical>, public std::enable_shared_from_this<UnchangedNumericalEffect> {
private:
    UnchangedNumericalEffect(int identifier, std::shared_ptr<const NamedNumerical> numerical);

    template<typename... Ts>
    friend class dlplan::ReferenceCountedObjectFactory;

public:
    bool are_equal_impl(const BaseEffect& other) const override;
    void str_impl(std::stringstream& out) const override;

    bool evaluate(const core::State& source_state, const core::State& target_state) const override;
    bool evaluate(const core::State& source_state, const core::State& target_state, core::DenotationsCaches& caches) const override;
    void accept(BaseEffectVisitor& visitor) const override;
};

class GreaterNumericalEffect : public NamedElementEffect<NamedNumerical>, public std::enable_shared_from_this<GreaterNumericalEffect> {
private:
    GreaterNumericalEffect(int identifier, std::shared_ptr<const NamedNumerical> numerical);

    template<typename... Ts>
    friend class dlplan::ReferenceCountedObjectFactory;

public:
    bool are_equal_impl(const BaseEffect& other) const override;
    void str_impl(std::stringstream& out) const override;

    bool evaluate(const core::State& source_state, const core::State& target_state) const override;
    bool evaluate(const core::State& source_state, const core::State& target_state, core::DenotationsCaches& caches) const override;
    void accept(BaseEffectVisitor& visitor) const override;
};

class EqualNumericalEffect : public NamedElementEffect<NamedNumerical>, public std::enable_shared_from_this<EqualNumericalEffect> {
private:
    EqualNumericalEffect(int identifier, std::shared_ptr<const NamedNumerical> numerical);

    template<typename... Ts>
    friend class dlplan::ReferenceCountedObjectFactory;

public:
    bool are_equal_impl(const BaseEffect& other) const override;
    void str_impl(std::stringstream& out) const override;

    bool evaluate(const core::State& source_state, const core::State& target_state) const override;
    bool evaluate(const core::State& source_state, const core::State& target_state, core::DenotationsCaches& caches) const override;
    void accept(BaseEffectVisitor& visitor) const override;

};

class IncrementConceptEffect : public NamedElementEffect<NamedConcept>, public std::enable_shared_from_this<IncrementConceptEffect> {
private:
    IncrementConceptEffect(int identifier, std::shared_ptr<const NamedConcept> concept_);

    template<typename... Ts>
    friend class dlplan::ReferenceCountedObjectFactory;

public:
    bool are_equal_impl(const BaseEffect& other) const override;
    void str_impl(std::stringstream& out) const override;

    bool evaluate(const core::State& source_state, const core::State& target_state) const override;
    bool evaluate(const core::State& source_state, const core::State& target_state, core::DenotationsCaches& caches) const override;
    void accept(BaseEffectVisitor& visitor) const override;
};

class DecrementConceptEffect : public NamedElementEffect<NamedConcept>, public std::enable_shared_from_this<DecrementConceptEffect> {
private:
    DecrementConceptEffect(int identifier, std::shared_ptr<const NamedConcept> concept_);

    template<typename... Ts>
    friend class dlplan::ReferenceCountedObjectFactory;

public:
    bool are_equal_impl(const BaseEffect& other) const override;
    void str_impl(std::stringstream& out) const override;

    bool evaluate(const core::State& source_state, const core::State& target_state) const override;
    bool evaluate(const core::State& source_state, const core::State& target_state, core::DenotationsCaches& caches) const override;
    void accept(BaseEffectVisitor& visitor) const override;
};

class UnchangedConceptEffect : public NamedElementEffect<NamedConcept>, public std::enable_shared_from_this<UnchangedConceptEffect> {
private:
    UnchangedConceptEffect(int identifier, std::shared_ptr<const NamedConcept> concept_);

    template<typename... Ts>
    friend class dlplan::ReferenceCountedObjectFactory;

public:
    bool are_equal_impl(const BaseEffect& other) const override;
    void str_impl(std::stringstream& out) const override;

    bool evaluate(const core::State& source_state, const core::State& target_state) const override;
    bool evaluate(const core::State& source_state, const core::State& target_state, core::DenotationsCaches& caches) const override;
    void accept(BaseEffectVisitor& visitor) const override;

};

class GreaterConceptEffect : public NamedElementEffect<NamedConcept>, public std::enable_shared_from_this<GreaterConceptEffect> {
private:
    GreaterConceptEffect(int identifier, std::shared_ptr<const NamedConcept> concept_);

    template<typename... Ts>
    friend class dlplan::ReferenceCountedObjectFactory;

public:
    bool are_equal_impl(const BaseEffect& other) const override;
    void str_impl(std::stringstream& out) const override;

    bool evaluate(const core::State& source_state, const core::State& target_state) const override;
    bool evaluate(const core::State& source_state, const core::State& target_state, core::DenotationsCaches& caches) const override;
    void accept(BaseEffectVisitor& visitor) const override;
};

class EqualConceptEffect : public NamedElementEffect<NamedConcept>, public std::enable_shared_from_this<EqualConceptEffect> {
private:
    EqualConceptEffect(int identifier, std::shared_ptr<const NamedConcept> concept_);

    template<typename... Ts>
    friend class dlplan::ReferenceCountedObjectFactory;

public:
    bool are_equal_impl(const BaseEffect& other) const override;
    void str_impl(std::stringstream& out) const override;

    bool evaluate(const core::State& source_state, const core::State& target_state) const override;
    bool evaluate(const core::State& source_state, const core::State& target_state, core::DenotationsCaches& caches) const override;
    void accept(BaseEffectVisitor& visitor) const override;

};


/// @brief Defines an interface for visiting effects.
class BaseEffectVisitor {
public:
    virtual void visit(const std::shared_ptr<const PositiveBooleanEffect>& effect) = 0;
    virtual void visit(const std::shared_ptr<const NegativeBooleanEffect>& effect) = 0;
    virtual void visit(const std::shared_ptr<const UnchangedBooleanEffect>& effect) = 0;
    virtual void visit(const std::shared_ptr<const IncrementNumericalEffect>& effect) = 0;
    virtual void visit(const std::shared_ptr<const IncrementOrUnchangedNumericalEffect>& effect) = 0;
    virtual void visit(const std::shared_ptr<const DecrementNumericalEffect>& effect) = 0;
    virtual void visit(const std::shared_ptr<const DecrementOrUnchangedNumericalEffect>& effect) = 0;
    virtual void visit(const std::shared_ptr<const UnchangedNumericalEffect>& effect) = 0;
    virtual void visit(const std::shared_ptr<const GreaterNumericalEffect>& effect) = 0;
    virtual void visit(const std::shared_ptr<const EqualNumericalEffect>& effect) = 0;
    virtual void visit(const std::shared_ptr<const IncrementConceptEffect>& effect) = 0;
    virtual void visit(const std::shared_ptr<const DecrementConceptEffect>& effect) = 0;
    virtual void visit(const std::shared_ptr<const UnchangedConceptEffect>& effect) = 0;
    virtual void visit(const std::shared_ptr<const GreaterConceptEffect>& effect) = 0;
    virtual void visit(const std::shared_ptr<const EqualConceptEffect>& effect) = 0;
};

}


namespace std {
    template<>
    struct less<std::shared_ptr<const dlplan::policy::PositiveBooleanEffect>>
    {
        bool operator()(
            const std::shared_ptr<const dlplan::policy::PositiveBooleanEffect>& left_effect,
            const std::shared_ptr<const dlplan::policy::PositiveBooleanEffect>& right_effect) const;
    };

    template<>
    struct less<std::shared_ptr<const dlplan::policy::NegativeBooleanEffect>>
    {
        bool operator()(
            const std::shared_ptr<const dlplan::policy::NegativeBooleanEffect>& left_effect,
            const std::shared_ptr<const dlplan::policy::NegativeBooleanEffect>& right_effect) const;
    };

    template<>
    struct less<std::shared_ptr<const dlplan::policy::UnchangedBooleanEffect>>
    {
        bool operator()(
            const std::shared_ptr<const dlplan::policy::UnchangedBooleanEffect>& left_effect,
            const std::shared_ptr<const dlplan::policy::UnchangedBooleanEffect>& right_effect) const;
    };

    template<>
    struct less<std::shared_ptr<const dlplan::policy::IncrementNumericalEffect>>
    {
        bool operator()(
            const std::shared_ptr<const dlplan::policy::IncrementNumericalEffect>& left_effect,
            const std::shared_ptr<const dlplan::policy::IncrementNumericalEffect>& right_effect) const;
    };

    template<>
    struct less<std::shared_ptr<const dlplan::policy::IncrementOrUnchangedNumericalEffect>>
    {
        bool operator()(
            const std::shared_ptr<const dlplan::policy::IncrementOrUnchangedNumericalEffect>& left_effect,
            const std::shared_ptr<const dlplan::policy::IncrementOrUnchangedNumericalEffect>& right_effect) const;
    };

    template<>
    struct less<std::shared_ptr<const dlplan::policy::DecrementNumericalEffect>>
    {
        bool operator()(
            const std::shared_ptr<const dlplan::policy::DecrementNumericalEffect>& left_effect,
            const std::shared_ptr<const dlplan::policy::DecrementNumericalEffect>& right_effect) const;
    };

    template<>
    struct less<std::shared_ptr<const dlplan::policy::DecrementOrUnchangedNumericalEffect>>
    {
        bool operator()(
            const std::shared_ptr<const dlplan::policy::DecrementOrUnchangedNumericalEffect>& left_effect,
            const std::shared_ptr<const dlplan::policy::DecrementOrUnchangedNumericalEffect>& right_effect) const;
    };

    template<>
    struct less<std::shared_ptr<const dlplan::policy::UnchangedNumericalEffect>>
    {
        bool operator()(
            const std::shared_ptr<const dlplan::policy::UnchangedNumericalEffect>& left_effect,
            const std::shared_ptr<const dlplan::policy::UnchangedNumericalEffect>& right_effect) const;
    };

    template<>
    struct less<std::shared_ptr<const dlplan::policy::GreaterNumericalEffect>>
    {
        bool operator()(
            const std::shared_ptr<const dlplan::policy::GreaterNumericalEffect>& left_effect,
            const std::shared_ptr<const dlplan::policy::GreaterNumericalEffect>& right_effect) const;
    };

    template<>
    struct less<std::shared_ptr<const dlplan::policy::EqualNumericalEffect>>
    {
        bool operator()(
            const std::shared_ptr<const dlplan::policy::EqualNumericalEffect>& left_effect,
            const std::shared_ptr<const dlplan::policy::EqualNumericalEffect>& right_effect) const;
    };

    template<>
    struct less<std::shared_ptr<const dlplan::policy::IncrementConceptEffect>>
    {
        bool operator()(
            const std::shared_ptr<const dlplan::policy::IncrementConceptEffect>& left_effect,
            const std::shared_ptr<const dlplan::policy::IncrementConceptEffect>& right_effect) const;
    };

    template<>
    struct less<std::shared_ptr<const dlplan::policy::DecrementConceptEffect>>
    {
        bool operator()(
            const std::shared_ptr<const dlplan::policy::DecrementConceptEffect>& left_effect,
            const std::shared_ptr<const dlplan::policy::DecrementConceptEffect>& right_effect) const;
    };

    template<>
    struct less<std::shared_ptr<const dlplan::policy::UnchangedConceptEffect>>
    {
        bool operator()(
            const std::shared_ptr<const dlplan::policy::UnchangedConceptEffect>& left_effect,
            const std::shared_ptr<const dlplan::policy::UnchangedConceptEffect>& right_effect) const;
    };

    template<>
    struct less<std::shared_ptr<const dlplan::policy::GreaterConceptEffect>>
    {
        bool operator()(
            const std::shared_ptr<const dlplan::policy::GreaterConceptEffect>& left_effect,
            const std::shared_ptr<const dlplan::policy::GreaterConceptEffect>& right_effect) const;
    };

    template<>
    struct less<std::shared_ptr<const dlplan::policy::EqualConceptEffect>>
    {
        bool operator()(
            const std::shared_ptr<const dlplan::policy::EqualConceptEffect>& left_effect,
            const std::shared_ptr<const dlplan::policy::EqualConceptEffect>& right_effect) const;
    };

    template<>
    struct hash<dlplan::policy::PositiveBooleanEffect>
    {
        std::size_t operator()(const dlplan::policy::PositiveBooleanEffect& effect) const;
    };

    template<>
    struct hash<dlplan::policy::NegativeBooleanEffect>
    {
        std::size_t operator()(const dlplan::policy::NegativeBooleanEffect& effect) const;
    };

    template<>
    struct hash<dlplan::policy::UnchangedBooleanEffect>
    {
        std::size_t operator()(const dlplan::policy::UnchangedBooleanEffect& effect) const;
    };

    template<>
    struct hash<dlplan::policy::IncrementNumericalEffect>
    {
        std::size_t operator()(const dlplan::policy::IncrementNumericalEffect& effect) const;
    };

    template<>
    struct hash<dlplan::policy::IncrementOrUnchangedNumericalEffect>
    {
        std::size_t operator()(const dlplan::policy::IncrementOrUnchangedNumericalEffect& effect) const;
    };

    template<>
    struct hash<dlplan::policy::DecrementNumericalEffect>
    {
        std::size_t operator()(const dlplan::policy::DecrementNumericalEffect& effect) const;
    };

    template<>
    struct hash<dlplan::policy::DecrementOrUnchangedNumericalEffect>
    {
        std::size_t operator()(const dlplan::policy::DecrementOrUnchangedNumericalEffect& effect) const;
    };

    template<>
    struct hash<dlplan::policy::UnchangedNumericalEffect>
    {
        std::size_t operator()(const dlplan::policy::UnchangedNumericalEffect& effect) const;
    };

    template<>
    struct hash<dlplan::policy::GreaterNumericalEffect>
    {
        std::size_t operator()(const dlplan::policy::GreaterNumericalEffect& effect) const;
    };

    template<>
    struct hash<dlplan::policy::EqualNumericalEffect>
    {
        std::size_t operator()(const dlplan::policy::EqualNumericalEffect& effect) const;
    };

    template<>
    struct hash<dlplan::policy::IncrementConceptEffect>
    {
        std::size_t operator()(const dlplan::policy::IncrementConceptEffect& effect) const;
    };

    template<>
    struct hash<dlplan::policy::DecrementConceptEffect>
    {
        std::size_t operator()(const dlplan::policy::DecrementConceptEffect& effect) const;
    };

    template<>
    struct hash<dlplan::policy::UnchangedConceptEffect>
    {
        std::size_t operator()(const dlplan::policy::UnchangedConceptEffect& effect) const;
    };

    template<>
    struct hash<dlplan::policy::GreaterConceptEffect>
    {
        std::size_t operator()(const dlplan::policy::GreaterConceptEffect& effect) const;
    };

    template<>
    struct hash<dlplan::policy::EqualConceptEffect>
    {
        std::size_t operator()(const dlplan::policy::EqualConceptEffect& effect) const;
    };
}


#endif