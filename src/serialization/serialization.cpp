#include "../../include/dlplan/serialization.h"

#include <boost/serialization/base_object.hpp>

namespace dlplan::serialization {


void serialize(const Data& data, std::ostream& out_buffer) {

}

Data deserialize(std::istream& buffer) {
    return Data{};
}

}
