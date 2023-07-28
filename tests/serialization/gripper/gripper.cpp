#include <gtest/gtest.h>

#include "../../../include/dlplan/state_space.h"
#include "../../../include/dlplan/serialization.h"

#include <sstream>

using namespace dlplan::state_space;
using namespace dlplan::serialization;

namespace dlplan::serialization::tests {


TEST(DLPTests, SerializationGripperTest) {
    auto result_1 = generate_state_space("domain.pddl", "p-1-0.pddl");
    std::shared_ptr<StateSpace> state_space_1 = std::move(result_1.state_space);
    auto result_2 = generate_state_space("domain.pddl", "p-2-0.pddl", state_space_1->get_instance_info()->get_vocabulary_info());
    std::shared_ptr<StateSpace> state_space_2 = std::move(result_2.state_space);
    Data out_data;
    out_data.state_spaces.push_back(state_space_1);
    out_data.state_spaces.push_back(state_space_2);

    std::stringstream buffer;
    dlplan::serialization::serialize(out_data, buffer);
    dlplan::serialization::Data in_data = dlplan::serialization::deserialize(buffer);
    EXPECT_EQ(in_data.state_spaces.size(), 2);
    EXPECT_NE(in_data.state_spaces[0]->get_instance_info(), in_data.state_spaces[1]->get_instance_info());
    EXPECT_EQ(in_data.state_spaces[0]->get_instance_info()->get_vocabulary_info(), in_data.state_spaces[1]->get_instance_info()->get_vocabulary_info());
}

}
