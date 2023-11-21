#ifndef DLPLAN_SRC_POLICY_EFFECT_H_
#define DLPLAN_SRC_POLICY_EFFECT_H_

#include "../../include/dlplan/policy.h"

#include <boost/serialization/export.hpp>

#include <string>
#include <memory>


namespace dlplan::utils {
template<typename... Ts>
class ReferenceCountedObjectFactory;
}


namespace dlplan::policy {
class BooleanEffect;
class NumericalEffect;
class PositiveBooleanEffect;
class NegativeBooleanEffect;
class UnchangedBooleanEffect;
class IncrementNumericalEffect;
class DecrementNumericalEffect;
class UnchangedNumericalEffect;
}


namespace boost::serialization {
    template <typename Archive>
    void serialize(Archive& ar, dlplan::policy::BooleanEffect& effect, const unsigned int version);
    template<class Archive>
    void save_construct_data(Archive& ar, const dlplan::policy::BooleanEffect* t, const unsigned int version);
    template<class Archive>
    void load_construct_data(Archive& ar, dlplan::policy::BooleanEffect* t, const unsigned int version);

    template <typename Archive>
    void serialize(Archive& ar, dlplan::policy::NumericalEffect& effect, const unsigned int version);
    template<class Archive>
    void save_construct_data(Archive& ar, const dlplan::policy::NumericalEffect* t, const unsigned int version);
    template<class Archive>
    void load_construct_data(Archive& ar, dlplan::policy::NumericalEffect* t, const unsigned int version);

    template <typename Archive>
    void serialize(Archive& ar, dlplan::policy::PositiveBooleanEffect& effect, const unsigned int version);
    template<class Archive>
    void save_construct_data(Archive& ar, const dlplan::policy::PositiveBooleanEffect* t, const unsigned int version);
    template<class Archive>
    void load_construct_data(Archive& ar, dlplan::policy::PositiveBooleanEffect* t, const unsigned int version);

    template <typename Archive>
    void serialize(Archive& ar, dlplan::policy::NegativeBooleanEffect& effect, const unsigned int version);
    template<class Archive>
    void save_construct_data(Archive& ar, const dlplan::policy::NegativeBooleanEffect* t, const unsigned int version);
    template<class Archive>
    void load_construct_data(Archive& ar, dlplan::policy::NegativeBooleanEffect* t, const unsigned int version);

    template <typename Archive>
    void serialize(Archive& ar, dlplan::policy::UnchangedBooleanEffect& effect, const unsigned int version);
    template<class Archive>
    void save_construct_data(Archive& ar, const dlplan::policy::UnchangedBooleanEffect* t, const unsigned int version);
    template<class Archive>
    void load_construct_data(Archive& ar, dlplan::policy::UnchangedBooleanEffect* t, const unsigned int version);

    template <typename Archive>
    void serialize(Archive& ar, dlplan::policy::IncrementNumericalEffect& effect, const unsigned int version);
    template<class Archive>
    void save_construct_data(Archive& ar, const dlplan::policy::IncrementNumericalEffect* t, const unsigned int version);
    template<class Archive>
    void load_construct_data(Archive& ar, dlplan::policy::IncrementNumericalEffect* t, const unsigned int version);

    template <typename Archive>
    void serialize(Archive& ar, dlplan::policy::DecrementNumericalEffect& effect, const unsigned int version);
    template<class Archive>
    void save_construct_data(Archive& ar, const dlplan::policy::DecrementNumericalEffect* t, const unsigned int version);
    template<class Archive>
    void load_construct_data(Archive& ar, dlplan::policy::DecrementNumericalEffect* t, const unsigned int version);

    template <typename Archive>
    void serialize(Archive& ar, dlplan::policy::UnchangedNumericalEffect& effect, const unsigned int version);
    template<class Archive>
    void save_construct_data(Archive& ar, const dlplan::policy::UnchangedNumericalEffect* t, const unsigned int version);
    template<class Archive>
    void load_construct_data(Archive& ar, dlplan::policy::UnchangedNumericalEffect* t, const unsigned int version);
}


namespace dlplan::policy {
class BooleanEffect : public BaseEffect {
private:
    template<typename Archive>
    friend void boost::serialization::serialize(Archive& ar, BooleanEffect& t, const unsigned int version);
    template<class Archive>
    friend void boost::serialization::save_construct_data(Archive& ar, const BooleanEffect* t, const unsigned int version);
    template<class Archive>
    friend void boost::serialization::load_construct_data(Archive& ar, BooleanEffect* t, const unsigned int version);

protected:
    std::shared_ptr<const NamedBoolean> m_boolean;

