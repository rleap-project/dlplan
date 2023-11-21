#ifndef DLPLAN_INCLUDE_DLPLAN_CORE_ELEMENTS_BOOLEANS_NULLARY_H_
#define DLPLAN_INCLUDE_DLPLAN_CORE_ELEMENTS_BOOLEANS_NULLARY_H_

#include "../../../../../src/core/elements/utils.h"
#include "../../../core.h"

#include <boost/serialization/export.hpp>

#include <sstream>
#include <memory>

using namespace std::string_literals;


namespace dlplan::utils {
template<typename... Ts>
class ReferenceCountedObjectFactory;
}


namespace dlplan::core {
class NullaryBoolean;
}


namespace boost::serialization {
    template<typename Archive>
    void serialize(Archive& ar, dlplan::core::NullaryBoolean& t, const unsigned int version);
    template<class Archive>
    void save_construct_data(Archive& ar, const dlplan::core::NullaryBoolean* t, const unsigned int version);
    template<class Archive>
    void load_construct_data(Archive& ar, dlplan::core::NullaryBoolean* t, const unsigned int version);

    template<typename Archive>
    void serialize(Archive& ar, std::pair<const dlplan::core::NullaryBoolean, std::weak_ptr<dlplan::core::NullaryBoolean>>& t, const unsigned int version);
    template<class Archive>
    void save_construct_data(Archive& ar, const std::pair<const dlplan::core::NullaryBoolean, std::weak_ptr<dlplan::core::NullaryBoolean>>* t, const unsigned int version);
    template<class Archive>
    void load_construct_data(Archive& ar, std::pair<const dlplan::core::NullaryBoolean, std::weak_ptr<dlplan::core::NullaryBoolean>>* t, const unsigned int version);
}


namespace dlplan::core {
class NullaryBoolean : public Boolean {
private:
    const Predicate m_predicate;

    void compute_result(const State& state, bool& result) const;

    bool evaluate_impl(const State& state, DenotationsCaches&) const override;

    BooleanDenotations
    evaluate_impl(const States& states, DenotationsCaches&) const override;

    NullaryBoolean(ElementIndex index, std::shared_ptr<VocabularyInfo> vocabulary_info, const Predicate& predicate);

    template<typename Archive>
    friend void boost::serialization::serialize(Archive& ar, NullaryBoolean& t, const unsigned int version);
    template<class Archive>
    friend void boost::serialization::save_construct_data(Archive& ar, const NullaryBoolean* t, const unsigned int version);
    template<class Archive>
    friend void boost::serialization::load_construct_data(Archive& ar, NullaryBoolean* t, const unsigned int version);
    template<typename... Ts>
    friend class dlplan::utils::ReferenceCountedObjectFactory;

public:
    bool operator==(const Boolean& other) const override;

    size_t hash() const;

    bool evaluate(const State& state) const override;

    int compute_complexity() const override;

    void compute_repr(std::stringstream& out) const override;

    int compute_evaluate_time_score() const override;
};

}

BOOST_CLASS_EXPORT_KEY2(dlplan::core::NullaryBoolean, "dlplan::core::NullaryBoolean")


namespace std {
    template<>
    struct less<std::shared_ptr<const dlplan::core::NullaryBoolean>>
    {
        bool operator()(
            const std::shared_ptr<const dlplan::core::NullaryBoolean>& left_boolean,
            const std::shared_ptr<const dlplan::core::NullaryBoolean>& right_boolean) const;
    };

    template<>
    struct hash<dlplan::core::NullaryBoolean>
    {
        std::size_t operator()(const dlplan::core::NullaryBoolean& boolean) const;
    };
}


#endif
