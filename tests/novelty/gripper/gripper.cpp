#include <gtest/gtest.h>

#include "../../../include/dlplan/state_space.h"
#include "../../../include/dlplan/novelty.h"

#include <iostream>
#include <fstream>

using namespace dlplan::core;
using namespace dlplan::state_space;
using namespace dlplan::novelty;


namespace dlplan::tests::novelty {

TEST(DLPTests, NoveltyGripperTest) {
    auto result = generate_state_space("domain.pddl", "p-1-0.pddl");
    std::shared_ptr<StateSpace> state_space = std::move(result.state_space);

    auto novelty_base_0 = std::make_shared<NoveltyBase>(
        state_space->get_instance_info()->get_atoms().size(),
        0);
    EXPECT_EQ(TupleGraph(novelty_base_0, state_space, 0, false).compute_repr(), "TupleGraph(\n  root_state_index=0,\n  tuple_nodes=[    TupleNode(index=0, tuple_index=0, state_indices={0}, predecessors=[], successors=[1, 2, 3]),\n    TupleNode(index=1, tuple_index=1, state_indices={3}, predecessors=[0], successors=[]),\n    TupleNode(index=2, tuple_index=2, state_indices={2}, predecessors=[0], successors=[]),\n    TupleNode(index=3, tuple_index=3, state_indices={1}, predecessors=[0], successors=[])\n  ],\n  node_indices_by_distance=[\n    [0],\n    [1, 2, 3]\n  ],\n  state_indices_by_distance=[\n    [0],\n    [1, 2, 3]\n  ]\n)");
    EXPECT_EQ(TupleGraph(novelty_base_0, state_space, 0, true).compute_repr(), "TupleGraph(\n  root_state_index=0,\n  tuple_nodes=[    TupleNode(index=0, tuple_index=0, state_indices={0}, predecessors=[], successors=[1, 2, 3]),\n    TupleNode(index=1, tuple_index=1, state_indices={3}, predecessors=[0], successors=[]),\n    TupleNode(index=2, tuple_index=2, state_indices={2}, predecessors=[0], successors=[]),\n    TupleNode(index=3, tuple_index=3, state_indices={1}, predecessors=[0], successors=[])\n  ],\n  node_indices_by_distance=[\n    [0],\n    [1, 2, 3]\n  ],\n  state_indices_by_distance=[\n    [0],\n    [1, 2, 3]\n  ]\n)");

    auto novelty_base_1 = std::make_shared<NoveltyBase>(
        state_space->get_instance_info()->get_atoms().size(),
        1);

    EXPECT_EQ(TupleGraph(novelty_base_1, state_space, 0, false).compute_repr(), "TupleGraph(\n  root_state_index=0,\n  tuple_nodes=[    TupleNode(index=0, tuple_index=0, state_indices={0}, predecessors=[], successors=[2, 6, 7]),\n    TupleNode(index=1, tuple_index=1, state_indices={0}, predecessors=[], successors=[2, 6, 7]),\n    TupleNode(index=2, tuple_index=2, state_indices={1}, predecessors=[0, 1, 3, 4, 5], successors=[]),\n    TupleNode(index=3, tuple_index=3, state_indices={0}, predecessors=[], successors=[2, 6, 7]),\n    TupleNode(index=4, tuple_index=4, state_indices={0}, predecessors=[], successors=[2, 6, 7]),\n    TupleNode(index=5, tuple_index=5, state_indices={0}, predecessors=[], successors=[2, 6, 7]),\n    TupleNode(index=6, tuple_index=7, state_indices={2}, predecessors=[0, 1, 3, 4, 5], successors=[]),\n    TupleNode(index=7, tuple_index=8, state_indices={3}, predecessors=[0, 1, 3, 4, 5], successors=[])\n  ],\n  node_indices_by_distance=[\n    [0, 1, 3, 4, 5],\n    [2, 6, 7]\n  ],\n  state_indices_by_distance=[\n    [0],\n    [1, 2, 3]\n  ]\n)");
    EXPECT_EQ(TupleGraph(novelty_base_1, state_space, 0, true).compute_repr(), "TupleGraph(\n  root_state_index=0,\n  tuple_nodes=[    TupleNode(index=0, tuple_index=0, state_indices={0}, predecessors=[], successors=[1, 2, 3]),\n    TupleNode(index=1, tuple_index=2, state_indices={1}, predecessors=[0], successors=[]),\n    TupleNode(index=2, tuple_index=7, state_indices={2}, predecessors=[0], successors=[]),\n    TupleNode(index=3, tuple_index=8, state_indices={3}, predecessors=[0], successors=[])\n  ],\n  node_indices_by_distance=[\n    [0],\n    [1, 2, 3]\n  ],\n  state_indices_by_distance=[\n    [0],\n    [1, 2, 3]\n  ]\n)");

    std::cout << std::endl;
    auto result2 = generate_state_space("domain.pddl", "p-2-0.pddl");
    auto state_space2 = result2.state_space;
    auto novelty_base_2 = std::make_shared<NoveltyBase>(
        state_space2->get_instance_info()->get_atoms().size(),
        1);
    std::ofstream file("out.dot");
    const auto tg = TupleGraph(novelty_base_2, state_space2, 2, true);
    file << tg.to_dot(1);
    file.close();

    std::cout << tg.compute_repr() << std::endl;
}

}
