#ifndef DLPLAN_SRC_CORE_ELEMENTS_CONCEPTS_PRIMITIVE_H_
#define DLPLAN_SRC_CORE_ELEMENTS_CONCEPTS_PRIMITIVE_H_

#include "../utils.h"
#include "../../../utils/collections.h"
#include "../../../../include/dlplan/core.h"

#include <boost/serialization/export.hpp>

#include <sstream>
#include <memory>


using namespace std::string_literals;


namespace dlplan::utils {
template<typename... Ts>
class ReferenceCountedObjectFactory;
}


namespace dlplan::core {
class PrimitiveConcept;
}


namespace boost::serialization {
    template<typename Archive>
    void serialize(Archive& ar, dlplan::core::PrimitiveConcept& t, const unsigned int version);
    template<class Archive>
    void save_construct_data(Archive& ar, const dlplan::core::PrimitiveConcept* t, const unsigned int version);
    template<class Archive>
    void load_construct_data(Archive& ar, dlplan::core::PrimitiveConcept* t, const unsigned int version);

    template<typename Archive>
    void serialize(Archive& ar, std::pair<const dlplan::core::PrimitiveConcept, std::weak_ptr<dlplan::core::PrimitiveConcept>>& t, const unsigned int version);
    template<class Archive>
    void save_construct_data(Archive& ar, const std::pair<const dlplan::core::PrimitiveConcept, std::weak_ptr<dlplan::core::PrimitiveConcept>>* t, const unsigned int version);
    template<class Archive>
    void load_construct_data(Archive& ar, std::pair<const dlplan::core::PrimitiveConcept, std::weak_ptr<dlplan::core::PrimitiveConcept>>* t, const unsigned int version);
}

namespace dlplan::core {
class PrimitiveConcept : public Concept {
private:
    const Predicate m_predicate;
    const int m_pos;

    void compute_result(const State& state, ConceptDenotation& result) const;

    ConceptDenotation evaluate_impl(const State& state, DenotationsCaches&) const override;

    ConceptDenotations evaluate_impl(const States& states, DenotationsCaches& caches) const override;

    PrimitiveConcept(ElementIndex index, std::shared_ptr<VocabularyInfo> vocabulary_info, const Predicate& predicate, int pos);

    template<typename Archive>
    friend void boost::serialization::serialize(Archive& ar, PrimitiveConcept& t, const unsigned int version);
    template<class Archive>
    friend void boost::serialization::save_construct_data(Archive& ar, const PrimitiveConcept* t, const unsigned int version);
    template<class Archive>
    friend void boost::serialization::load_construct_data(Archive& ar, PrimitiveConcept* t, const unsigned int version);
    template<typename... Ts>
    friend class dlplan::utils::ReferenceCountedObjectFactory;

public:
    bool operator==(const Concept& other) const override;

    size_t hash() const;

    ConceptDenotation evaluate(const State& state) const override;

    int compute_complexity() const override;

    void compute_repr(std::stringstream& out) const override;

    int compute_evaluate_time_score() const override;
};

}

BOOST_CLASS_EXPORT_KEY2(dlplan::core::PrimitiveConcept, "dlplan::core::PrimitiveConcept")


namespace std {
    template<>
    struct less<std::shared_ptr<const dlplan::core::PrimitiveConcept>>
    {
        bool operator()(
            const std::shared_ptr<const dlplan::core::PrimitiveConcept>& left_concept,
            const std::shared_ptr<const dlplan::core::PrimitiveConcept>& right_concept) const {
            return *left_concept < *right_concept;
        }
    };

    template<>
    struct hash<dlplan::core::PrimitiveConcept>
    {
        std::size_t operator()(const dlplan::core::PrimitiveConcept& concept_) const {
            return concept_.hash();
        }
    };
}

#endif
