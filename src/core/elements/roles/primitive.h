#ifndef DLPLAN_SRC_CORE_ELEMENTS_ROLES_PRIMITIVE_H_
#define DLPLAN_SRC_CORE_ELEMENTS_ROLES_PRIMITIVE_H_

#include "../../../../include/dlplan/core.h"

#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/serialization/export.hpp>

#include <sstream>
#include <memory>


namespace dlplan::utils {
template<typename... Ts>
class ReferenceCountedObjectFactory;
}


namespace dlplan::core {
class PrimitiveRole;
}


namespace boost::serialization {
    class access;
    template<typename Archive>
    void serialize(Archive& ar, dlplan::core::PrimitiveRole& t, const unsigned int version);
    template<class Archive>
    void save_construct_data(Archive& ar, const dlplan::core::PrimitiveRole* t, const unsigned int version);
    template<class Archive>
    void load_construct_data(Archive& ar, dlplan::core::PrimitiveRole* t, const unsigned int version);

    template<typename Archive>
    void serialize(Archive& ar, std::pair<const dlplan::core::PrimitiveRole, std::weak_ptr<dlplan::core::PrimitiveRole>>& t, const unsigned int version);
    template<class Archive>
    void save_construct_data(Archive& ar, const std::pair<const dlplan::core::PrimitiveRole, std::weak_ptr<dlplan::core::PrimitiveRole>>* t, const unsigned int version);
    template<class Archive>
    void load_construct_data(Archive& ar, std::pair<const dlplan::core::PrimitiveRole, std::weak_ptr<dlplan::core::PrimitiveRole>>* t, const unsigned int version);
}


namespace dlplan::core {
class PrimitiveRole : public Role {
private:
    const Predicate m_predicate;
    const int m_pos_1;
    const int m_pos_2;

    void compute_result(const State& state, RoleDenotation& result) const;

    RoleDenotation evaluate_impl(const State& state, DenotationsCaches&) const override;

    RoleDenotations evaluate_impl(const States& states, DenotationsCaches& caches) const override;

    PrimitiveRole(ElementIndex index, std::shared_ptr<VocabularyInfo> vocabulary_info, const Predicate& predicate, int pos_1, int pos_2);

    friend class boost::serialization::access;
    template<typename Archive>
    friend void boost::serialization::serialize(Archive& ar, PrimitiveRole& t, const unsigned int version);
    template<class Archive>
    friend void boost::serialization::save_construct_data(Archive& ar, const PrimitiveRole* t, const unsigned int version);
    template<class Archive>
    friend void boost::serialization::load_construct_data(Archive& ar, PrimitiveRole* t, const unsigned int version);
    template<typename... Ts>
    friend class dlplan::utils::ReferenceCountedObjectFactory;

public:
    bool operator==(const Role& other) const override;
    size_t hash() const;

    RoleDenotation evaluate(const State& state) const override;

    int compute_complexity() const override;

    void compute_repr(std::stringstream& out) const override;

    int compute_evaluate_time_score() const override;

    const Predicate& get_predicate() const;
};

}

BOOST_CLASS_EXPORT_KEY2(dlplan::core::PrimitiveRole, "dlplan::core::PrimitiveRole")

namespace std {
    template<>
    struct less<std::shared_ptr<const dlplan::core::PrimitiveRole>>
    {
        bool operator()(
            const std::shared_ptr<const dlplan::core::PrimitiveRole>& left_role,
            const std::shared_ptr<const dlplan::core::PrimitiveRole>& right_role) const;
    };

    template<>
    struct hash<dlplan::core::PrimitiveRole>
    {
        std::size_t operator()(const dlplan::core::PrimitiveRole& role) const;
    };
}

#endif
