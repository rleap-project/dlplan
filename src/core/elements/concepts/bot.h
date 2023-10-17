#ifndef DLPLAN_SRC_CORE_ELEMENTS_CONCEPTS_BOT_H_
#define DLPLAN_SRC_CORE_ELEMENTS_CONCEPTS_BOT_H_

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
class BotConcept;
}


namespace boost::serialization {
    template<typename Archive>
    void serialize(Archive& ar, dlplan::core::BotConcept& concept, const unsigned int version);
    template<class Archive>
    void save_construct_data(Archive& ar, const dlplan::core::BotConcept* concept, const unsigned int version);
    template<class Archive>
    void load_construct_data(Archive& ar, dlplan::core::BotConcept* concept, const unsigned int version);
}


namespace dlplan::core {
class BotConcept : public Concept {
private:
    ConceptDenotation evaluate_impl(const State& state, DenotationsCaches&) const override {
        return ConceptDenotation(state.get_instance_info()->get_objects().size());
    }

    ConceptDenotations evaluate_impl(const States& states, DenotationsCaches& caches) const override {
        ConceptDenotations denotations;
        denotations.reserve(states.size());
        for (size_t i = 0; i < states.size(); ++i) {
            ConceptDenotation denotation(states[i].get_instance_info()->get_objects().size());
            denotations.push_back(caches.concept_denotation_cache.insert_denotation(std::move(denotation)));
        }
        return denotations;
    }

    template<typename Archive>
    friend void boost::serialization::serialize(Archive& ar, BotConcept& concept, const unsigned int version);
    template<class Archive>
    friend void boost::serialization::save_construct_data(Archive& ar, const BotConcept* concept, const unsigned int version);
    template<class Archive>
    friend void boost::serialization::load_construct_data(Archive& ar, BotConcept* concept, const unsigned int version);

public:
    BotConcept(std::shared_ptr<VocabularyInfo> vocabulary_info, ElementIndex index)
    : Concept(vocabulary_info, index, true) { }

    ConceptDenotation evaluate(const State& state) const override {
        return ConceptDenotation(state.get_instance_info()->get_objects().size());
    }

    int compute_complexity() const override {
        return 1;
    }

    void compute_repr(std::stringstream& out) const override {
        out << "c_bot";
    }

    int compute_evaluate_time_score() const override {
        return SCORE_CONSTANT;
    }
};

}


namespace boost::serialization {
template<typename Archive>
void serialize(Archive& /* ar */ , dlplan::core::BotConcept& t, const unsigned int /* version */ )
{
    boost::serialization::base_object<dlplan::core::Concept>(t);
}

template<class Archive>
void save_construct_data(Archive& ar, const dlplan::core::BotConcept* t, const unsigned int /* version */ )
{
    ar << t->m_vocabulary_info;
    ar << t->m_index;
}

template<class Archive>
void load_construct_data(Archive& ar, dlplan::core::BotConcept* t, const unsigned int /* version */ )
{
    std::shared_ptr<dlplan::core::VocabularyInfo> vocabulary;
    int index;
    ar >> vocabulary;
    ar >> index;
    ::new(t)dlplan::core::BotConcept(vocabulary, index);
}

}

BOOST_CLASS_EXPORT_GUID(dlplan::core::BotConcept, "dlplan::core::BotConcept")

#endif
