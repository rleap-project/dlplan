#ifndef DLPLAN_SRC_POLICY_CONDITION_H_
#define DLPLAN_SRC_POLICY_CONDITION_H_

#include "../../include/dlplan/policy.h"

#include <boost/serialization/export.hpp>

#include <string>
#include <memory>

using namespace dlplan;


namespace dlplan::utils {
template<typename... Ts>
class ReferenceCountedObjectFactory;
}


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

    template<typename Archive>
    void serialize(Archive& ar, std::pair<const dlplan::policy::PositiveBooleanCondition, std::weak_ptr<dlplan::policy::PositiveBooleanCondition>>& t, const unsigned int version);
    template<class Archive>
    void save_construct_data(Archive& ar, const std::pair<const dlplan::policy::PositiveBooleanCondition, std::weak_ptr<dlplan::policy::PositiveBooleanCondition>>* t, const unsigned int version);
    template<class Archive>
    void load_construct_data(Archive& ar, std::pair<const dlplan::policy::PositiveBooleanCondition, std::weak_ptr<dlplan::policy::PositiveBooleanCondition>>* t, const unsigned int version);

    template<typename Archive>
    void serialize(Archive& ar, std::pair<const dlplan::policy::NegativeBooleanCondition, std::weak_ptr<dlplan::policy::NegativeBooleanCondition>>& t, const unsigned int version);
    template<class Archive>
    void save_construct_data(Archive& ar, const std::pair<const dlplan::policy::NegativeBooleanCondition, std::weak_ptr<dlplan::policy::NegativeBooleanCondition>>* t, const unsigned int version);
    template<class Archive>
    void load_construct_data(Archive& ar, std::pair<const dlplan::policy::NegativeBooleanCondition, std::weak_ptr<dlplan::policy::NegativeBooleanCondition>>* t, const unsigned int version);

    template<typename Archive>
    void serialize(Archive& ar, std::pair<const dlplan::policy::GreaterNumericalCondition, std::weak_ptr<dlplan::policy::GreaterNumericalCondition>>& t, const unsigned int version);
    template<class Archive>
    void save_construct_data(Archive& ar, const std::pair<const dlplan::policy::GreaterNumericalCondition, std::weak_ptr<dlplan::policy::GreaterNumericalCondition>>* t, const unsigned int version);
    template<class Archive>
    void load_construct_data(Archive& ar, std::pair<const dlplan::policy::GreaterNumericalCondition, std::weak_ptr<dlplan::policy::GreaterNumericalCondition>>* t, const unsigned int version);

    template<typename Archive>
    void serialize(Archive& ar, std::pair<const dlplan::policy::EqualNumericalCondition, std::weak_ptr<dlplan::policy::EqualNumericalCondition>>& t, const unsigned int version);
    template<class Archive>
    void save_construct_data(Archive& ar, const std::pair<const dlplan::policy::EqualNumericalCondition, std::weak_ptr<dlplan::policy::EqualNumericalCondition>>* t, const unsigned int version);
    template<class Archive>
    void load_construct_data(Archive& ar, std::pair<const dlplan::policy::EqualNumericalCondition, std::weak_ptr<dlplan::policy::EqualNumericalCondition>>* t, const unsigned int version);
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
    const std::shared_ptr<const NamedBoolean> m_boolean;

    BooleanCondition(int identifier, std::shared_ptr<const NamedBoolean> boolean);

    int compute_evaluate_time_score() const override;

    std::shared_ptr<const NamedBoolean> get_boolean() const override;

    std::shared_ptr<const NamedNumerical> get_numerical() const override;
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
    const std::shared_ptr<const NamedNumerical> m_numerical;

    NumericalCondition(int identifier, std::shared_ptr<const NamedNumerical> numerical);

    int compute_evaluate_time_score() const override;

    std::shared_ptr<const NamedBoolean> get_boolean() const override;
    std::shared_ptr<const NamedNumerical> get_numerical() const override;
};


class PositiveBooleanCondition : public BooleanCondition {
private:
    PositiveBooleanCondition(int identifier, std::shared_ptr<const NamedBoolean> boolean);

    template<typename Archive>
    friend void boost::serialization::serialize(Archive& ar, PositiveBooleanCondition& t, const unsigned int version);
    template<class Archive>
    friend void boost::serialization::save_construct_data(Archive& ar, const PositiveBooleanCondition* t, const unsigned int version);
    template<class Archive>
    friend void boost::serialization::load_construct_data(Archive& ar, PositiveBooleanCondition* t, const unsigned int version);
    template<typename... Ts>
    friend class dlplan::utils::ReferenceCountedObjectFactory;

public:
    bool operator==(const BaseCondition& other) const override;

    size_t hash() const override;

    bool evaluate(const core::State& source_state) const override;
    bool evaluate(const core::State& source_state, core::DenotationsCaches& caches) const override;

    std::string compute_repr() const override;
    std::string str() const override;
};

class NegativeBooleanCondition : public BooleanCondition {
private:
    NegativeBooleanCondition(int identifier, std::shared_ptr<const NamedBoolean> boolean);

    template<typename Archive>
    friend void boost::serialization::serialize(Archive& ar, NegativeBooleanCondition& t, const unsigned int version);
    template<class Archive>
    friend void boost::serialization::save_construct_data(Archive& ar, const NegativeBooleanCondition* t, const unsigned int version);
    template<class Archive>
    friend void boost::serialization::load_construct_data(Archive& ar, NegativeBooleanCondition* t, const unsigned int version);
    template<typename... Ts>
    friend class dlplan::utils::ReferenceCountedObjectFactory;

public:
    bool operator==(const BaseCondition& other) const override;

    size_t hash() const override;

    bool evaluate(const core::State& source_state) const override;
    bool evaluate(const core::State& source_state, core::DenotationsCaches& caches) const override;

    std::string compute_repr() const override;
    std::string str() const override;
};

class EqualNumericalCondition : public NumericalCondition {
private:
    EqualNumericalCondition(int identifier, std::shared_ptr<const NamedNumerical> numerical);

    template<typename Archive>
    friend void boost::serialization::serialize(Archive& ar, EqualNumericalCondition& t, const unsigned int version);
    template<class Archive>
    friend void boost::serialization::save_construct_data(Archive& ar, const EqualNumericalCondition* t, const unsigned int version);
    template<class Archive>
    friend void boost::serialization::load_construct_data(Archive& ar, EqualNumericalCondition* t, const unsigned int version);
    template<typename... Ts>
    friend class dlplan::utils::ReferenceCountedObjectFactory;

public:
    bool operator==(const BaseCondition& other) const override;

    size_t hash() const override;

    bool evaluate(const core::State& source_state) const override;
    bool evaluate(const core::State& source_state, core::DenotationsCaches& caches) const override;

    std::string compute_repr() const override;
    std::string str() const override;
};

class GreaterNumericalCondition : public NumericalCondition {
private:
    GreaterNumericalCondition(int identifier, std::shared_ptr<const NamedNumerical> numerical);

    template<typename Archive>
    friend void boost::serialization::serialize(Archive& ar, GreaterNumericalCondition& t, const unsigned int version);
    template<class Archive>
    friend void boost::serialization::save_construct_data(Archive& ar, const GreaterNumericalCondition* t, const unsigned int version);
    template<class Archive>
    friend void boost::serialization::load_construct_data(Archive& ar, GreaterNumericalCondition* t, const unsigned int version);
    template<typename... Ts>
    friend class dlplan::utils::ReferenceCountedObjectFactory;

public:
    bool operator==(const BaseCondition& other) const override;

    size_t hash() const override;

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
}

#endif