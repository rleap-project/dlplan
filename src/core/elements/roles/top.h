#ifndef DLPLAN_SRC_CORE_ELEMENTS_ROLES_TOP_H_
#define DLPLAN_SRC_CORE_ELEMENTS_ROLES_TOP_H_

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
class TopRole;
}


namespace boost::serialization {
    template<typename Archive>
    void serialize(Archive& ar, dlplan::core::TopRole& t, const unsigned int version);
    template<class Archive>
    void save_construct_data(Archive& ar, const dlplan::core::TopRole* t, const unsigned int version);
    template<class Archive>
    void load_construct_data(Archive& ar, dlplan::core::TopRole* t, const unsigned int version);

    template<typename Archive>
    void serialize(Archive& ar, std::pair<const dlplan::core::AndConcept, std::weak_ptr<dlplan::core::AndConcept>>& t, const unsigned int version);
    template<class Archive>
    void save_construct_data(Archive& ar, const std::pair<const dlplan::core::AndConcept, std::weak_ptr<dlplan::core::AndConcept>>* t, const unsigned int version);
    template<class Archive>
    void load_construct_data(Archive& ar, std::pair<const dlplan::core::AndConcept, std::weak_ptr<dlplan::core::AndConcept>>* t, const unsigned int version);
}


namespace dlplan::core {
class TopRole : public Role {
private:
    RoleDenotation evaluate_impl(const State& state, DenotationsCaches&) const override {
        RoleDenotation denotation(state.get_instance_info()->get_objects().size());
        denotation.set();
        return denotation;
    }

    RoleDenotations evaluate_impl(const States& states, DenotationsCaches& caches) const override {
        RoleDenotations denotations;
        denotations.reserve(states.size());
        for (size_t i = 0; i < states.size(); ++i) {
            RoleDenotation denotation(states[i].get_instance_info()->get_objects().size());
            denotation.set();
            denotations.push_back(caches.role_denotation_cache.insert_denotation(std::move(denotation)));
        }
        return denotations;
    }

    TopRole(ElementIndex index, std::shared_ptr<VocabularyInfo> vocabulary_info)
        : Role(vocabulary_info, index, true) { }

    template<typename Archive>
    friend void boost::serialization::serialize(Archive& ar, TopRole& t, const unsigned int version);
    template<class Archive>
    friend void boost::serialization::save_construct_data(Archive& ar, const TopRole* t, const unsigned int version);
    template<class Archive>
    friend void boost::serialization::load_construct_data(Archive& ar, TopRole* t, const unsigned int version);
    template<typename... Ts>
    friend class dlplan::utils::ReferenceCountedObjectFactory;

public:
    bool operator==(const Role& other) const override {
        if (typeid(*this) == typeid(other)) {
            const auto& other_derived = static_cast<const TopRole&>(other);
            return m_is_static == other_derived.m_is_static;
        }
        return false;
    }

    size_t hash() const {
        return dlplan::utils::hash_combine(m_is_static);
    }

    RoleDenotation evaluate(const State& state) const override {
        auto denotation = RoleDenotation(state.get_instance_info()->get_objects().size());
        denotation.set();
        return denotation;

    }

    int compute_complexity() const override {
        return 1;
    }

    void compute_repr(std::stringstream& out) const override {
        out << "r_top";
    }

    int compute_evaluate_time_score() const override {
        return SCORE_CONSTANT;
    }
};

}


namespace boost::serialization {
template<typename Archive>
void serialize(Archive& /* ar */ , dlplan::core::TopRole& t, const unsigned int /* version */ )
{
    boost::serialization::base_object<dlplan::core::Role>(t);
}

template<class Archive>
void save_construct_data(Archive & ar, const dlplan::core::TopRole* t, const unsigned int /* version */ )
{
    ar << t->m_vocabulary_info;
    ar << t->m_index;
}

template<class Archive>
void load_construct_data(Archive & ar, dlplan::core::TopRole* t, const unsigned int /* version */ )
{
    std::shared_ptr<dlplan::core::VocabularyInfo> vocabulary;
    int index;
    ar >> vocabulary;
    ar >> index;
    ::new(t)dlplan::core::TopRole(index, vocabulary);
}


template<typename Archive>
void serialize(Archive& /*ar*/, std::pair<const dlplan::core::TopRole, std::weak_ptr<dlplan::core::TopRole>>& /*t*/, const unsigned int /*version*/) {
}

template<class Archive>
void save_construct_data(Archive& ar, const std::pair<const dlplan::core::TopRole, std::weak_ptr<dlplan::core::TopRole>>* t, const unsigned int /*version*/) {
    ar << t->first;
    ar << t->second;
}

template<class Archive>
void load_construct_data(Archive& ar, std::pair<const dlplan::core::TopRole, std::weak_ptr<dlplan::core::TopRole>>* t, const unsigned int /*version*/) {
    dlplan::core::TopRole* first;
    std::weak_ptr<dlplan::core::TopRole>* second;
    ar >> const_cast<dlplan::core::TopRole&>(t->first);
    ar >> t->second;
    ::new(t)std::pair<const dlplan::core::TopRole, std::weak_ptr<dlplan::core::TopRole>>(*first, *second);
    delete first;
    delete second;
}

}

BOOST_CLASS_EXPORT_GUID(dlplan::core::TopRole, "dlplan::core::TopRole")


namespace std {
    template<>
    struct less<std::shared_ptr<const dlplan::core::TopRole>>
    {
        bool operator()(
            const std::shared_ptr<const dlplan::core::TopRole>& left_role,
            const std::shared_ptr<const dlplan::core::TopRole>& right_role) const {
            return *left_role < *right_role;
        }
    };

    template<>
    struct hash<dlplan::core::TopRole>
    {
        std::size_t operator()(const dlplan::core::TopRole& role) const {
            return role.hash();
        }
    };
}

#endif
