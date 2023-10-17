
#include <boost/serialization/serialization.hpp>
#include <boost/serialization/unordered_map.hpp>
#include <boost/serialization/shared_ptr.hpp>
#include <boost/serialization/utility.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>

#include "include/dlplan/serialization.h"
#include "include/dlplan/core.h"
#include "include/dlplan/novelty.h"
#include "include/dlplan/policy.h"
#include "include/dlplan/state_space.h"


namespace boost::serialization {
template<typename Archive>
void serialize( Archive& ar, dlplan::serialization::Data& t, const unsigned int /* version */ )
{
    ar & t.vocabulary_infos;
    ar & t.instance_infos;
    ar & t.syntatic_element_factories;
    ar & t.denotations_caches;
    ar & t.state_spaces;
    ar & t.tuple_graphs;
    ar & t.policies;
    ar & t.policy_factories;
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

