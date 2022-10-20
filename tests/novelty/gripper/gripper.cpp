#include <gtest/gtest.h>

#include "../include/dlplan/state_space.h"
#include "../include/dlplan/novelty.h"

#include <iostream>

using namespace dlplan::core;
using namespace dlplan::state_space;
using namespace dlplan::novelty;


TEST(DLPTests, NoveltyGripperTest) {
    StateSpaceGenerator().generate_state_space("domain.pddl", "p-1-0.pddl");
    auto state_space = std::make_shared<StateSpace>(StateSpaceReader().read());
    std::cout << "State space:" << std::endl;
    std::cout << state_space->to_dot(1) << std::endl;
    auto novelty_base = std::make_shared<NoveltyBase>(
        state_space->get_instance_info_ref().get_atoms_ref().size(),
        2);
    std::cout << std::endl << "Tuple graphs:" << std::endl;
    for (const auto state_index : state_space->get_state_indices_ref()) {
        auto tuple_graph = TupleGraph(novelty_base, state_space, state_index, 2);
        std::cout << std::endl << tuple_graph.to_dot(1) << std::endl;
    }
}
