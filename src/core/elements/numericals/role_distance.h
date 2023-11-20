#ifndef DLPLAN_SRC_CORE_ELEMENTS_NUMERICAL_ROLE_DISTANCE_H_
#define DLPLAN_SRC_CORE_ELEMENTS_NUMERICAL_ROLE_DISTANCE_H_

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
class RoleDistanceNumerical;
}


namespace boost::serialization {
    template<typename Archive>
    void serialize(Archive& ar, dlplan::core::RoleDistanceNumerical& t, const unsigned int version);
    template<class Archive>
    void save_construct_data(Archive& ar, const dlplan::core::RoleDistanceNumerical* t, const unsigned int version);
    template<class Archive>
    void load_construct_data(Archive& ar, dlplan::core::RoleDistanceNumerical* t, const unsigned int version);

    template<typename Archive>
    void serialize(Archive& ar, std::pair<const dlplan::core::RoleDistanceNumerical, std::weak_ptr<dlplan::core::RoleDistanceNumerical>>& t, const unsigned int version);
    template<class Archive>
    void save_construct_data(Archive& ar, const std::pair<const dlplan::core::RoleDistanceNumerical, std::weak_ptr<dlplan::core::RoleDistanceNumerical>>* t, const unsigned int version);
    template<class Archive>
    void load_construct_data(Archive& ar, std::pair<const dlplan::core::RoleDistanceNumerical, std::weak_ptr<dlplan::core::RoleDistanceNumerical>>* t, const unsigned int version);
}


namespace dlplan::core {
class RoleDistanceNumerical : public Numerical {
private:
    const std::shared_ptr<const Role> m_role_from;
    const std::shared_ptr<const Role> m_role;
    const std::shared_ptr<const Role> m_role_to;

    void compute_result(const RoleDenotation& role_from_denot, const RoleDenotation& role_denot, const RoleDenotation& role_to_denot, int& result) const {
        utils::PairwiseDistances pairwise_distances = utils::compute_floyd_warshall(role_denot);
        result = INF;
        int num_objects = role_denot.get_num_objects();
        for (int k = 0; k < num_objects; ++k) {  // property
            for (int i = 0; i < num_objects; ++i) {  // source
                if (role_from_denot.contains(std::make_pair(k, i))) {
                    for (int j = 0; j < num_objects; ++j) {  // target
                        if (role_to_denot.contains(std::make_pair(k, j))) {
                            result = std::min<int>(result, pairwise_distances[i][j]);
                        }
                    }
                }
            }
        }
    }

    int evaluate_impl(const State& state, DenotationsCaches& caches) const override {
        auto role_from_denot = m_role_from->evaluate(state, caches);
        if (role_from_denot->empty()) {
            return INF;
        }
        auto role_to_denot = m_role_to->evaluate(state, caches);
        if (role_to_denot->empty()) {
            return INF;
        }
        auto role_denot = m_role->evaluate(state, caches);
        int denotation;
        compute_result(*role_from_denot, *role_denot, *role_to_denot, denotation);
        return denotation;
    }

    NumericalDenotations evaluate_impl(const States& states, DenotationsCaches& caches) const override {
        NumericalDenotations denotations;
        denotations.reserve(states.size());
        auto role_from_denots = m_role_from->evaluate(states, caches);
        auto role_denots = m_role->evaluate(states, caches);
        auto role_to_denots = m_role_to->evaluate(states, caches);
        for (size_t i = 0; i < states.size(); ++i) {
            if ((*role_from_denots)[i]->empty()) {
                denotations.push_back(INF);
                continue;
            }
            if ((*role_to_denots)[i]->empty()) {
                denotations.push_back(INF);
                continue;
            }
            int denotation;
            compute_result(
                *(*role_from_denots)[i],
                *(*role_denots)[i],
                *(*role_to_denots)[i],
                denotation);
            denotations.push_back(denotation);
        }
        return denotations;
    }

    RoleDistanceNumerical(ElementIndex index, std::shared_ptr<VocabularyInfo> vocabulary_info, std::shared_ptr<const Role> role_from, std::shared_ptr<const Role> role, std::shared_ptr<const Role> role_to)
        : Numerical(vocabulary_info, index, role_from->is_static() && role->is_static() && role_to->is_static()),
          m_role_from(role_from), m_role(role), m_role_to(role_to) { }

    template<typename Archive>
    friend void boost::serialization::serialize(Archive& ar, RoleDistanceNumerical& t, const unsigned int version);
    template<class Archive>
    friend void boost::serialization::save_construct_data(Archive& ar, const RoleDistanceNumerical* t, const unsigned int version);
    template<class Archive>
    friend void boost::serialization::load_construct_data(Archive& ar, RoleDistanceNumerical* t, const unsigned int version);
    template<typename... Ts>
    friend class dlplan::utils::ReferenceCountedObjectFactory;

public:
    bool operator==(const Numerical& other) const override {
        if (typeid(*this) == typeid(other)) {
            const auto& other_derived = static_cast<const RoleDistanceNumerical&>(other);
            return m_is_static == other_derived.m_is_static
                && m_role_from == other_derived.m_role_from
                && m_role == other_derived.m_role
                && m_role_to == other_derived.m_role_to;
        }
        return false;
    }

