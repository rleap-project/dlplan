#include "../../include/dlplan/serialization.h"

#include "../../include/dlplan/core.h"
#include "../../include/dlplan/state_space.h"

#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/unordered_map.hpp>
#include <boost/serialization/unordered_set.hpp>
#include <boost/serialization/shared_ptr.hpp>

#include <unordered_map>


namespace boost::serialization {

template<typename Archive>
void serialize( Archive& ar, dlplan::core::Predicate& predicate, const unsigned int /* version */ )
{
    ar & predicate.m_index;
    ar & predicate.m_name;
    ar & predicate.m_arity;
    ar & predicate.m_is_static;
}

template<typename Archive>
void serialize( Archive& ar, dlplan::core::Constant& constant, const unsigned int /* version */ )
{
    ar & constant.m_index;
    ar & constant.m_name;
}

template<typename Archive>
void serialize( Archive& ar, dlplan::core::Object& object, const unsigned int /* version */ )
{
    ar & object.m_index;
    ar & object.m_name;
}

template<typename Archive>
void serialize( Archive& ar, dlplan::core::Atom& atom, const unsigned int /* version */ )
{
    ar & atom.m_index;
    ar & atom.m_name;
    ar & atom.m_is_static;
    ar & atom.m_predicate_index;
    ar & atom.m_object_indices;
}

template<typename Archive>
void serialize( Archive& ar, dlplan::core::VocabularyInfo& vocabulary_info, const unsigned int /* version */ )
{
    ar & vocabulary_info.m_constants;
    ar & vocabulary_info.m_constant_name_to_index;
    ar & vocabulary_info.m_predicates;
    ar & vocabulary_info.m_predicate_name_to_index;
}

template<typename Archive>
void serialize( Archive& ar, dlplan::core::InstanceInfo& instance_info, const unsigned int /* version */ )
{
    ar & instance_info.m_index;
    ar & instance_info.m_objects;
    ar & instance_info.m_object_name_to_index;
    ar & instance_info.m_atoms;
    ar & instance_info.m_atom_name_to_index;
    ar & instance_info.m_static_atoms;
    ar & instance_info.m_static_atom_name_to_index;
}

template <typename Archive>
void serialize(Archive& ar, dlplan::core::State& state, const unsigned int /* version */ )
{
    ar & state.m_index;
    ar & state.m_instance_info;
    ar & state.m_atom_indices;
}

template<typename Archive>
void serialize( Archive& ar, dlplan::state_space::StateSpace& state_space, const unsigned int /* version */ )
{
    ar & state_space.m_instance_info;
    ar & state_space.m_states;
    ar & state_space.m_initial_state_index;
    ar & state_space.m_goal_state_indices;
    ar & state_space.m_forward_successor_state_indices;
    ar & state_space.m_backward_successor_state_indices;
}

template<typename Archive>
void serialize( Archive& ar, dlplan::serialization::Data& data, const unsigned int /* version */ )
{
    ar & data.state_spaces;
}

}


namespace dlplan::serialization {

void serialize(const Data& data, std::ostream& out_buffer) {
    boost::archive::text_oarchive oa(out_buffer);
    oa << data;
}

Data deserialize(std::istream& buffer) {
    boost::archive::text_iarchive ia(buffer);
    Data data;
    ia >> data;
    return data;
}

}