    BooleanEffect(int identifier, std::shared_ptr<const NamedBoolean> boolean);

    int compute_evaluate_time_score() const override;

    std::shared_ptr<const NamedBoolean> get_boolean() const override;
    std::shared_ptr<const NamedNumerical> get_numerical() const override;
};


class NumericalEffect : public BaseEffect {
private:
    template<typename Archive>
    friend void boost::serialization::serialize(Archive& ar, NumericalEffect& t, const unsigned int version);
    template<class Archive>
    friend void boost::serialization::save_construct_data(Archive& ar, const NumericalEffect* t, const unsigned int version);
    template<class Archive>
    friend void boost::serialization::load_construct_data(Archive& ar, NumericalEffect* t, const unsigned int version);

protected:
    std::shared_ptr<const NamedNumerical> m_numerical;

    NumericalEffect(int identifier, std::shared_ptr<const NamedNumerical> numerical);

    int compute_evaluate_time_score() const override;

    std::shared_ptr<const NamedBoolean> get_boolean() const override;
    std::shared_ptr<const NamedNumerical> get_numerical() const override;
};


class PositiveBooleanEffect : public BooleanEffect {
private:
    PositiveBooleanEffect(int identifier, std::shared_ptr<const NamedBoolean> boolean);

    template<typename Archive>
    friend void boost::serialization::serialize(Archive& ar, PositiveBooleanEffect& t, const unsigned int version);
    template<class Archive>
    friend void boost::serialization::save_construct_data(Archive& ar, const PositiveBooleanEffect* t, const unsigned int version);
    template<class Archive>
    friend void boost::serialization::load_construct_data(Archive& ar, PositiveBooleanEffect* t, const unsigned int version);
    template<typename... Ts>
    friend class dlplan::utils::ReferenceCountedObjectFactory;

public:
    bool operator==(const BaseEffect& other) const override;

    size_t hash() const override;

    bool evaluate(const core::State& source_state, const core::State& target_state) const override;
    bool evaluate(const core::State& source_state, const core::State& target_state, core::DenotationsCaches& caches) const override;

    std::string compute_repr() const override;
    std::string str() const override;
};

class NegativeBooleanEffect : public BooleanEffect {
private:
    NegativeBooleanEffect(int identifier, std::shared_ptr<const NamedBoolean> boolean);

    template<typename Archive>
    friend void boost::serialization::serialize(Archive& ar, NegativeBooleanEffect& t, const unsigned int version);
    template<class Archive>
    friend void boost::serialization::save_construct_data(Archive& ar, const NegativeBooleanEffect* t, const unsigned int version);
    template<class Archive>
    friend void boost::serialization::load_construct_data(Archive& ar, NegativeBooleanEffect* t, const unsigned int version);
    template<typename... Ts>
    friend class dlplan::utils::ReferenceCountedObjectFactory;

public:
    bool operator==(const BaseEffect& other) const override;

    size_t hash() const override;

    bool evaluate(const core::State& source_state, const core::State& target_state) const override;
    bool evaluate(const core::State& source_state, const core::State& target_state, core::DenotationsCaches& caches) const override;

    std::string compute_repr() const override;
    std::string str() const override;
};

class UnchangedBooleanEffect : public BooleanEffect {
private:
    UnchangedBooleanEffect(int identifier, std::shared_ptr<const NamedBoolean> boolean);

    template<typename Archive>
    friend void boost::serialization::serialize(Archive& ar, UnchangedBooleanEffect& t, const unsigned int version);
    template<class Archive>
    friend void boost::serialization::save_construct_data(Archive& ar, const UnchangedBooleanEffect* t, const unsigned int version);
    template<class Archive>
    friend void boost::serialization::load_construct_data(Archive& ar, UnchangedBooleanEffect* t, const unsigned int version);
    template<typename... Ts>
    friend class dlplan::utils::ReferenceCountedObjectFactory;

public:
    bool operator==(const BaseEffect& other) const override;

    size_t hash() const override;

    bool evaluate(const core::State& source_state, const core::State& target_state) const override;
    bool evaluate(const core::State& source_state, const core::State& target_state, core::DenotationsCaches& caches) const override;

    std::string compute_repr() const override;
    std::string str() const override;
};

class IncrementNumericalEffect : public NumericalEffect {
private:
    IncrementNumericalEffect(int identifier, std::shared_ptr<const NamedNumerical> numerical);

    template<typename Archive>
    friend void boost::serialization::serialize(Archive& ar, IncrementNumericalEffect& t, const unsigned int version);
    template<class Archive>
    friend void boost::serialization::save_construct_data(Archive& ar, const IncrementNumericalEffect* t, const unsigned int version);
    template<class Archive>
    friend void boost::serialization::load_construct_data(Archive& ar, IncrementNumericalEffect* t, const unsigned int version);
    template<typename... Ts>
    friend class dlplan::utils::ReferenceCountedObjectFactory;

public:
    bool operator==(const BaseEffect& other) const override;

