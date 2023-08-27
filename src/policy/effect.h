#ifndef DLPLAN_SRC_POLICY_EFFECT_H_
#define DLPLAN_SRC_POLICY_EFFECT_H_

#include "../../include/dlplan/policy.h"

#include <string>
#include <memory>


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

// Forward declare the serialize function template in boost::serialization namespace
namespace boost::serialization {
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
protected:
    std::shared_ptr<const core::Boolean> m_boolean;

protected:
    BooleanEffect(std::shared_ptr<const core::Boolean> boolean, EffectIndex index);

    int compute_evaluate_time_score() const override;

    std::shared_ptr<const core::Boolean> get_boolean() const override;
    std::shared_ptr<const core::Numerical> get_numerical() const override;
};


class NumericalEffect : public BaseEffect {
protected:
    std::shared_ptr<const core::Numerical> m_numerical;

protected:
    NumericalEffect(std::shared_ptr<const core::Numerical> numerical, EffectIndex index);

    int compute_evaluate_time_score() const override;

    std::shared_ptr<const core::Boolean> get_boolean() const override;
    std::shared_ptr<const core::Numerical> get_numerical() const override;
};


class PositiveBooleanEffect : public BooleanEffect {
private:
    template<typename Archive>
    friend void boost::serialization::serialize(Archive& ar, PositiveBooleanEffect& t, const unsigned int version);
    template<class Archive>
    friend void boost::serialization::save_construct_data(Archive& ar, const PositiveBooleanEffect* t, const unsigned int version);
    template<class Archive>
    friend void boost::serialization::load_construct_data(Archive& ar, PositiveBooleanEffect* t, const unsigned int version);

public:
    PositiveBooleanEffect(std::shared_ptr<const core::Boolean> boolean, EffectIndex index);

    bool evaluate(const core::State& source_state, const core::State& target_state) const override;
    bool evaluate(const core::State& source_state, const core::State& target_state, core::DenotationsCaches& caches) const override;

    std::string compute_repr() const override;
    std::string str() const override;
};

class NegativeBooleanEffect : public BooleanEffect {
private:
    template<typename Archive>
    friend void boost::serialization::serialize(Archive& ar, NegativeBooleanEffect& t, const unsigned int version);
    template<class Archive>
    friend void boost::serialization::save_construct_data(Archive& ar, const NegativeBooleanEffect* t, const unsigned int version);
    template<class Archive>
    friend void boost::serialization::load_construct_data(Archive& ar, NegativeBooleanEffect* t, const unsigned int version);

public:
    NegativeBooleanEffect(std::shared_ptr<const core::Boolean> boolean, EffectIndex index);

    bool evaluate(const core::State& source_state, const core::State& target_state) const override;
    bool evaluate(const core::State& source_state, const core::State& target_state, core::DenotationsCaches& caches) const override;

    std::string compute_repr() const override;
    std::string str() const override;
};

class UnchangedBooleanEffect : public BooleanEffect {
private:
    template<typename Archive>
    friend void boost::serialization::serialize(Archive& ar, UnchangedBooleanEffect& t, const unsigned int version);
    template<class Archive>
    friend void boost::serialization::save_construct_data(Archive& ar, const UnchangedBooleanEffect* t, const unsigned int version);
    template<class Archive>
    friend void boost::serialization::load_construct_data(Archive& ar, UnchangedBooleanEffect* t, const unsigned int version);

public:
    UnchangedBooleanEffect(std::shared_ptr<const core::Boolean> boolean, EffectIndex index);

    bool evaluate(const core::State& source_state, const core::State& target_state) const override;
    bool evaluate(const core::State& source_state, const core::State& target_state, core::DenotationsCaches& caches) const override;

    std::string compute_repr() const override;
    std::string str() const override;
};

class IncrementNumericalEffect : public NumericalEffect {
private:
    template<typename Archive>
    friend void boost::serialization::serialize(Archive& ar, IncrementNumericalEffect& t, const unsigned int version);
    template<class Archive>
    friend void boost::serialization::save_construct_data(Archive& ar, const IncrementNumericalEffect* t, const unsigned int version);
    template<class Archive>
    friend void boost::serialization::load_construct_data(Archive& ar, IncrementNumericalEffect* t, const unsigned int version);

public:
    IncrementNumericalEffect(std::shared_ptr<const core::Numerical> numerical, EffectIndex index);

    bool evaluate(const core::State& source_state, const core::State& target_state) const override;
    bool evaluate(const core::State& source_state, const core::State& target_state, core::DenotationsCaches& caches) const override;

    std::string compute_repr() const override;
    std::string str() const override;
};

class DecrementNumericalEffect : public NumericalEffect {
private:
    template<typename Archive>
    friend void boost::serialization::serialize(Archive& ar, DecrementNumericalEffect& t, const unsigned int version);
    template<class Archive>
    friend void boost::serialization::save_construct_data(Archive& ar, const DecrementNumericalEffect* t, const unsigned int version);
    template<class Archive>
    friend void boost::serialization::load_construct_data(Archive& ar, DecrementNumericalEffect* t, const unsigned int version);

public:
    DecrementNumericalEffect(std::shared_ptr<const core::Numerical> numerical, EffectIndex index);

    bool evaluate(const core::State& source_state, const core::State& target_state) const override;
    bool evaluate(const core::State& source_state, const core::State& target_state, core::DenotationsCaches& caches) const override;

    std::string compute_repr() const override;
    std::string str() const override;
};

class UnchangedNumericalEffect : public NumericalEffect {
private:
    template<typename Archive>
    friend void boost::serialization::serialize(Archive& ar, UnchangedNumericalEffect& t, const unsigned int version);
    template<class Archive>
    friend void boost::serialization::save_construct_data(Archive& ar, const UnchangedNumericalEffect* t, const unsigned int version);
    template<class Archive>
    friend void boost::serialization::load_construct_data(Archive& ar, UnchangedNumericalEffect* t, const unsigned int version);

public:
    UnchangedNumericalEffect(std::shared_ptr<const core::Numerical> numerical, EffectIndex index);

    bool evaluate(const core::State& source_state, const core::State& target_state) const override;
    bool evaluate(const core::State& source_state, const core::State& target_state, core::DenotationsCaches& caches) const override;

    std::string compute_repr() const override;
    std::string str() const override;
};

}

#endif