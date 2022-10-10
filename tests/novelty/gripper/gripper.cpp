#include <gtest/gtest.h>

#include "../include/dlplan/state_space.h"
#include "../include/dlplan/novelty.h"

using namespace dlplan::core;
using namespace dlplan::state_space;
using namespace dlplan::novelty;


TEST(DLPTests, NoveltyGripperTest) {
    StateSpaceGenerator().generate_state_space("domain.pddl", "p-1-0.pddl");
    auto state_space = StateSpaceReader().read();
    auto distance_information = state_space.compute_goal_distance_information();
    auto novelty_base = std::make_shared<NoveltyBase>(state_space.get_instance_info_ref().get_atoms_ref().size(), 1);
    for (const auto state_index : state_space.get_state_indices_ref()) {
        auto tuple_graph = TupleGraph(novelty_base, state_space, state_index);
    }
}
