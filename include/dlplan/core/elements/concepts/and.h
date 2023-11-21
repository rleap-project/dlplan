#ifndef DLPLAN_SRC_CORE_ELEMENTS_CONCEPTS_AND_H_
#define DLPLAN_SRC_CORE_ELEMENTS_CONCEPTS_AND_H_

#include "../../../../../src/core/elements/utils.h"
#include "../../../core.h"

#include <boost/serialization/export.hpp>

#include <sstream>
#include <memory>

using namespace std::string_literals;


namespace dlplan::utils {
template<typename... Ts>
class ReferenceCountedObjectFactory;
}


namespace dlplan::core {
class AndConcept;
}


namespace boost::serialization {
    class access;
    template<typename Archive>
    void serialize(Archive& ar, dlplan::core::AndConcept& t, const unsigned int version);
    template<class Archive>
    void save_construct_data(Archive& ar, const dlplan::core::AndConcept* t, const unsigned int version);
    template<class Archive>
    void load_construct_data(Archive& ar, dlplan::core::AndConcept* t, const unsigned int version);

    template<typename Archive>
    void serialize(Archive& ar, std::pair<const dlplan::core::AndConcept, std::weak_ptr<dlplan::core::AndConcept>>& t, const unsigned int version);
    template<class Archive>
    void save_construct_data(Archive& ar, const std::pair<const dlplan::core::AndConcept, std::weak_ptr<dlplan::core::AndConcept>>* t, const unsigned int version);
    template<class Archive>
    void load_construct_data(Archive& ar, std::pair<const dlplan::core::AndConcept, std::weak_ptr<dlplan::core::AndConcept>>* t, const unsigned int version);
}


namespace dlplan::core {
class AndConcept : public Concept {
private:
    const std::shared_ptr<const Concept> m_concept_left;
    const std::shared_ptr<const Concept> m_concept_right;

    void compute_result(const ConceptDenotation& left_denot, const ConceptDenotation& right_denot, ConceptDenotation& result) const;

    ConceptDenotation evaluate_impl(const State& state, DenotationsCaches& caches) const override;

    ConceptDenotations evaluate_impl(const States& states, DenotationsCaches& caches) const override;

    AndConcept(ElementIndex index, std::shared_ptr<VocabularyInfo> vocabulary_info, std::shared_ptr<const Concept> concept_1, std::shared_ptr<const Concept> concept_2);
    template<typename Archive>
    friend void boost::serialization::serialize(Archive& ar, AndConcept& t, const unsigned int version);
    template<class Archive>
    friend void boost::serialization::save_construct_data(Archive& ar, const AndConcept* t, const unsigned int version);
    template<class Archive>
    friend void boost::serialization::load_construct_data(Archive& ar, AndConcept* t, const unsigned int version);
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

BOOST_CLASS_EXPORT_KEY2(dlplan::core::AndConcept, "dlplan::core::AndConcept")


namespace std {
    template<>
    struct less<std::shared_ptr<const dlplan::core::AndConcept>>
    {
        bool operator()(
            const std::shared_ptr<const dlplan::core::AndConcept>& left_concept,
            const std::shared_ptr<const dlplan::core::AndConcept>& right_concept) const;
    };

    template<>
    struct hash<dlplan::core::AndConcept>
    {
        std::size_t operator()(const dlplan::core::AndConcept& concept_) const;
    };
}

#endif