    size_t hash() const override;

    bool evaluate(const core::State& source_state, const core::State& target_state) const override;
    bool evaluate(const core::State& source_state, const core::State& target_state, core::DenotationsCaches& caches) const override;

    std::string compute_repr() const override;
    std::string str() const override;
};

class DecrementNumericalEffect : public NumericalEffect {
private:
    DecrementNumericalEffect(int identifier, std::shared_ptr<const NamedNumerical> numerical);

    template<typename Archive>
    friend void boost::serialization::serialize(Archive& ar, DecrementNumericalEffect& t, const unsigned int version);
    template<class Archive>
    friend void boost::serialization::save_construct_data(Archive& ar, const DecrementNumericalEffect* t, const unsigned int version);
    template<class Archive>
    friend void boost::serialization::load_construct_data(Archive& ar, DecrementNumericalEffect* t, const unsigned int version);
    template<typename... Ts>
    friend class dlplan::utils::ReferenceCountedObjectFactory;

public:
    bool operator==(const BaseEffect& other) const override;

    size_t hash() const override;

    bool evaluate(const core::State& source_state, const core::State& target_state) const override;
    bool evaluate(const core::State& source_state, const core::State& target_state, core::DenotationsCaches& caches) const override;

    std::string compute_repr() const override;
    std::string str() const override;
};

class UnchangedNumericalEffect : public NumericalEffect {
private:
    UnchangedNumericalEffect(int identifier, std::shared_ptr<const NamedNumerical> numerical);

    template<typename Archive>
    friend void boost::serialization::serialize(Archive& ar, UnchangedNumericalEffect& t, const unsigned int version);
    template<class Archive>
    friend void boost::serialization::save_construct_data(Archive& ar, const UnchangedNumericalEffect* t, const unsigned int version);
    template<class Archive>
    friend void boost::serialization::load_construct_data(Archive& ar, UnchangedNumericalEffect* t, const unsigned int version);
    template<typename... Ts>
    friend class dlplan::utils::ReferenceCountedObjectFactory;

public:
    bool operator==(const BaseEffect& other) const override;

    size_t hash() const override;

    bool evaluate(const core::State& source_state, const core::State& target_state) const override;
    bool evaluate(const core::State& source_state, const core::State& target_state, core::DenotationsCaches& caches) const override;

    std::string compute_repr() const override;
    std::string str() const override;
};

}

BOOST_CLASS_EXPORT_KEY2(dlplan::policy::PositiveBooleanEffect, "dlplan::policy::PositiveBooleanEffect")
BOOST_CLASS_EXPORT_KEY2(dlplan::policy::NegativeBooleanEffect, "dlplan::policy::NegativeBooleanEffect")
BOOST_CLASS_EXPORT_KEY2(dlplan::policy::UnchangedBooleanEffect, "dlplan::policy::UnchangedBooleanEffect")
BOOST_CLASS_EXPORT_KEY2(dlplan::policy::IncrementNumericalEffect, "dlplan::policy::IncrementNumericalEffect")
BOOST_CLASS_EXPORT_KEY2(dlplan::policy::DecrementNumericalEffect, "dlplan::policy::DecrementNumericalEffect")
BOOST_CLASS_EXPORT_KEY2(dlplan::policy::UnchangedNumericalEffect, "dlplan::policy::UnchangedNumericalEffect")


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
    struct less<std::shared_ptr<const dlplan::policy::DecrementNumericalEffect>>
    {
        bool operator()(
            const std::shared_ptr<const dlplan::policy::DecrementNumericalEffect>& left_effect,
            const std::shared_ptr<const dlplan::policy::DecrementNumericalEffect>& right_effect) const;
    };

    template<>
    struct less<std::shared_ptr<const dlplan::policy::UnchangedNumericalEffect>>
    {
        bool operator()(
            const std::shared_ptr<const dlplan::policy::UnchangedNumericalEffect>& left_effect,
            const std::shared_ptr<const dlplan::policy::UnchangedNumericalEffect>& right_effect) const;
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
    struct hash<dlplan::policy::DecrementNumericalEffect>
    {
        std::size_t operator()(const dlplan::policy::DecrementNumericalEffect& effect) const;
    };

    template<>
    struct hash<dlplan::policy::UnchangedNumericalEffect>
    {
        std::size_t operator()(const dlplan::policy::UnchangedNumericalEffect& effect) const;
    };
}


#endif