    size_t hash() const {
        return dlplan::utils::hash_combine(m_is_static, m_role_from, m_role, m_role_to);
    }

    int evaluate(const State& state) const override {
        auto role_from_denot = m_role_from->evaluate(state);
        if (role_from_denot.empty()) {
            return INF;
        }
        auto role_to_denot = m_role_to->evaluate(state);
        if (role_to_denot.empty()) {
            return INF;
        }
        auto role_denot = m_role->evaluate(state);
        int denotation;
        compute_result(role_from_denot, role_denot, role_to_denot, denotation);
        return denotation;
    }

    int compute_complexity() const override {
        return m_role_from->compute_complexity() + m_role->compute_complexity() + m_role_to->compute_complexity() + 1;
    }

    void compute_repr(std::stringstream& out) const override {
        out << "n_role_distance_numerical" << "(";
        m_role_from->compute_repr(out);
        out << ",";
        m_role->compute_repr(out);
        out << ",";
        m_role_to->compute_repr(out);
        out << ")";
    }

    int compute_evaluate_time_score() const override {
        return m_role_from->compute_evaluate_time_score() + m_role->compute_evaluate_time_score() + m_role_to->compute_evaluate_time_score() + SCORE_QUBIC;
    }
};

}


namespace boost::serialization {
template<typename Archive>
void serialize(Archive& /* ar */ , dlplan::core::RoleDistanceNumerical& t, const unsigned int /* version */ )
{
    boost::serialization::base_object<dlplan::core::Numerical>(t);
}

template<class Archive>
void save_construct_data(Archive & ar, const dlplan::core::RoleDistanceNumerical* t, const unsigned int /* version */ )
{
    ar << t->m_vocabulary_info;
    ar << t->m_index;
    ar << t->m_role_from;
    ar << t->m_role;
    ar << t->m_role_to;
}

template<class Archive>
void load_construct_data(Archive & ar, dlplan::core::RoleDistanceNumerical* t, const unsigned int /* version */ )
{
    std::shared_ptr<dlplan::core::VocabularyInfo> vocabulary;
    int index;
    std::shared_ptr<const dlplan::core::Role> role_from;
    std::shared_ptr<const dlplan::core::Role> role;
    std::shared_ptr<const dlplan::core::Role> role_to;
    ar >> vocabulary;
    ar >> index;
    ar >> role_from;
    ar >> role;
    ar >> role_to;
    ::new(t)dlplan::core::RoleDistanceNumerical(index, vocabulary, role_from, role, role_to);
}


template<typename Archive>
void serialize(Archive& /*ar*/, std::pair<const dlplan::core::RoleDistanceNumerical, std::weak_ptr<dlplan::core::RoleDistanceNumerical>>& /*t*/, const unsigned int /*version*/) {
}

template<class Archive>
void save_construct_data(Archive& ar, const std::pair<const dlplan::core::RoleDistanceNumerical, std::weak_ptr<dlplan::core::RoleDistanceNumerical>>* t, const unsigned int /*version*/) {
    ar << t->first;
    ar << t->second;
}

template<class Archive>
void load_construct_data(Archive& ar, std::pair<const dlplan::core::RoleDistanceNumerical, std::weak_ptr<dlplan::core::RoleDistanceNumerical>>* t, const unsigned int /*version*/) {
    dlplan::core::RoleDistanceNumerical* first = nullptr;
    std::weak_ptr<dlplan::core::RoleDistanceNumerical>* second = nullptr;
    ar >> const_cast<dlplan::core::RoleDistanceNumerical&>(t->first);
    ar >> t->second;
    ::new(t)std::pair<const dlplan::core::RoleDistanceNumerical, std::weak_ptr<dlplan::core::RoleDistanceNumerical>>(*first, *second);
    delete first;
    delete second;
}

}

BOOST_CLASS_EXPORT_KEY2(dlplan::core::RoleDistanceNumerical, "dlplan::core::RoleDistanceNumerical")


namespace std {
    template<>
    struct less<std::shared_ptr<const dlplan::core::RoleDistanceNumerical>>
    {
        bool operator()(
            const std::shared_ptr<const dlplan::core::RoleDistanceNumerical>& left_numerical,
            const std::shared_ptr<const dlplan::core::RoleDistanceNumerical>& right_numerical) const {
            return *left_numerical < *right_numerical;
        }
    };

    template<>
    struct hash<dlplan::core::RoleDistanceNumerical>
    {
        std::size_t operator()(const dlplan::core::RoleDistanceNumerical& numerical) const {
            return numerical.hash();
        }
    };
}


#endif
