#ifndef DLPLAN_SRC_CORE_ELEMENTS_ROLES_NOT_H_
#define DLPLAN_SRC_CORE_ELEMENTS_ROLES_NOT_H_

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
class NotRole;
}


namespace boost::serialization {
    template<typename Archive>
    void serialize(Archive& ar, dlplan::core::NotRole& role, const unsigned int version);
    template<class Archive>
    void save_construct_data(Archive& ar, const dlplan::core::NotRole* role, const unsigned int version);
    template<class Archive>
    void load_construct_data(Archive& ar, dlplan::core::NotRole* role, const unsigned int version);
}


namespace dlplan::core {
class NotRole : public Role {
private:
    void compute_result(const RoleDenotation& denot, RoleDenotation& result) const {
        result = denot;
        ~result;
    }

    RoleDenotation evaluate_impl(const State& state, DenotationsCaches& caches) const override {
        RoleDenotation denotation(state.get_instance_info()->get_objects().size());
        compute_result(
            *m_role->evaluate(state, caches),
            denotation);
        return denotation;
    }

    RoleDenotations evaluate_impl(const States& states, DenotationsCaches& caches) const override {
        RoleDenotations denotations;
        denotations.reserve(states.size());
        auto role_denotations = m_role->evaluate(states, caches);
        for (size_t i = 0; i < states.size(); ++i) {
            RoleDenotation denotation(states[i].get_instance_info()->get_objects().size());
            compute_result(
                *(*role_denotations)[i],
                denotation);
            denotations.push_back(caches.role_denotation_cache.insert_denotation(std::move(denotation)));
        }
       return denotations;
    }

    template<typename Archive>
    friend void boost::serialization::serialize(Archive& ar, NotRole& role, const unsigned int version);
    template<class Archive>
    friend void boost::serialization::save_construct_data(Archive& ar, const NotRole* role, const unsigned int version);
    template<class Archive>
    friend void boost::serialization::load_construct_data(Archive& ar, NotRole* role, const unsigned int version);

protected:
    const std::shared_ptr<const Role> m_role;

public:
    NotRole(std::shared_ptr<VocabularyInfo> vocabulary_info, ElementIndex index, std::shared_ptr<const Role> role)
    : Role(vocabulary_info, index, role->is_static()), m_role(role) {
        if (!role) {
            throw std::runtime_error("NotRole::NotRole - child is a nullptr.");
        }
    }

    RoleDenotation evaluate(const State& state) const override {
        RoleDenotation denotation(state.get_instance_info()->get_objects().size());
        compute_result(
            m_role->evaluate(state),
            denotation);
        return denotation;
    }

    int compute_complexity() const override {
        return m_role->compute_complexity() + 1;
    }

    void compute_repr(std::stringstream& out) const override {
        out << "r_not" << "(";
        m_role->compute_repr(out);
        out << ")";
    }

    int compute_evaluate_time_score() const override {
        return m_role->compute_evaluate_time_score() + SCORE_QUADRATIC;
    }
};

}


namespace boost::serialization {
template<typename Archive>
void serialize(Archive& /* ar */ , dlplan::core::NotRole& t, const unsigned int /* version */ )
{
    boost::serialization::base_object<dlplan::core::Role>(t);
}

template<class Archive>
void save_construct_data(Archive & ar, const dlplan::core::NotRole* t, const unsigned int /* version */ )
{
    ar << t->m_vocabulary_info;
    ar << t->m_index;
    ar << t->m_role;
}

template<class Archive>
void load_construct_data(Archive & ar, dlplan::core::NotRole* t, const unsigned int /* version */ )
{
    std::shared_ptr<dlplan::core::VocabularyInfo> vocabulary;
    int index;
    std::shared_ptr<const dlplan::core::Role> role;
    ar >> vocabulary;
    ar >> index;
    ar >> role;
    ::new(t)dlplan::core::NotRole(vocabulary, index, role);
}

}

BOOST_CLASS_EXPORT_GUID(dlplan::core::NotRole, "dlplan::core::NotRole")

#endif
