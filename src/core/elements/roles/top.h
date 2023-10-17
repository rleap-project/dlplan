#ifndef DLPLAN_SRC_CORE_ELEMENTS_ROLES_TOP_H_
#define DLPLAN_SRC_CORE_ELEMENTS_ROLES_TOP_H_

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
class TopRole;
}


namespace boost::serialization {
    template<typename Archive>
    void serialize(Archive& ar, dlplan::core::TopRole& role, const unsigned int version);
    template<class Archive>
    void save_construct_data(Archive& ar, const dlplan::core::TopRole* role, const unsigned int version);
    template<class Archive>
    void load_construct_data(Archive& ar, dlplan::core::TopRole* role, const unsigned int version);
}


namespace dlplan::core {
class TopRole : public Role {
private:
    RoleDenotation evaluate_impl(const State& state, DenotationsCaches&) const override {
        RoleDenotation denotation(state.get_instance_info()->get_objects().size());
        denotation.set();
        return denotation;
    }

    RoleDenotations evaluate_impl(const States& states, DenotationsCaches& caches) const override {
        RoleDenotations denotations;
        denotations.reserve(states.size());
        for (size_t i = 0; i < states.size(); ++i) {
            RoleDenotation denotation(states[i].get_instance_info()->get_objects().size());
            denotation.set();
            denotations.push_back(caches.role_denotation_cache.insert_denotation(std::move(denotation)));
        }
        return denotations;
    }

    template<typename Archive>
    friend void boost::serialization::serialize(Archive& ar, TopRole& role, const unsigned int version);
    template<class Archive>
    friend void boost::serialization::save_construct_data(Archive& ar, const TopRole* role, const unsigned int version);
    template<class Archive>
    friend void boost::serialization::load_construct_data(Archive& ar, TopRole* role, const unsigned int version);

public:
    TopRole(std::shared_ptr<VocabularyInfo> vocabulary_info, ElementIndex index)
    : Role(vocabulary_info, index, true) {
    }

    RoleDenotation evaluate(const State& state) const override {
        auto denotation = RoleDenotation(state.get_instance_info()->get_objects().size());
        denotation.set();
        return denotation;

    }

    int compute_complexity() const override {
        return 1;
    }

    void compute_repr(std::stringstream& out) const override {
        out << "r_top";
    }

    int compute_evaluate_time_score() const override {
        return SCORE_CONSTANT;
    }
};

}


namespace boost::serialization {
template<typename Archive>
void serialize(Archive& /* ar */ , dlplan::core::TopRole& t, const unsigned int /* version */ )
{
    boost::serialization::base_object<dlplan::core::Role>(t);
}

template<class Archive>
void save_construct_data(Archive & ar, const dlplan::core::TopRole* t, const unsigned int /* version */ )
{
    ar << t->m_vocabulary_info;
    ar << t->m_index;
}

template<class Archive>
void load_construct_data(Archive & ar, dlplan::core::TopRole* t, const unsigned int /* version */ )
{
    std::shared_ptr<dlplan::core::VocabularyInfo> vocabulary;
    int index;
    ar >> vocabulary;
    ar >> index;
    ::new(t)dlplan::core::TopRole(vocabulary, index);
}

}

BOOST_CLASS_EXPORT_GUID(dlplan::core::TopRole, "dlplan::core::TopRole")

#endif
