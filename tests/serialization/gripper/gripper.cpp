#include <gtest/gtest.h>

#include "../../../include/dlplan/novelty.h"
#include "../../../include/dlplan/state_space.h"
#include "../../../include/dlplan/serialization.h"

#include <sstream>

using namespace dlplan::novelty;
using namespace dlplan::state_space;
using namespace dlplan::serialization;

namespace dlplan::serialization::tests {

TEST(DLPTests, SerializationGripperTest) {
    auto result_1 = generate_state_space("domain.pddl", "p-1-0.pddl", nullptr, 0);
    std::shared_ptr<StateSpace> state_space_1 = std::move(result_1.state_space);
    auto result_2 = generate_state_space("domain.pddl", "p-2-0.pddl", state_space_1->get_instance_info()->get_vocabulary_info(), 1);
    std::shared_ptr<StateSpace> state_space_2 = std::move(result_2.state_space);
    Data out_data;
    out_data.state_spaces.emplace("0", state_space_1);
    out_data.state_spaces.emplace("1", state_space_2);
    auto novelty_base = std::make_shared<NoveltyBase>(state_space_1->get_instance_info()->get_atoms().size(), 1);
    for (const auto& pair : state_space_1->get_states()) {
        out_data.tuple_graphs.emplace(std::to_string(state_space_1->get_instance_info()->get_index()) + "_" + std::to_string(pair.first), std::make_shared<TupleGraph>(novelty_base, state_space_1, pair.first));
    }
    novelty_base = std::make_shared<NoveltyBase>(state_space_2->get_instance_info()->get_atoms().size(), 1);
    for (const auto& pair : state_space_2->get_states()) {
        out_data.tuple_graphs.emplace(std::to_string(state_space_2->get_instance_info()->get_index()) + "_" + std::to_string(pair.first), std::make_shared<TupleGraph>(novelty_base, state_space_2, pair.first));
    }

    std::stringstream buffer;
    dlplan::serialization::serialize(out_data, buffer);
    dlplan::serialization::Data in_data = dlplan::serialization::deserialize(buffer);
    EXPECT_EQ(in_data.state_spaces.size(), 2);
    EXPECT_NE(in_data.state_spaces.at("0")->get_instance_info(), in_data.state_spaces.at("1")->get_instance_info());
    EXPECT_EQ(in_data.state_spaces.at("0")->get_instance_info()->get_vocabulary_info(), in_data.state_spaces.at("1")->get_instance_info()->get_vocabulary_info());

    EXPECT_EQ(in_data.tuple_graphs.size(), state_space_1->get_states().size() + state_space_2->get_states().size());
    EXPECT_EQ(in_data.tuple_graphs.count("0_0"), 1);
    EXPECT_EQ(in_data.tuple_graphs.count("1_0"), 1);
}

}
