#ifndef DLPLAN_SRC_POLICY_CONDITION_H_
#define DLPLAN_SRC_POLICY_CONDITION_H_

#include <string>
#include <memory>

#include <boost/serialization/export.hpp>

#include "include/dlplan/policy.h"

using namespace dlplan;


namespace dlplan::policy {
class BooleanCondition;
class NumericalCondition;
class PositiveBooleanCondition;
class NegativeBooleanCondition;
class EqualNumericalCondition;
class GreaterNumericalCondition;
}


namespace boost::serialization {
    template <typename Archive>
    void serialize(Archive& ar, dlplan::policy::BooleanCondition& t, const unsigned int version);
    template<class Archive>
    void save_construct_data(Archive& ar, const dlplan::policy::BooleanCondition* t, const unsigned int version);
    template<class Archive>
    void load_construct_data(Archive& ar, dlplan::policy::BooleanCondition* t, const unsigned int version);

    template <typename Archive>
    void serialize(Archive& ar, dlplan::policy::NumericalCondition& t, const unsigned int version);
    template<class Archive>
    void save_construct_data(Archive& ar, const dlplan::policy::NumericalCondition* t, const unsigned int version);
    template<class Archive>
    void load_construct_data(Archive& ar, dlplan::policy::NumericalCondition* t, const unsigned int version);

    template <typename Archive>
    void serialize(Archive& ar, dlplan::policy::PositiveBooleanCondition& t, const unsigned int version);
    template<class Archive>
    void save_construct_data(Archive& ar, const dlplan::policy::PositiveBooleanCondition* t, const unsigned int version);
    template<class Archive>
    void load_construct_data(Archive& ar, dlplan::policy::PositiveBooleanCondition* t, const unsigned int version);

    template <typename Archive>
    void serialize(Archive& ar, dlplan::policy::NegativeBooleanCondition& t, const unsigned int version);
    template<class Archive>
    void save_construct_data(Archive& ar, const dlplan::policy::NegativeBooleanCondition* t, const unsigned int version);
    template<class Archive>
    void load_construct_data(Archive& ar, dlplan::policy::NegativeBooleanCondition* t, const unsigned int version);

    template <typename Archive>
    void serialize(Archive& ar, dlplan::policy::EqualNumericalCondition& t, const unsigned int version);
    template<class Archive>
    void save_construct_data(Archive& ar, const dlplan::policy::EqualNumericalCondition* t, const unsigned int version);
    template<class Archive>
    void load_construct_data(Archive& ar, dlplan::policy::EqualNumericalCondition* t, const unsigned int version);

    template <typename Archive>
    void serialize(Archive& ar, dlplan::policy::GreaterNumericalCondition& t, const unsigned int version);
    template<class Archive>
    void save_construct_data(Archive& ar, const dlplan::policy::GreaterNumericalCondition* t, const unsigned int version);
    template<class Archive>
    void load_construct_data(Archive& ar, dlplan::policy::GreaterNumericalCondition* t, const unsigned int version);
}


namespace dlplan::policy {

class BooleanCondition : public BaseCondition {
private:
    template<typename Archive>
    friend void boost::serialization::serialize(Archive& ar, BooleanCondition& t, const unsigned int version);
    template<class Archive>
    friend void boost::serialization::save_construct_data(Archive& ar, const BooleanCondition* t, const unsigned int version);
    template<class Archive>
    friend void boost::serialization::load_construct_data(Archive& ar, BooleanCondition* t, const unsigned int version);

protected:
    const std::shared_ptr<const core::Boolean> m_boolean;

protected:
    BooleanCondition(std::shared_ptr<const core::Boolean> boolean, ConditionIndex index);

    int compute_evaluate_time_score() const override;

    std::shared_ptr<const core::Boolean> get_boolean() const override;

    std::shared_ptr<const core::Numerical> get_numerical() const override;
};


class NumericalCondition : public BaseCondition {
private:
    template<typename Archive>
    friend void boost::serialization::serialize(Archive& ar, NumericalCondition& t, const unsigned int version);
    template<class Archive>
    friend void boost::serialization::save_construct_data(Archive& ar, const NumericalCondition* t, const unsigned int version);
    template<class Archive>
    friend void boost::serialization::load_construct_data(Archive& ar, NumericalCondition* t, const unsigned int version);

protected:
    const std::shared_ptr<const core::Numerical> m_numerical;

protected:
    NumericalCondition(std::shared_ptr<const core::Numerical> numerical, ConditionIndex index);

