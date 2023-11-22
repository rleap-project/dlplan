#ifndef DLPLAN_INCLUDE_DLPLAN_CORE_ELEMENTS_BOOLEANS_NULLARY_H_
#define DLPLAN_INCLUDE_DLPLAN_CORE_ELEMENTS_BOOLEANS_NULLARY_H_

#include "../utils.h"
#include "../../../core.h"

#include <sstream>
#include <memory>

using namespace std::string_literals;


namespace dlplan {
template<typename... Ts>
class ReferenceCountedObjectFactory;
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

    template<typename... Ts>
    friend class dlplan::ReferenceCountedObjectFactory;

public:
    bool are_equal_impl(const Boolean& other) const override;

    size_t hash_impl() const override;

    bool evaluate(const State& state) const override;

    int compute_complexity_impl() const override;

    void str_impl(std::stringstream& out) const override;

    int compute_evaluate_time_score_impl() const override;
};

}


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
