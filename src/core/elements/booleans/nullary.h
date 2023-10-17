#ifndef DLPLAN_SRC_CORE_ELEMENTS_BOOLEANS_NULLARY_H_
#define DLPLAN_SRC_CORE_ELEMENTS_BOOLEANS_NULLARY_H_

#include <sstream>
#include <memory>

#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/serialization/export.hpp>
#include <boost/serialization/base_object.hpp>
#include <boost/serialization/shared_ptr.hpp>

#include "src/core/elements/utils.h"
#include "include/dlplan/core.h"

using namespace std::string_literals;


namespace dlplan::core {
class NullaryBoolean;
}


namespace boost::serialization {
    template<typename Archive>
    void serialize(Archive& ar, dlplan::core::NullaryBoolean& boolean, const unsigned int version);
    template<class Archive>
    void save_construct_data(Archive& ar, const dlplan::core::NullaryBoolean* boolean, const unsigned int version);
    template<class Archive>
    void load_construct_data(Archive& ar, dlplan::core::NullaryBoolean* boolean, const unsigned int version);
}


namespace dlplan::core {
class NullaryBoolean : public Boolean {
private:
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

    template<typename Archive>
    friend void boost::serialization::serialize(Archive& ar, NullaryBoolean& boolean, const unsigned int version);
    template<class Archive>
    friend void boost::serialization::save_construct_data(Archive& ar, const NullaryBoolean* boolean, const unsigned int version);
    template<class Archive>
    friend void boost::serialization::load_construct_data(Archive& ar, NullaryBoolean* boolean, const unsigned int version);

protected:
    const Predicate m_predicate;

public:
    NullaryBoolean(std::shared_ptr<VocabularyInfo> vocabulary_info, ElementIndex index, const Predicate& predicate)
    : Boolean(vocabulary_info, index, predicate.is_static()), m_predicate(predicate) {
        if (predicate.get_arity() != 0) {
            throw std::runtime_error("NullaryBoolean::NullaryBoolean - expected predicate with arity 0.");
        }
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
    ::new(t)dlplan::core::NullaryBoolean(vocabulary, index, *predicate);
    delete predicate;
}

}

BOOST_CLASS_EXPORT_GUID(dlplan::core::NullaryBoolean, "dlplan::core::NullaryBoolean")

#endif
