#ifndef DLPLAN_SRC_CORE_ELEMENTS_CONCEPTS_BOT_H_
#define DLPLAN_SRC_CORE_ELEMENTS_CONCEPTS_BOT_H_

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
class BotConcept;
}


namespace boost::serialization {
    template<typename Archive>
    void serialize(Archive& ar, dlplan::core::BotConcept& t, const unsigned int version);
    template<class Archive>
    void save_construct_data(Archive& ar, const dlplan::core::BotConcept* t, const unsigned int version);
    template<class Archive>
    void load_construct_data(Archive& ar, dlplan::core::BotConcept* t, const unsigned int version);

    template<typename Archive>
    void serialize(Archive& ar, std::pair<const dlplan::core::BotConcept, std::weak_ptr<dlplan::core::BotConcept>>& t, const unsigned int version);
    template<class Archive>
    void save_construct_data(Archive& ar, const std::pair<const dlplan::core::BotConcept, std::weak_ptr<dlplan::core::BotConcept>>* t, const unsigned int version);
    template<class Archive>
    void load_construct_data(Archive& ar, std::pair<const dlplan::core::BotConcept, std::weak_ptr<dlplan::core::BotConcept>>* t, const unsigned int version);
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

    BotConcept(ElementIndex index, std::shared_ptr<VocabularyInfo> vocabulary_info)
    : Concept(vocabulary_info, index, true) {
     }

    /// @brief Constructor for serialization.
    BotConcept() {}

    template<typename Archive>
    friend void boost::serialization::serialize(Archive& ar, BotConcept& t, const unsigned int version);
    template<class Archive>
    friend void boost::serialization::save_construct_data(Archive& ar, const BotConcept* t, const unsigned int version);
    template<class Archive>
    friend void boost::serialization::load_construct_data(Archive& ar, BotConcept* t, const unsigned int version);
    template<typename... Ts>
    friend class dlplan::utils::ReferenceCountedObjectFactory;

public:
    bool operator==(const Concept& other) const override {
        if (typeid(*this) == typeid(other)) {
            const auto& other_derived = static_cast<const BotConcept&>(other);
            return m_is_static == other_derived.m_is_static;
        }
        return false;
    }

    size_t hash() const {
        return dlplan::utils::hash_combine(m_is_static);
    }

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
    ::new(t)dlplan::core::BotConcept(index, vocabulary);
}


template<typename Archive>
void serialize(Archive& /*ar*/, std::pair<const dlplan::core::BotConcept, std::weak_ptr<dlplan::core::BotConcept>>& /*t*/, const unsigned int /*version*/) {
}

template<class Archive>
void save_construct_data(Archive& ar, const std::pair<const dlplan::core::BotConcept, std::weak_ptr<dlplan::core::BotConcept>>* t, const unsigned int /*version*/) {
    ar << t->first;
    ar << t->second;
}

template<class Archive>
void load_construct_data(Archive& ar, std::pair<const dlplan::core::BotConcept, std::weak_ptr<dlplan::core::BotConcept>>* t, const unsigned int /*version*/) {
    dlplan::core::BotConcept* first = nullptr;
    std::weak_ptr<dlplan::core::BotConcept>* second = nullptr;
    ar >> const_cast<dlplan::core::BotConcept&>(t->first);
    ar >> t->second;
    ::new(t)std::pair<const dlplan::core::BotConcept, std::weak_ptr<dlplan::core::BotConcept>>(*first, *second);
    delete first;
    delete second;
}
}

BOOST_CLASS_EXPORT_KEY2(dlplan::core::BotConcept, "dlplan::core::BotConcept")


namespace std {
    template<>
    struct less<std::shared_ptr<const dlplan::core::BotConcept>>
    {
        bool operator()(
            const std::shared_ptr<const dlplan::core::BotConcept>& left_concept,
            const std::shared_ptr<const dlplan::core::BotConcept>& right_concept) const {
            return *left_concept < *right_concept;
        }
    };

    template<>
    struct hash<dlplan::core::BotConcept>
    {
        std::size_t operator()(const dlplan::core::BotConcept& concept_) const {
            return concept_.hash();
        }
    };
}

#endif
