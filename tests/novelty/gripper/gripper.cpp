#include <gtest/gtest.h>

#include "../include/dlplan/state_space.h"
#include "../include/dlplan/novelty.h"

#include <iostream>

using namespace dlplan::core;
using namespace dlplan::state_space;
using namespace dlplan::novelty;


TEST(DLPTests, NoveltyGripperTest) {
    StateSpaceGenerator().generate_state_space("domain.pddl", "p-1-0.pddl");
    auto state_space = StateSpaceReader().read();
    std::cout << "State space:" << std::endl;
    state_space.print();
    auto novelty_base = std::make_shared<NoveltyBase>(
        state_space.get_instance_info_ref().get_atoms_ref().size(),
        1);
    std::cout << std::endl << "Atoms:" << std::endl;
    for (const auto& atom : state_space.get_instance_info_ref().get_atoms_ref()) {
        std::cout << atom.get_index() << " " << atom.get_name_ref() << std::endl;
    }
    std::cout << std::endl << "Tuple graphs:" << std::endl;
    for (const auto state_index : state_space.get_state_indices_ref()) {
        auto tuple_graph = TupleGraph(novelty_base, state_space, state_index, 1);
        std::cout << tuple_graph.str() << std::endl << std::endl;
    }
}
