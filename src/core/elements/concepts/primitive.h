#ifndef DLPLAN_SRC_CORE_ELEMENTS_CONCEPTS_PRIMITIVE_H_
#define DLPLAN_SRC_CORE_ELEMENTS_CONCEPTS_PRIMITIVE_H_

#include <sstream>
#include <memory>

#include <boost/serialization/export.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>

#include "../utils.h"
#include "../../../utils/collections.h"
#include "../../../../include/dlplan/core.h"

using namespace std::string_literals;


namespace dlplan::core {
class PrimitiveConcept;
}


namespace boost::serialization {
    template<typename Archive>
    void serialize(Archive& ar, dlplan::core::PrimitiveConcept& concept, const unsigned int version);
    template<class Archive>
    void save_construct_data(Archive& ar, const dlplan::core::PrimitiveConcept* concept, const unsigned int version);
    template<class Archive>
    void load_construct_data(Archive& ar, dlplan::core::PrimitiveConcept* concept, const unsigned int version);
}


namespace dlplan::core {
class PrimitiveConcept : public Concept {
private:
    void compute_result(const State& state, ConceptDenotation& result) const;

    ConceptDenotation evaluate_impl(const State& state, DenotationsCaches&) const override;

    ConceptDenotations evaluate_impl(const States& states, DenotationsCaches& caches) const override;

    template<typename Archive>
    friend void boost::serialization::serialize(Archive& ar, PrimitiveConcept& concept, const unsigned int version);
    template<class Archive>
    friend void boost::serialization::save_construct_data(Archive& ar, const PrimitiveConcept* concept, const unsigned int version);
    template<class Archive>
    friend void boost::serialization::load_construct_data(Archive& ar, PrimitiveConcept* concept, const unsigned int version);

protected:
    const Predicate m_predicate;
    const int m_pos;

public:
    PrimitiveConcept(std::shared_ptr<VocabularyInfo> vocabulary_info, ElementIndex index, const Predicate& predicate, int pos);

    ConceptDenotation evaluate(const State& state) const override;

    int compute_complexity() const override;

    void compute_repr(std::stringstream& out) const override;

    int compute_evaluate_time_score() const override;

    static std::string get_name();
};

}

BOOST_CLASS_EXPORT_KEY2(dlplan::core::PrimitiveConcept, "dlplan::core::PrimitiveConcept")

#endif
