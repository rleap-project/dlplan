#include "../../include/dlplan/serialization.h"

#include "../../include/dlplan/core.h"
#include "../../include/dlplan/state_space.h"

#include <boost/serialization/base_object.hpp>
#include <boost/serialization/shared_ptr.hpp>


using namespace dlplan;


namespace dlplan::serialization {

template<typename Archive>
void serialize( Archive& ar, core::Predicate& predicate, const unsigned int version )
{
}

template<typename Archive>
void serialize( Archive& ar, core::Constant& constant, const unsigned int version )
{
}

template<typename Archive>
void serialize( Archive& ar, core::Object& object, const unsigned int version )
{
}

template<typename Archive>
void serialize( Archive& ar, core::Atom& atom, const unsigned int version )
{
}

template<typename Archive>
void serialize( Archive& ar, std::shared_ptr<core::VocabularyInfo>& vocabulary_info, const unsigned int version )
{
}

template<typename Archive>
void serialize( Archive& ar, std::shared_ptr<core::InstanceInfo>& instance_info, const unsigned int version )
{
}

template<typename Archive>
void serialize( Archive& ar, std::unique_ptr<state_space::StateSpace>& state_space, const unsigned int version )
{
}


void serialize(const Data& data, std::ostream& out_buffer) {

}

Data deserialize(std::istream& buffer) {
    return Data{};
}

}
