#include "state_space.h"

#include <istream>

using namespace dlplan;


namespace dlplan::serialization {

struct Data {
    std::vector<std::shared_ptr<state_space::StateSpace>> state_spaces;
};

void serialize(const Data& data, std::ostream& out_buffer);

Data deserialize(std::istream& buffer);

}
