#ifndef DLPLAN_SRC_CORE_ELEMENTS_ROLES_PRIMITIVE_H_
#define DLPLAN_SRC_CORE_ELEMENTS_ROLES_PRIMITIVE_H_

#include <sstream>
#include <memory>

#include <boost/serialization/export.hpp>

#include "include/dlplan/core.h"


namespace dlplan::core {
class PrimitiveRole;
}


namespace boost::serialization {
    template<typename Archive>
    void serialize(Archive& ar, dlplan::core::PrimitiveRole& role, const unsigned int version);
    template<class Archive>
    void save_construct_data(Archive& ar, const dlplan::core::PrimitiveRole* role, const unsigned int version);
    template<class Archive>
    void load_construct_data(Archive& ar, dlplan::core::PrimitiveRole* role, const unsigned int version);
}


namespace dlplan::core {
class PrimitiveRole : public Role {
private:
    void compute_result(const State& state, RoleDenotation& result) const;

    RoleDenotation evaluate_impl(const State& state, DenotationsCaches&) const override;

    RoleDenotations evaluate_impl(const States& states, DenotationsCaches& caches) const override;

    template<typename Archive>
    friend void boost::serialization::serialize(Archive& ar, PrimitiveRole& role, const unsigned int version);
    template<class Archive>
    friend void boost::serialization::save_construct_data(Archive& ar, const PrimitiveRole* role, const unsigned int version);
    template<class Archive>
    friend void boost::serialization::load_construct_data(Archive& ar, PrimitiveRole* role, const unsigned int version);

protected:
    const Predicate m_predicate;
    const int m_pos_1;
    const int m_pos_2;

public:
    PrimitiveRole(std::shared_ptr<VocabularyInfo> vocabulary_info, ElementIndex index, const Predicate& predicate, int pos_1, int pos_2);

    RoleDenotation evaluate(const State& state) const override;

    int compute_complexity() const override;

    void compute_repr(std::stringstream& out) const override;

    int compute_evaluate_time_score() const override;

    const Predicate& get_predicate() const;
};

}

BOOST_CLASS_EXPORT_KEY2(dlplan::core::PrimitiveRole, "dlplan::core::PrimitiveRole")

#endif
