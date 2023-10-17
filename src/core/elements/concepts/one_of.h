#ifndef DLPLAN_SRC_CORE_ELEMENTS_CONCEPTS_ONE_OF_H_
#define DLPLAN_SRC_CORE_ELEMENTS_CONCEPTS_ONE_OF_H_

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
class OneOfConcept;
}


namespace boost::serialization {
    template<typename Archive>
    void serialize(Archive& ar, dlplan::core::OneOfConcept& concept, const unsigned int version);
    template<class Archive>
    void save_construct_data(Archive& ar, const dlplan::core::OneOfConcept* concept, const unsigned int version);
    template<class Archive>
    void load_construct_data(Archive& ar, dlplan::core::OneOfConcept* concept, const unsigned int version);
}


namespace dlplan::core {
class OneOfConcept : public Concept {
private:
    void compute_result(const State& state, ConceptDenotation& result) const {
        bool found = false;
        for (const auto& object : state.get_instance_info()->get_objects()) {
            if (object.get_name() == m_constant.get_name()) {
                result.insert(object.get_index());
                found = true;
                break;
            }
        }
        if (!found) {
            throw std::runtime_error("OneOfConcept::evaluate - no object with name of constant exists in instance: (" + m_constant.get_name() + ")");
        }
    }

    ConceptDenotation evaluate_impl(const State& state, DenotationsCaches&) const override {
        ConceptDenotation denotation(state.get_instance_info()->get_objects().size());
        compute_result(
            state,
            denotation);
        return denotation;
    }

    ConceptDenotations evaluate_impl(const States& states, DenotationsCaches& caches) const override {
        ConceptDenotations denotations;
        denotations.reserve(states.size());
        for (size_t i = 0; i < states.size(); ++i) {
            ConceptDenotation denotation(states[i].get_instance_info()->get_objects().size());
            compute_result(
                states[i],
                denotation);
            denotations.push_back(caches.concept_denotation_cache.insert_denotation(std::move(denotation)));
        }
        return denotations;
    }

    template<typename Archive>
    friend void boost::serialization::serialize(Archive& ar, OneOfConcept& concept, const unsigned int version);
    template<class Archive>
    friend void boost::serialization::save_construct_data(Archive& ar, const OneOfConcept* concept, const unsigned int version);
    template<class Archive>
    friend void boost::serialization::load_construct_data(Archive& ar, OneOfConcept* concept, const unsigned int version);

protected:
    const Constant m_constant;

public:
    OneOfConcept(std::shared_ptr<VocabularyInfo> vocabulary_info, ElementIndex index, const Constant& constant)
    : Concept(vocabulary_info, index, true), m_constant(constant) {
    }

    ConceptDenotation evaluate(const State& state) const override {
        ConceptDenotation result(state.get_instance_info()->get_objects().size());
        compute_result(state, result);
        return result;
    }

    int compute_complexity() const override {
        return 1;
    }

    void compute_repr(std::stringstream& out) const override {
        out << "c_one_of" << "(" << m_constant.get_name() << ")";
    }

    int compute_evaluate_time_score() const override {
        return SCORE_LINEAR;
    }
};

}


namespace boost::serialization {
template<typename Archive>
void serialize(Archive& /* ar */ , dlplan::core::OneOfConcept& t, const unsigned int /* version */ )
{
    boost::serialization::base_object<dlplan::core::Concept>(t);
}

template<class Archive>
void save_construct_data(Archive& ar, const dlplan::core::OneOfConcept* t, const unsigned int /* version */ )
{
    ar << t->m_vocabulary_info;
    ar << t->m_index;
    ar << &t->m_constant;
}

template<class Archive>
void load_construct_data(Archive& ar, dlplan::core::OneOfConcept* t, const unsigned int /* version */ )
{
    std::shared_ptr<dlplan::core::VocabularyInfo> vocabulary;
    int index;
    dlplan::core::Constant* constant;
    ar >> vocabulary;
    ar >> index;
    ar >> constant;
    ::new(t)dlplan::core::OneOfConcept(vocabulary, index, *constant);
    delete constant;
}

}

BOOST_CLASS_EXPORT_GUID(dlplan::core::OneOfConcept, "dlplan::core::OneOfConcept")

#endif
