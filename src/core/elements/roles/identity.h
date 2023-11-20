#ifndef DLPLAN_SRC_CORE_ELEMENTS_ROLES_IDENTITY_H_
#define DLPLAN_SRC_CORE_ELEMENTS_ROLES_IDENTITY_H_

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
class IdentityRole;
}


namespace boost::serialization {
    template<typename Archive>
    void serialize(Archive& ar, dlplan::core::IdentityRole& t, const unsigned int version);
    template<class Archive>
    void save_construct_data(Archive& ar, const dlplan::core::IdentityRole* t, const unsigned int version);
    template<class Archive>
    void load_construct_data(Archive& ar, dlplan::core::IdentityRole* t, const unsigned int version);
}


namespace dlplan::core {

class IdentityRole : public Role {
private:
    const std::shared_ptr<const Concept> m_concept;

    void compute_result(const ConceptDenotation& denot, RoleDenotation& result) const {
        for (const auto& single : denot.to_vector()) {
            result.insert(std::make_pair(single, single));
        }
    }

    RoleDenotation evaluate_impl(const State& state, DenotationsCaches& caches) const override {
        RoleDenotation denotation(state.get_instance_info()->get_objects().size());
        compute_result(
            *m_concept->evaluate(state, caches),
            denotation);
        return denotation;
    }

    RoleDenotations evaluate_impl(const States& states, DenotationsCaches& caches) const override {
        RoleDenotations denotations;
        denotations.reserve(states.size());
        auto concept_denotations = m_concept->evaluate(states, caches);
        for (size_t i = 0; i < states.size(); ++i) {
            RoleDenotation denotation(states[i].get_instance_info()->get_objects().size());
            compute_result(
                *(*concept_denotations)[i],
                denotation);
            denotations.push_back(caches.role_denotation_cache.insert_denotation(std::move(denotation)));
        }
       return denotations;
    }

    IdentityRole(ElementIndex index, std::shared_ptr<VocabularyInfo> vocabulary_info, std::shared_ptr<const Concept> concept)
        : Role(vocabulary_info, index, concept->is_static()), m_concept(concept) { }

    template<typename Archive>
    friend void boost::serialization::serialize(Archive& ar, IdentityRole& t, const unsigned int version);
    template<class Archive>
    friend void boost::serialization::save_construct_data(Archive& ar, const IdentityRole* t, const unsigned int version);
    template<class Archive>
    friend void boost::serialization::load_construct_data(Archive& ar, IdentityRole* t, const unsigned int version);
    template<typename... Ts>
    friend class dlplan::utils::ReferenceCountedObjectFactory;

public:
    bool operator==(const Role& other) const override {
        if (typeid(*this) == typeid(other)) {
            const auto& other_derived = static_cast<const IdentityRole&>(other);
            return m_is_static == other_derived.m_is_static
                && m_concept == other_derived.m_concept;
        }
        return false;
    }

    size_t hash() const {
        return dlplan::utils::hash_combine(m_is_static, m_concept);
    }

    RoleDenotation evaluate(const State& state) const override {
        RoleDenotation denotation(state.get_instance_info()->get_objects().size());
        compute_result(
            m_concept->evaluate(state),
            denotation);
        return denotation;
    }

    int compute_complexity() const override {
        return m_concept->compute_complexity() + 1;
    }

    void compute_repr(std::stringstream& out) const override {
        out << "r_identity" << "(";
        m_concept->compute_repr(out);
        out << ")";
    }

    int compute_evaluate_time_score() const override {
        return m_concept->compute_evaluate_time_score() + SCORE_LINEAR;
    }
};

}


namespace boost::serialization {
template<typename Archive>
void serialize(Archive& /* ar */ , dlplan::core::IdentityRole& t, const unsigned int /* version */ )
{
    boost::serialization::base_object<dlplan::core::Role>(t);
}

template<class Archive>
void save_construct_data(Archive & ar, const dlplan::core::IdentityRole* t, const unsigned int /* version */ )
{
    ar << t->m_vocabulary_info;
    ar << t->m_index;
    ar << t->m_concept;
}

template<class Archive>
void load_construct_data(Archive & ar, dlplan::core::IdentityRole* t, const unsigned int /* version */ )
{
    std::shared_ptr<dlplan::core::VocabularyInfo> vocabulary;
    int index;
    std::shared_ptr<const dlplan::core::Concept> concept_;
    ar >> vocabulary;
    ar >> index;
    ar >> concept_;
    ::new(t)dlplan::core::IdentityRole(index, vocabulary, concept_);
}


template<typename Archive>
void serialize(Archive& /*ar*/, std::pair<const dlplan::core::IdentityRole, std::weak_ptr<dlplan::core::IdentityRole>>& /*t*/, const unsigned int /*version*/) {
}

template<class Archive>
void save_construct_data(Archive& ar, const std::pair<const dlplan::core::IdentityRole, std::weak_ptr<dlplan::core::IdentityRole>>* t, const unsigned int /*version*/) {
    ar << t->first;
    ar << t->second;
}

template<class Archive>
void load_construct_data(Archive& ar, std::pair<const dlplan::core::IdentityRole, std::weak_ptr<dlplan::core::IdentityRole>>* t, const unsigned int /*version*/) {
    dlplan::core::IdentityRole* first = nullptr;
    std::weak_ptr<dlplan::core::IdentityRole>* second = nullptr;
    ar >> const_cast<dlplan::core::IdentityRole&>(t->first);
    ar >> t->second;
    ::new(t)std::pair<const dlplan::core::IdentityRole, std::weak_ptr<dlplan::core::IdentityRole>>(*first, *second);
    delete first;
    delete second;
}

}

BOOST_CLASS_EXPORT_KEY2(dlplan::core::IdentityRole, "dlplan::core::IdentityRole")


namespace std {
    template<>
    struct less<std::shared_ptr<const dlplan::core::IdentityRole>>
    {
        bool operator()(
            const std::shared_ptr<const dlplan::core::IdentityRole>& left_role,
            const std::shared_ptr<const dlplan::core::IdentityRole>& right_role) const {
            return *left_role < *right_role;
        }
    };

    template<>
    struct hash<dlplan::core::IdentityRole>
    {
        std::size_t operator()(const dlplan::core::IdentityRole& role) const {
            return role.hash();
        }
    };
}

#endif
