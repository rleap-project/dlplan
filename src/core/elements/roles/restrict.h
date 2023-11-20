#ifndef DLPLAN_SRC_CORE_ELEMENTS_ROLES_RESTRICT_H_
#define DLPLAN_SRC_CORE_ELEMENTS_ROLES_RESTRICT_H_

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
class RestrictRole;
}


namespace boost::serialization {
    template<typename Archive>
    void serialize(Archive& ar, dlplan::core::RestrictRole& t, const unsigned int version);
    template<class Archive>
    void save_construct_data(Archive& ar, const dlplan::core::RestrictRole* t, const unsigned int version);
    template<class Archive>
    void load_construct_data(Archive& ar, dlplan::core::RestrictRole* t, const unsigned int version);

    template<typename Archive>
    void serialize(Archive& ar, std::pair<const dlplan::core::AndConcept, std::weak_ptr<dlplan::core::AndConcept>>& t, const unsigned int version);
    template<class Archive>
    void save_construct_data(Archive& ar, const std::pair<const dlplan::core::AndConcept, std::weak_ptr<dlplan::core::AndConcept>>* t, const unsigned int version);
    template<class Archive>
    void load_construct_data(Archive& ar, std::pair<const dlplan::core::AndConcept, std::weak_ptr<dlplan::core::AndConcept>>* t, const unsigned int version);
}


namespace dlplan::core {
class RestrictRole : public Role {
private:
    const std::shared_ptr<const Role> m_role;
    const std::shared_ptr<const Concept> m_concept;

    void compute_result(const RoleDenotation& role_denot, const ConceptDenotation& concept_denot, RoleDenotation& result) const {
        result = role_denot;
        for (const auto& pair : role_denot.to_vector()) {
            if (!concept_denot.contains(pair.second)) {
                result.erase(pair);
            }
        }
    }

    RoleDenotation evaluate_impl(const State& state, DenotationsCaches& caches) const override {
        RoleDenotation denotation(state.get_instance_info()->get_objects().size());
        compute_result(
            *m_role->evaluate(state, caches),
            *m_concept->evaluate(state, caches),
            denotation);
        return denotation;
    }

    RoleDenotations evaluate_impl(const States& states, DenotationsCaches& caches) const override {
        RoleDenotations denotations;
        denotations.reserve(states.size());
        auto role_denotations = m_role->evaluate(states, caches);
        auto concept_denotations = m_concept->evaluate(states, caches);
        for (size_t i = 0; i < states.size(); ++i) {
            RoleDenotation denotation(states[i].get_instance_info()->get_objects().size());
            compute_result(
                *(*role_denotations)[i],
                *(*concept_denotations)[i],
                denotation);
            denotations.push_back(caches.role_denotation_cache.insert_denotation(std::move(denotation)));
        }
        return denotations;
    }

    RestrictRole(ElementIndex index, std::shared_ptr<VocabularyInfo> vocabulary_info, std::shared_ptr<const Role> role, std::shared_ptr<const Concept> concept)
    : Role(vocabulary_info, index, role->is_static() && concept->is_static()), m_role(role), m_concept(concept) { }

    template<typename Archive>
    friend void boost::serialization::serialize(Archive& ar, RestrictRole& t, const unsigned int version);
    template<class Archive>
    friend void boost::serialization::save_construct_data(Archive& ar, const RestrictRole* t, const unsigned int version);
    template<class Archive>
    friend void boost::serialization::load_construct_data(Archive& ar, RestrictRole* t, const unsigned int version);
    template<typename... Ts>
    friend class dlplan::utils::ReferenceCountedObjectFactory;

public:
    bool operator==(const Role& other) const override {
        if (typeid(*this) == typeid(other)) {
            const auto& other_derived = static_cast<const RestrictRole&>(other);
            return m_is_static == other_derived.m_is_static
                && m_role == other_derived.m_role
                && m_concept == other_derived.m_concept;
        }
        return false;
    }

