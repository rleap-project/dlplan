#include "../../include/dlplan/serialization.h"

#include "../../include/dlplan/core.h"
#include "../../include/dlplan/state_space.h"

#include <boost/serialization/base_object.hpp>
#include <boost/serialization/shared_ptr.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>


using namespace dlplan;


namespace boost::serialization {

template<typename Archive>
void serialize( Archive& ar, core::Predicate& predicate, const unsigned int version )
{
    ar & predicate.m_index;
    ar & predicate.m_name;
    ar & predicate.m_arity;
    ar & predicate.m_is_static;
}

template<typename Archive>
void serialize( Archive& ar, core::Constant& constant, const unsigned int version )
{
    ar & constant.m_index;
    ar & constant.m_name;
}

template<typename Archive>
void serialize( Archive& ar, core::Object& object, const unsigned int version )
{
    ar & object.m_index;
    ar & object.m_name;
}

template<typename Archive>
void serialize( Archive& ar, core::Atom& atom, const unsigned int version )
{
    ar & atom.m_index;
    ar & atom.m_name;
    ar & atom.m_is_static;
    ar & atom.m_predicate_index;
    ar & atom.m_object_indices;
}

template<typename Archive>
void serialize( Archive& ar, std::shared_ptr<core::VocabularyInfo>& vocabulary_info, const unsigned int version )
{
    ar & vocabulary_info;
}

template<typename Archive>
void serialize( Archive& ar, core::VocabularyInfo& vocabulary_info, const unsigned int version )
{
    ar & vocabulary_info.m_constants;
    ar & vocabulary_info.m_constant_name_to_index;
    ar & vocabulary_info.m_predicates;
    ar & vocabulary_info.m_predicate_name_to_index;
}

template<typename Archive>
void serialize( Archive& ar, std::shared_ptr<core::InstanceInfo>& instance_info, const unsigned int version )
{
    ar & instance_info;
}

template<typename Archive>
void serialize( Archive& ar, core::InstanceInfo& instance_info, const unsigned int version )
{
    ar & instance_info.m_index;
    ar & instance_info.m_objects;
    ar & instance_info.m_object_name_to_index;
    ar & instance_info.m_atoms;
    ar & instance_info.m_atom_name_to_index;
    ar & instance_info.m_static_atoms;
    ar & instance_info.m_static_atom_name_to_index;
}

template<typename Archive>
void serialize( Archive& ar, std::vector<std::unique_ptr<state_space::StateSpace>>& state_spaces, const unsigned int version )
{
    // Serialize the size of the vector
    size_t size = state_spaces.size();
    ar & size;

    // Serialize each element in the vector
    for (size_t i = 0; i < size; ++i)
    {
        // For saving: serialize the underlying StateSpace object
        // For loading: create a new unique_ptr and serialize the underlying StateSpace object
        if (Archive::is_saving::value)
        {
            ar & *(state_spaces[i]);
        }
        else if (Archive::is_loading::value)
        {
            state_spaces.emplace_back(std::make_unique<dlplan::state_space::StateSpace>());
            ar & *(state_spaces[i]);
        }
    }
}

template<typename Archive>
void serialize( Archive& ar, std::unique_ptr<state_space::StateSpace>& state_space, const unsigned int version )
{
    ar & state_space;
}

template<typename Archive>
void serialize( Archive& ar, state_space::StateSpace& state_space, const unsigned int version )
{
    ar & state_space.m_instance_info;
    ar & state_space.m_states;
    ar & state_space.m_initial_state_index;
    ar & state_space.m_goal_state_indices;
    ar & state_space.m_forward_successor_state_indices;
    ar & state_space.m_backward_successor_state_indices;
}

}


namespace dlplan::serialization {


void serialize(const Data& data, std::ostream& out_buffer) {
    boost::archive::text_oarchive oa(out_buffer);
    oa & data.m_state_spaces;
}

Data deserialize(std::istream& buffer) {
    return Data{};
}

}
