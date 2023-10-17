#ifndef DLPLAN_SRC_CORE_ELEMENTS_CONCEPTS_PROJECTION_H_
#define DLPLAN_SRC_CORE_ELEMENTS_CONCEPTS_PROJECTION_H_

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
class ProjectionConcept;
}


namespace boost::serialization {
    template<typename Archive>
    void serialize(Archive& ar, dlplan::core::ProjectionConcept& concept, const unsigned int version);
    template<class Archive>
    void save_construct_data(Archive& ar, const dlplan::core::ProjectionConcept* concept, const unsigned int version);
    template<class Archive>
    void load_construct_data(Archive& ar, dlplan::core::ProjectionConcept* concept, const unsigned int version);
}


namespace dlplan::core {
class ProjectionConcept : public Concept {
private:
    void compute_result(const RoleDenotation& denot, ConceptDenotation& result) const {
        for (const auto& pair : denot.to_vector()) {
            if (m_pos == 0) result.insert(pair.first);
            else if (m_pos == 1) result.insert(pair.second);
        }
    }

    ConceptDenotation evaluate_impl(const State& state, DenotationsCaches& caches) const override {
        ConceptDenotation denotation(state.get_instance_info()->get_objects().size());
        compute_result(
            *m_role->evaluate(state, caches),
            denotation);
        return denotation;
    }

    ConceptDenotations evaluate_impl(const States& states, DenotationsCaches& caches) const override {
        ConceptDenotations denotations;
        denotations.reserve(states.size());
        auto role_denotations = m_role->evaluate(states, caches);
        for (size_t i = 0; i < states.size(); ++i) {
            ConceptDenotation denotation(states[i].get_instance_info()->get_objects().size());
            compute_result(
                *(*role_denotations)[i],
                denotation);
            denotations.push_back(caches.concept_denotation_cache.insert_denotation(std::move(denotation)));
        }
       return denotations;
    }

    template<typename Archive>
    friend void boost::serialization::serialize(Archive& ar, ProjectionConcept& concept, const unsigned int version);
    template<class Archive>
    friend void boost::serialization::save_construct_data(Archive& ar, const ProjectionConcept* concept, const unsigned int version);
    template<class Archive>
    friend void boost::serialization::load_construct_data(Archive& ar, ProjectionConcept* concept, const unsigned int version);

protected:
    const std::shared_ptr<const Role> m_role;
    const int m_pos;

public:
    ProjectionConcept(std::shared_ptr<VocabularyInfo> vocabulary_info, ElementIndex index, const std::shared_ptr<const Role>& role, int pos)
    : Concept(vocabulary_info, index, role->is_static()), m_role(role), m_pos(pos) {
        if (pos < 0 || pos > 1) {
            throw std::runtime_error("ProjectionConcept::ProjectionConcept - projection index out of range, should be 0 or 1 ("s + std::to_string(pos) + ")");
        }
        if (!role) {
            throw std::runtime_error("ProjectionConcept::ProjectionConcept - child is a nullptr.");
        }
    }

    ConceptDenotation evaluate(const State& state) const override {
        ConceptDenotation denotation(state.get_instance_info()->get_objects().size());
        compute_result(
            m_role->evaluate(state),
            denotation);
        return denotation;
    }

    int compute_complexity() const override {
        return m_role->compute_complexity() + 1;
    }

    void compute_repr(std::stringstream& out) const override {
        out << "c_projection" << "(";
        m_role->compute_repr(out);
        out << "," << std::to_string(m_pos) << ")";
    }

    int compute_evaluate_time_score() const override {
        return m_role->compute_evaluate_time_score() + SCORE_QUADRATIC;
    }
};

}


namespace boost::serialization {
template<typename Archive>
void serialize(Archive& /* ar */ , dlplan::core::ProjectionConcept& t, const unsigned int /* version */ )
{
    boost::serialization::base_object<dlplan::core::Concept>(t);
}

template<class Archive>
void save_construct_data(Archive& ar, const dlplan::core::ProjectionConcept* t, const unsigned int /* version */ )
{
    ar << t->m_vocabulary_info;
    ar << t->m_index;
    ar << t->m_role;
    ar << t->m_pos;
}

template<class Archive>
void load_construct_data(Archive& ar, dlplan::core::ProjectionConcept* t, const unsigned int /* version */ )
{
    std::shared_ptr<dlplan::core::VocabularyInfo> vocabulary;
    int index;
    std::shared_ptr<const dlplan::core::Role> role;
    int pos;
    ar >> vocabulary;
    ar >> index;
    ar >> role;
    ar >> pos;
    ::new(t)dlplan::core::ProjectionConcept(vocabulary, index, role, pos);
}

}

BOOST_CLASS_EXPORT_GUID(dlplan::core::ProjectionConcept, "dlplan::core::ProjectionConcept")

#endif