    int compute_evaluate_time_score() const override;

    std::shared_ptr<const core::Boolean> get_boolean() const override;
    std::shared_ptr<const core::Numerical> get_numerical() const override;
};


class PositiveBooleanCondition : public BooleanCondition {
private:
    template<typename Archive>
    friend void boost::serialization::serialize(Archive& ar, PositiveBooleanCondition& t, const unsigned int version);
    template<class Archive>
    friend void boost::serialization::save_construct_data(Archive& ar, const PositiveBooleanCondition* t, const unsigned int version);
    template<class Archive>
    friend void boost::serialization::load_construct_data(Archive& ar, PositiveBooleanCondition* t, const unsigned int version);

public:
    PositiveBooleanCondition(std::shared_ptr<const core::Boolean> boolean, ConditionIndex index);

    bool evaluate(const core::State& source_state) const override;
    bool evaluate(const core::State& source_state, core::DenotationsCaches& caches) const override;

    std::string compute_repr() const override;
    std::string str() const override;
};

class NegativeBooleanCondition : public BooleanCondition {
private:
    template<typename Archive>
    friend void boost::serialization::serialize(Archive& ar, NegativeBooleanCondition& t, const unsigned int version);
    template<class Archive>
    friend void boost::serialization::save_construct_data(Archive& ar, const NegativeBooleanCondition* t, const unsigned int version);
    template<class Archive>
    friend void boost::serialization::load_construct_data(Archive& ar, NegativeBooleanCondition* t, const unsigned int version);

public:
    NegativeBooleanCondition(std::shared_ptr<const core::Boolean> boolean, ConditionIndex index);

    bool evaluate(const core::State& source_state) const override;
    bool evaluate(const core::State& source_state, core::DenotationsCaches& caches) const override;

    std::string compute_repr() const override;
    std::string str() const override;
};

class EqualNumericalCondition : public NumericalCondition {
private:
    template<typename Archive>
    friend void boost::serialization::serialize(Archive& ar, EqualNumericalCondition& t, const unsigned int version);
    template<class Archive>
    friend void boost::serialization::save_construct_data(Archive& ar, const EqualNumericalCondition* t, const unsigned int version);
    template<class Archive>
    friend void boost::serialization::load_construct_data(Archive& ar, EqualNumericalCondition* t, const unsigned int version);

public:
    EqualNumericalCondition(std::shared_ptr<const core::Numerical> numerical, ConditionIndex index);

    bool evaluate(const core::State& source_state) const override;
    bool evaluate(const core::State& source_state, core::DenotationsCaches& caches) const override;

    std::string compute_repr() const override;
    std::string str() const override;
};

class GreaterNumericalCondition : public NumericalCondition {
private:
    template<typename Archive>
    friend void boost::serialization::serialize(Archive& ar, GreaterNumericalCondition& t, const unsigned int version);
    template<class Archive>
    friend void boost::serialization::save_construct_data(Archive& ar, const GreaterNumericalCondition* t, const unsigned int version);
    template<class Archive>
    friend void boost::serialization::load_construct_data(Archive& ar, GreaterNumericalCondition* t, const unsigned int version);

public:
    GreaterNumericalCondition(std::shared_ptr<const core::Numerical> numerical, ConditionIndex index);

    bool evaluate(const core::State& source_state) const override;
    bool evaluate(const core::State& source_state, core::DenotationsCaches& caches) const override;

    std::string compute_repr() const override;
    std::string str() const override;
};

}

BOOST_CLASS_EXPORT_KEY2(dlplan::policy::PositiveBooleanCondition, "dlplan::policy::PositiveBooleanCondition")
BOOST_CLASS_EXPORT_KEY2(dlplan::policy::NegativeBooleanCondition, "dlplan::policy::NegativeBooleanCondition")
BOOST_CLASS_EXPORT_KEY2(dlplan::policy::GreaterNumericalCondition, "dlplan::policy::GreaterNumericalCondition")
BOOST_CLASS_EXPORT_KEY2(dlplan::policy::EqualNumericalCondition, "dlplan::policy::EqualNumericalCondition")

#endif