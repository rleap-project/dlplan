#ifndef DLPLAN_SRC_CORE_ELEMENTS_ROLES_TRANSITIVE_CLOSURE_H_
#define DLPLAN_SRC_CORE_ELEMENTS_ROLES_TRANSITIVE_CLOSURE_H_

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
class TransitiveClosureRole;
}


namespace boost::serialization {
    template<typename Archive>
    void serialize(Archive& ar, dlplan::core::TransitiveClosureRole& t, const unsigned int version);
    template<class Archive>
    void save_construct_data(Archive& ar, const dlplan::core::TransitiveClosureRole* t, const unsigned int version);
    template<class Archive>
    void load_construct_data(Archive& ar, dlplan::core::TransitiveClosureRole* t, const unsigned int version);
}


namespace dlplan::core {
// https://stackoverflow.com/questions/3517524/what-is-the-best-known-transitive-closure-algorithm-for-a-directed-graph
class TransitiveClosureRole : public Role {
private:
    const std::shared_ptr<const Role> m_role;

    void compute_result(const RoleDenotation& denot, RoleDenotation& result) const {
        result = denot;
        bool changed = false;
        do {
            RoleDenotation tmp_result = result;
            PairsOfObjectIndices pairs = tmp_result.to_vector();
            for (const auto& pair_1 : pairs) {
                for (const auto& pair_2 : pairs) {
                    if (pair_1.second == pair_2.first) {
                        result.insert(std::make_pair(pair_1.first, pair_2.second));
                    }
                }
            }
            changed = (result.size() != tmp_result.size());
        } while (changed);
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

    TransitiveClosureRole(ElementIndex index, std::shared_ptr<VocabularyInfo> vocabulary_info, std::shared_ptr<const Role> role)
        : Role(vocabulary_info, index, role->is_static()), m_role(role) { }

    template<typename Archive>
    friend void boost::serialization::serialize(Archive& ar, TransitiveClosureRole& t, const unsigned int version);
    template<class Archive>
    friend void boost::serialization::save_construct_data(Archive& ar, const TransitiveClosureRole* t, const unsigned int version);
    template<class Archive>
    friend void boost::serialization::load_construct_data(Archive& ar, TransitiveClosureRole* t, const unsigned int version);
    template<typename... Ts>
    friend class dlplan::utils::ReferenceCountedObjectFactory;

public:
    bool operator==(const Role& other) const override {
        if (typeid(*this) == typeid(other)) {
            const auto& other_derived = static_cast<const TransitiveClosureRole&>(other);
            return m_is_static == other_derived.m_is_static
                && m_role == other_derived.m_role;
        }
        return false;
    }

    size_t hash() const {
        return dlplan::utils::hash_combine(m_is_static, m_role);
    }

    RoleDenotation evaluate(const State& state) const override {
        RoleDenotation result(state.get_instance_info()->get_objects().size());
        compute_result(
            m_role->evaluate(state),
            result);
        return result;
    }

    int compute_complexity() const override {
        return m_role->compute_complexity() + 1;
    }

    void compute_repr(std::stringstream& out) const override {
        out << "r_transitive_closure" << "(";
        m_role->compute_repr(out);
        out << ")";
    }

    int compute_evaluate_time_score() const override {
        return m_role->compute_evaluate_time_score() + SCORE_QUBIC;
    }
};

}


namespace boost::serialization {
template<typename Archive>
void serialize(Archive& /* ar */ , dlplan::core::TransitiveClosureRole& t, const unsigned int /* version */ )
{
    boost::serialization::base_object<dlplan::core::Role>(t);
}

template<class Archive>
void save_construct_data(Archive & ar, const dlplan::core::TransitiveClosureRole* t, const unsigned int /* version */ )
{
    ar << t->m_vocabulary_info;
    ar << t->m_index;
    ar << t->m_role;
}

template<class Archive>
void load_construct_data(Archive & ar, dlplan::core::TransitiveClosureRole* t, const unsigned int /* version */ )
{
    std::shared_ptr<dlplan::core::VocabularyInfo> vocabulary;
    int index;
    std::shared_ptr<const dlplan::core::Role> role;
    ar >> vocabulary;
    ar >> index;
    ar >> role;
    ::new(t)dlplan::core::TransitiveClosureRole(index, vocabulary, role);
}


template<typename Archive>
void serialize(Archive& /*ar*/, std::pair<const dlplan::core::TransitiveClosureRole, std::weak_ptr<dlplan::core::TransitiveClosureRole>>& /*t*/, const unsigned int /*version*/) {
}

template<class Archive>
void save_construct_data(Archive& ar, const std::pair<const dlplan::core::TransitiveClosureRole, std::weak_ptr<dlplan::core::TransitiveClosureRole>>* t, const unsigned int /*version*/) {
    ar << t->first;
    ar << t->second;
}

template<class Archive>
void load_construct_data(Archive& ar, std::pair<const dlplan::core::TransitiveClosureRole, std::weak_ptr<dlplan::core::TransitiveClosureRole>>* t, const unsigned int /*version*/) {
    dlplan::core::TransitiveClosureRole* first;
    std::weak_ptr<dlplan::core::TransitiveClosureRole>* second;
    ar >> const_cast<dlplan::core::TransitiveClosureRole&>(t->first);
    ar >> t->second;
    ::new(t)std::pair<const dlplan::core::TransitiveClosureRole, std::weak_ptr<dlplan::core::TransitiveClosureRole>>(*first, *second);
    delete first;
    delete second;
}

}

BOOST_CLASS_EXPORT_KEY2(dlplan::core::TransitiveClosureRole, "dlplan::core::TransitiveClosureRole")


namespace std {
    template<>
    struct less<std::shared_ptr<const dlplan::core::TransitiveClosureRole>>
    {
        bool operator()(
            const std::shared_ptr<const dlplan::core::TransitiveClosureRole>& left_role,
            const std::shared_ptr<const dlplan::core::TransitiveClosureRole>& right_role) const {
            return *left_role < *right_role;
        }
    };

    template<>
    struct hash<dlplan::core::TransitiveClosureRole>
    {
        std::size_t operator()(const dlplan::core::TransitiveClosureRole& role) const {
            return role.hash();
        }
    };
}

#endif