    size_t hash() const {
        return dlplan::utils::hash_combine(m_is_static, m_role, m_concept);
    }

    RoleDenotation evaluate(const State& state) const override {
        auto role_denot = m_role->evaluate(state);
        auto concept_denot = m_concept->evaluate(state);
        RoleDenotation denotation(state.get_instance_info()->get_objects().size());
        compute_result(
            m_role->evaluate(state),
            m_concept->evaluate(state),
            denotation);
        return denotation;
    }

    int compute_complexity() const override {
        return m_role->compute_complexity() + m_concept->compute_complexity() + 1;
    }

    void compute_repr(std::stringstream& out) const override {
        out << "r_restrict" << "(";
        m_role->compute_repr(out);
        out << ",";
        m_concept->compute_repr(out);
        out << ")";
    }

    int compute_evaluate_time_score() const override {
        return m_role->compute_evaluate_time_score() + m_concept->compute_evaluate_time_score() + SCORE_QUADRATIC;
    }
};

}


namespace boost::serialization {
template<typename Archive>
void serialize(Archive& /* ar */ , dlplan::core::RestrictRole& t, const unsigned int /* version */ )
{
    boost::serialization::base_object<dlplan::core::Role>(t);
}

template<class Archive>
void save_construct_data(Archive & ar, const dlplan::core::RestrictRole* t, const unsigned int /* version */ )
{
    ar << t->m_vocabulary_info;
    ar << t->m_index;
    ar << t->m_role;
    ar << t->m_concept;
}

template<class Archive>
void load_construct_data(Archive & ar, dlplan::core::RestrictRole* t, const unsigned int /* version */ )
{
    std::shared_ptr<dlplan::core::VocabularyInfo> vocabulary;
    int index;
    std::shared_ptr<const dlplan::core::Role> role;
    std::shared_ptr<const dlplan::core::Concept> concept_;
    ar >> vocabulary;
    ar >> index;
    ar >> role;
    ar >> concept_;
    ::new(t)dlplan::core::RestrictRole(index, vocabulary, role, concept_);
}


template<typename Archive>
void serialize(Archive& /*ar*/, std::pair<const dlplan::core::RestrictRole, std::weak_ptr<dlplan::core::RestrictRole>>& /*t*/, const unsigned int /*version*/) {
}

template<class Archive>
void save_construct_data(Archive& ar, const std::pair<const dlplan::core::RestrictRole, std::weak_ptr<dlplan::core::RestrictRole>>* t, const unsigned int /*version*/) {
    ar << t->first;
    ar << t->second;
}

template<class Archive>
void load_construct_data(Archive& ar, std::pair<const dlplan::core::RestrictRole, std::weak_ptr<dlplan::core::RestrictRole>>* t, const unsigned int /*version*/) {
    dlplan::core::RestrictRole* first;
    std::weak_ptr<dlplan::core::RestrictRole>* second;
    ar >> const_cast<dlplan::core::RestrictRole&>(t->first);
    ar >> t->second;
    ::new(t)std::pair<const dlplan::core::RestrictRole, std::weak_ptr<dlplan::core::RestrictRole>>(*first, *second);
    delete first;
    delete second;
}

}

BOOST_CLASS_EXPORT_GUID(dlplan::core::RestrictRole, "dlplan::core::RestrictRole")


namespace std {
    template<>
    struct less<std::shared_ptr<const dlplan::core::RestrictRole>>
    {
        bool operator()(
            const std::shared_ptr<const dlplan::core::RestrictRole>& left_role,
            const std::shared_ptr<const dlplan::core::RestrictRole>& right_role) const {
            return *left_role < *right_role;
        }
    };

    template<>
    struct hash<dlplan::core::RestrictRole>
    {
        std::size_t operator()(const dlplan::core::RestrictRole& role) const {
            return role.hash();
        }
    };
}

#endif
