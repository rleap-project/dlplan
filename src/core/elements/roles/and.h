#ifndef DLPLAN_SRC_CORE_ELEMENTS_ROLES_AND_H_
#define DLPLAN_SRC_CORE_ELEMENTS_ROLES_AND_H_

#include "../utils.h"
#include "../../../../include/dlplan/core.h"

#include <boost/serialization/export.hpp>

#include <sstream>
#include <memory>

using namespace std::string_literals;


namespace dlplan::utils {
template<typename... Ts>
class ReferenceCountedObjectFactory;
}


namespace dlplan::core {
class AndRole;
}


namespace boost::serialization {
    template<typename Archive>
    void serialize(Archive& ar, dlplan::core::AndRole& t, const unsigned int version);
    template<class Archive>
    void save_construct_data(Archive& ar, const dlplan::core::AndRole* t, const unsigned int version);
    template<class Archive>
    void load_construct_data(Archive& ar, dlplan::core::AndRole* t, const unsigned int version);

    template<typename Archive>
    void serialize(Archive& ar, std::pair<const dlplan::core::AndRole, std::weak_ptr<dlplan::core::AndRole>>& t, const unsigned int version);
    template<class Archive>
    void save_construct_data(Archive& ar, const std::pair<const dlplan::core::AndRole, std::weak_ptr<dlplan::core::AndRole>>* t, const unsigned int version);
    template<class Archive>
    void load_construct_data(Archive& ar, std::pair<const dlplan::core::AndRole, std::weak_ptr<dlplan::core::AndRole>>* t, const unsigned int version);
}


namespace dlplan::core {
class AndRole : public Role {
private:
    const std::shared_ptr<const Role> m_role_left;
    const std::shared_ptr<const Role> m_role_right;

    void compute_result(const RoleDenotation& left_denot, const RoleDenotation& right_denot, RoleDenotation& result) const;

    RoleDenotation evaluate_impl(const State& state, DenotationsCaches& caches) const override;

    RoleDenotations evaluate_impl(const States& states, DenotationsCaches& caches) const override;
    AndRole(ElementIndex index, std::shared_ptr<VocabularyInfo> vocabulary_info, std::shared_ptr<const Role> role_1, std::shared_ptr<const Role> role_2);

    template<typename Archive>
    friend void boost::serialization::serialize(Archive& ar, AndRole& t, const unsigned int version);
    template<class Archive>
    friend void boost::serialization::save_construct_data(Archive& ar, const AndRole* t, const unsigned int version);
    template<class Archive>
    friend void boost::serialization::load_construct_data(Archive& ar, AndRole* t, const unsigned int version);
    template<typename... Ts>
    friend class dlplan::utils::ReferenceCountedObjectFactory;

public:
    bool operator==(const Role& other) const override;

    size_t hash() const;

    RoleDenotation evaluate(const State& state) const override;

    int compute_complexity() const override;

    void compute_repr(std::stringstream& out) const override;

    int compute_evaluate_time_score() const override;
};

}

BOOST_CLASS_EXPORT_KEY2(dlplan::core::AndRole, "dlplan::core::AndRole")


namespace std {
    template<>
    struct less<std::shared_ptr<const dlplan::core::AndRole>>
    {
        bool operator()(
            const std::shared_ptr<const dlplan::core::AndRole>& left_role,
            const std::shared_ptr<const dlplan::core::AndRole>& right_role) const {
            return *left_role < *right_role;
        }
    };

    template<>
    struct hash<dlplan::core::AndRole>
    {
        std::size_t operator()(const dlplan::core::AndRole& role) const {
            return role.hash();
        }
    };
}

#endif
