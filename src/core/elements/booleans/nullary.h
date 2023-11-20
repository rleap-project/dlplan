#ifndef DLPLAN_SRC_CORE_ELEMENTS_BOOLEANS_NULLARY_H_
#define DLPLAN_SRC_CORE_ELEMENTS_BOOLEANS_NULLARY_H_

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
class NullaryBoolean;
}


namespace boost::serialization {
    template<typename Archive>
    void serialize(Archive& ar, dlplan::core::NullaryBoolean& t, const unsigned int version);
    template<class Archive>
    void save_construct_data(Archive& ar, const dlplan::core::NullaryBoolean* t, const unsigned int version);
    template<class Archive>
    void load_construct_data(Archive& ar, dlplan::core::NullaryBoolean* t, const unsigned int version);

    template<typename Archive>
    void serialize(Archive& ar, std::pair<const dlplan::core::NullaryBoolean, std::weak_ptr<dlplan::core::NullaryBoolean>>& t, const unsigned int version);
    template<class Archive>
    void save_construct_data(Archive& ar, const std::pair<const dlplan::core::NullaryBoolean, std::weak_ptr<dlplan::core::NullaryBoolean>>* t, const unsigned int version);
    template<class Archive>
    void load_construct_data(Archive& ar, std::pair<const dlplan::core::NullaryBoolean, std::weak_ptr<dlplan::core::NullaryBoolean>>* t, const unsigned int version);
}


namespace dlplan::core {
class NullaryBoolean : public Boolean {
private:
    const Predicate m_predicate;

    void compute_result(const State& state, bool& result) const {
        const auto& atoms = state.get_instance_info()->get_atoms();
        for (int atom_idx : state.get_atom_indices()) {
            const auto& atom = atoms[atom_idx];
            if (atom.get_predicate_index() == m_predicate.get_index()) {
                result = true;
                return;
            }
        }
        for (const auto &atom : state.get_instance_info()->get_static_atoms()) {
            if (atom.get_predicate_index() == m_predicate.get_index()) {
                result = true;
                return;
            }
        }
        result = false;
    }

    bool evaluate_impl(const State& state, DenotationsCaches&) const override {
        return evaluate(state);
    }

    BooleanDenotations
    evaluate_impl(const States& states, DenotationsCaches&) const override {
        BooleanDenotations denotations;
        for (size_t i = 0; i < states.size(); ++i) {
            denotations.push_back(evaluate(states[i]));
        }
        return denotations;
    }

    NullaryBoolean(ElementIndex index, std::shared_ptr<VocabularyInfo> vocabulary_info, const Predicate& predicate)
    : Boolean(vocabulary_info, index, predicate.is_static()), m_predicate(predicate) {
        if (predicate.get_arity() != 0) {
            throw std::runtime_error("NullaryBoolean::NullaryBoolean - expected predicate with arity 0.");
        }
    }

    template<typename Archive>
    friend void boost::serialization::serialize(Archive& ar, NullaryBoolean& t, const unsigned int version);
    template<class Archive>
    friend void boost::serialization::save_construct_data(Archive& ar, const NullaryBoolean* t, const unsigned int version);
    template<class Archive>
    friend void boost::serialization::load_construct_data(Archive& ar, NullaryBoolean* t, const unsigned int version);
    template<typename... Ts>
    friend class dlplan::utils::ReferenceCountedObjectFactory;

public:
    bool operator==(const Boolean& other) const override {
        if (typeid(*this) == typeid(other)) {
            const auto& other_derived = static_cast<const NullaryBoolean&>(other);
            return m_is_static == other_derived.m_is_static
                && m_predicate == other_derived.m_predicate;
        }
        return false;
    }

    size_t hash() const {
        return dlplan::utils::hash_combine(m_is_static, m_predicate);
    }

    bool evaluate(const State& state) const override {
        bool denotation;
        compute_result(state, denotation);
        return denotation;
    }

    int compute_complexity() const override {
        return 1;
    }

    void compute_repr(std::stringstream& out) const override {
        out << "b_nullary" << "(" << m_predicate.get_name() << ")";
    }

    int compute_evaluate_time_score() const override {
        return SCORE_LINEAR;
    }
};

}


namespace boost::serialization {
template<typename Archive>
void serialize(Archive& /* ar */ , dlplan::core::NullaryBoolean& t, const unsigned int /* version */ )
{
    boost::serialization::base_object<dlplan::core::Boolean>(t);
}

template<class Archive>
void save_construct_data(Archive& ar, const dlplan::core::NullaryBoolean* t, const unsigned int /* version */ )
{
    ar << t->m_vocabulary_info;
    ar << t->m_index;
    ar << &t->m_predicate;
}

template<class Archive>
void load_construct_data(Archive& ar, dlplan::core::NullaryBoolean* t, const unsigned int /* version */ )
{
    std::shared_ptr<dlplan::core::VocabularyInfo> vocabulary;
    int index;
    dlplan::core::Predicate* predicate;
    ar >> vocabulary;
    ar >> index;
    ar >> predicate;
    ::new(t)dlplan::core::NullaryBoolean(index, vocabulary, *predicate);
    delete predicate;
}


template<typename Archive>
void serialize(Archive& /*ar*/, std::pair<const dlplan::core::NullaryBoolean, std::weak_ptr<dlplan::core::NullaryBoolean>>& /*t*/, const unsigned int /*version*/) {
}

template<class Archive>
void save_construct_data(Archive& ar, const std::pair<const dlplan::core::NullaryBoolean, std::weak_ptr<dlplan::core::NullaryBoolean>>* t, const unsigned int /*version*/) {
    ar << t->first;
    ar << t->second;
}

template<class Archive>
void load_construct_data(Archive& ar, std::pair<const dlplan::core::NullaryBoolean, std::weak_ptr<dlplan::core::NullaryBoolean>>* t, const unsigned int /*version*/) {
    dlplan::core::NullaryBoolean* first;
    std::weak_ptr<dlplan::core::NullaryBoolean>* second;
    ar >> const_cast<dlplan::core::NullaryBoolean&>(t->first);
    ar >> t->second;
    ::new(t)std::pair<const dlplan::core::NullaryBoolean, std::weak_ptr<dlplan::core::NullaryBoolean>>(*first, *second);
    delete first;
    delete second;
}

}

BOOST_CLASS_EXPORT_KEY2(dlplan::core::NullaryBoolean, "dlplan::core::NullaryBoolean")


namespace std {
    template<>
    struct less<std::shared_ptr<const dlplan::core::NullaryBoolean>>
    {
        bool operator()(
            const std::shared_ptr<const dlplan::core::NullaryBoolean>& left_boolean,
            const std::shared_ptr<const dlplan::core::NullaryBoolean>& right_boolean) const {
            return *left_boolean < *right_boolean;
        }
    };

    template<>
    struct hash<dlplan::core::NullaryBoolean>
    {
        std::size_t operator()(const dlplan::core::NullaryBoolean& boolean) const {
            return boolean.hash();
        }
    };
}


#endif
