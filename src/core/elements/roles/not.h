#ifndef DLPLAN_SRC_CORE_ELEMENTS_ROLES_NOT_H_
#define DLPLAN_SRC_CORE_ELEMENTS_ROLES_NOT_H_

#include "../utils.h"
#include "../../../../include/dlplan/core.h"

#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/serialization/export.hpp>
#include <boost/serialization/base_object.hpp>
#include <boost/serialization/shared_ptr.hpp>

#include <sstream>
#include <memory>

using namespace std::string_literals;


namespace dlplan::utils {
template<typename... Ts>
class ReferenceCountedObjectFactory;
}


namespace dlplan::core {
class NotRole;
}


namespace boost::serialization {
    template<typename Archive>
    void serialize(Archive& ar, dlplan::core::NotRole& t, const unsigned int version);
    template<class Archive>
    void save_construct_data(Archive& ar, const dlplan::core::NotRole* t, const unsigned int version);
    template<class Archive>
    void load_construct_data(Archive& ar, dlplan::core::NotRole* t, const unsigned int version);
}


namespace dlplan::core {
class NotRole : public Role {
private:
    const std::shared_ptr<const Role> m_role;

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

    NotRole(ElementIndex index, std::shared_ptr<VocabularyInfo> vocabulary_info, std::shared_ptr<const Role> role)
        : Role(vocabulary_info, index, role->is_static()), m_role(role) { }

    template<typename Archive>
    friend void boost::serialization::serialize(Archive& ar, NotRole& t, const unsigned int version);
    template<class Archive>
    friend void boost::serialization::save_construct_data(Archive& ar, const NotRole* t, const unsigned int version);
    template<class Archive>
    friend void boost::serialization::load_construct_data(Archive& ar, NotRole* t, const unsigned int version);
    template<typename... Ts>
    friend class dlplan::utils::ReferenceCountedObjectFactory;

public:
    bool operator==(const Role& other) const override {
        if (typeid(*this) == typeid(other)) {
            const auto& other_derived = static_cast<const NotRole&>(other);
            return m_is_static == other_derived.m_is_static
                && m_role == other_derived.m_role;
        }
        return false;
    }

    size_t hash() const {
        return dlplan::utils::hash_combine(m_is_static, m_role);
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
    ::new(t)dlplan::core::NotRole(index, vocabulary, role);
}


template<typename Archive>
void serialize(Archive& /*ar*/, std::pair<const dlplan::core::NotRole, std::weak_ptr<dlplan::core::NotRole>>& /*t*/, const unsigned int /*version*/) {
}

template<class Archive>
void save_construct_data(Archive& ar, const std::pair<const dlplan::core::NotRole, std::weak_ptr<dlplan::core::NotRole>>* t, const unsigned int /*version*/) {
    ar << t->first;
    ar << t->second;
}

template<class Archive>
void load_construct_data(Archive& ar, std::pair<const dlplan::core::NotRole, std::weak_ptr<dlplan::core::NotRole>>* t, const unsigned int /*version*/) {
    dlplan::core::NotRole* first = nullptr;
    std::weak_ptr<dlplan::core::NotRole>* second = nullptr;
    ar >> const_cast<dlplan::core::NotRole&>(t->first);
    ar >> t->second;
    ::new(t)std::pair<const dlplan::core::NotRole, std::weak_ptr<dlplan::core::NotRole>>(*first, *second);
    delete first;
    delete second;
}

}

BOOST_CLASS_EXPORT_KEY2(dlplan::core::NotRole, "dlplan::core::NotRole")


namespace std {
    template<>
    struct less<std::shared_ptr<const dlplan::core::NotRole>>
    {
        bool operator()(
            const std::shared_ptr<const dlplan::core::NotRole>& left_role,
            const std::shared_ptr<const dlplan::core::NotRole>& right_role) const {
            return *left_role < *right_role;
        }
    };

    template<>
    struct hash<dlplan::core::NotRole>
    {
        std::size_t operator()(const dlplan::core::NotRole& role) const {
            return role.hash();
        }
    };
}

#endif
