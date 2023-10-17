#ifndef DLPLAN_SRC_CORE_ELEMENTS_ROLES_AND_H_
#define DLPLAN_SRC_CORE_ELEMENTS_ROLES_AND_H_

#include <sstream>
#include <memory>

#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/serialization/export.hpp>
#include <boost/serialization/shared_ptr.hpp>

#include "src/core/elements/utils.h"
#include "include/dlplan/core.h"

using namespace std::string_literals;


namespace dlplan::core {
class AndRole;
}


namespace boost::serialization {
    template<typename Archive>
    void serialize(Archive& ar, dlplan::core::AndRole& role, const unsigned int version);
    template<class Archive>
    void save_construct_data(Archive& ar, const dlplan::core::AndRole* role, const unsigned int version);
    template<class Archive>
    void load_construct_data(Archive& ar, dlplan::core::AndRole* role, const unsigned int version);
}


namespace dlplan::core {
class AndRole : public Role {
private:
    void compute_result(const RoleDenotation& left_denot, const RoleDenotation& right_denot, RoleDenotation& result) const {
        result = left_denot;
        result &= right_denot;
    }

    RoleDenotation evaluate_impl(const State& state, DenotationsCaches& caches) const override {
        RoleDenotation denotation(state.get_instance_info()->get_objects().size());
        compute_result(
            *m_role_left->evaluate(state, caches),
            *m_role_right->evaluate(state, caches),
            denotation);
        return denotation;
    }

    RoleDenotations evaluate_impl(const States& states, DenotationsCaches& caches) const override {
        RoleDenotations denotations;
        denotations.reserve(states.size());
        auto role_left_denotations = m_role_left->evaluate(states, caches);
        auto role_right_denotations = m_role_right->evaluate(states, caches);
        for (size_t i = 0; i < states.size(); ++i) {
            RoleDenotation denotation(states[i].get_instance_info()->get_objects().size());
            compute_result(
                *(*role_left_denotations)[i],
                *(*role_right_denotations)[i],
                denotation);
            denotations.push_back(caches.role_denotation_cache.insert_denotation(std::move(denotation)));
        }
        return denotations;
    }

    template<typename Archive>
    friend void boost::serialization::serialize(Archive& ar, AndRole& role, const unsigned int version);
    template<class Archive>
    friend void boost::serialization::save_construct_data(Archive& ar, const AndRole* role, const unsigned int version);
    template<class Archive>
    friend void boost::serialization::load_construct_data(Archive& ar, AndRole* role, const unsigned int version);

protected:
    std::shared_ptr<const Role> m_role_left;
    std::shared_ptr<const Role> m_role_right;

public:
    AndRole(std::shared_ptr<VocabularyInfo> vocabulary_info, ElementIndex index, std::shared_ptr<const Role> role_1, std::shared_ptr<const Role> role_2)
    : Role(vocabulary_info, index, role_1->is_static() && role_2->is_static()),
      m_role_left(role_1),
      m_role_right(role_2) {
        if (!(role_1 && role_2)) {
            throw std::runtime_error("AndRole::AndRole - at least one child is a nullptr.");
        }
        std::stringstream ss1;
        m_role_left->compute_repr(ss1);
        std::stringstream ss2;
        m_role_right->compute_repr(ss2);
        if (ss1.str() > ss2.str()) swap(m_role_left, m_role_right);
    }

    RoleDenotation evaluate(const State& state) const override {
        RoleDenotation denotation(state.get_instance_info()->get_objects().size());
        compute_result(
            m_role_left->evaluate(state),
            m_role_right->evaluate(state),
            denotation);
        return denotation;
    }

    int compute_complexity() const override {
        return m_role_left->compute_complexity() + m_role_right->compute_complexity() + 1;
    }

    void compute_repr(std::stringstream& out) const override {
        out << "r_and" << "(";
        m_role_left->compute_repr(out);
        out << ",";
        m_role_right->compute_repr(out);
        out << ")";
    }

    int compute_evaluate_time_score() const override {
        return m_role_left->compute_evaluate_time_score() + m_role_right->compute_evaluate_time_score() + SCORE_QUADRATIC;
    }
};

}


namespace boost::serialization {
template<typename Archive>
void serialize(Archive& /* ar */ , dlplan::core::AndRole& t, const unsigned int /* version */ )
{
    boost::serialization::base_object<dlplan::core::Role>(t);
}

template<class Archive>
void save_construct_data(Archive & ar, const dlplan::core::AndRole* t, const unsigned int /* version */ )
{
    ar << t->m_vocabulary_info;
    ar << t->m_index;
    ar << t->m_role_left;
    ar << t->m_role_right;
}

template<class Archive>
void load_construct_data(Archive & ar, dlplan::core::AndRole* t, const unsigned int /* version */ )
{
    std::shared_ptr<dlplan::core::VocabularyInfo> vocabulary;
    int index;
    std::shared_ptr<const dlplan::core::Role> role_left;
    std::shared_ptr<const dlplan::core::Role> role_right;
    ar >> vocabulary;
    ar >> index;
    ar >> role_left;
    ar >> role_right;
    ::new(t)dlplan::core::AndRole(vocabulary, index, role_left, role_right);
}

}

BOOST_CLASS_EXPORT_GUID(dlplan::core::AndRole, "dlplan::core::AndRole")

#endif
