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
    auto state_space = std::move(result.state_space);

    auto novelty_base_0 = std::make_shared<NoveltyBase>(
        state_space->get_instance_info()->get_atoms().size(),
        0);
    EXPECT_EQ(TupleGraph(novelty_base_0, state_space, 0).compute_repr(), "TupleGraph(\n  root_state_index=0,\n  tuple_nodes=[    TupleNode(index=0, tuple_index=0, state_indices={0}, predecessors=[], successors=[1, 2, 3]),\n    TupleNode(index=1, tuple_index=1, state_indices={3}, predecessors=[0], successors=[]),\n    TupleNode(index=2, tuple_index=2, state_indices={2}, predecessors=[0], successors=[]),\n    TupleNode(index=3, tuple_index=3, state_indices={1}, predecessors=[0], successors=[])\n  ],\n  node_indices_by_distance=[\n    [0],\n    [1, 2, 3]\n  ],\n  state_indices_by_distance=[\n    [0],\n    [1, 2, 3]\n  ]\n)");

    auto novelty_base_1 = std::make_shared<NoveltyBase>(
        state_space->get_instance_info()->get_atoms().size(),
        1);

    EXPECT_EQ(TupleGraph(novelty_base_1, state_space, 0).compute_repr(), "TupleGraph(\n  root_state_index=0,\n  tuple_nodes=[    TupleNode(index=0, tuple_index=0, state_indices={0}, predecessors=[], successors=[1, 2, 3]),\n    TupleNode(index=1, tuple_index=2, state_indices={1}, predecessors=[0], successors=[]),\n    TupleNode(index=2, tuple_index=7, state_indices={2}, predecessors=[0], successors=[]),\n    TupleNode(index=3, tuple_index=8, state_indices={3}, predecessors=[0], successors=[])\n  ],\n  node_indices_by_distance=[\n    [0],\n    [1, 2, 3]\n  ],\n  state_indices_by_distance=[\n    [0],\n    [1, 2, 3]\n  ]\n)");

    auto result2 = generate_state_space("domain.pddl", "p-2-0.pddl");
    auto state_space2 = result2.state_space;
    auto novelty_base_2 = std::make_shared<NoveltyBase>(
        state_space2->get_instance_info()->get_atoms().size(),
        2);

    EXPECT_EQ(TupleGraph(novelty_base_2, state_space2, 0).compute_repr(), "TupleGraph(\n  root_state_index=0,\n  tuple_nodes=[    TupleNode(index=0, tuple_index=0, state_indices={0}, predecessors=[], successors=[1, 4, 6, 10, 13]),\n    TupleNode(index=1, tuple_index=41, state_indices={1}, predecessors=[0], successors=[]),\n    TupleNode(index=2, tuple_index=79, state_indices={16}, predecessors=[3], successors=[]),\n    TupleNode(index=3, tuple_index=82, state_indices={12}, predecessors=[5, 7], successors=[2]),\n    TupleNode(index=4, tuple_index=92, state_indices={2}, predecessors=[0], successors=[5, 15]),\n    TupleNode(index=5, tuple_index=93, state_indices={6}, predecessors=[4], successors=[3]),\n    TupleNode(index=6, tuple_index=105, state_indices={4}, predecessors=[0], successors=[7, 12]),\n    TupleNode(index=7, tuple_index=106, state_indices={10}, predecessors=[6], successors=[3]),\n    TupleNode(index=8, tuple_index=131, state_indices={19}, predecessors=[9], successors=[]),\n    TupleNode(index=9, tuple_index=135, state_indices={14}, predecessors=[11, 14], successors=[8]),\n    TupleNode(index=10, tuple_index=144, state_indices={3}, predecessors=[0], successors=[11, 12]),\n    TupleNode(index=11, tuple_index=145, state_indices={8}, predecessors=[10], successors=[9]),\n    TupleNode(index=12, tuple_index=151, state_indices={9}, predecessors=[6, 10], successors=[]),\n    TupleNode(index=13, tuple_index=157, state_indices={5}, predecessors=[0], successors=[14, 15]),\n    TupleNode(index=14, tuple_index=158, state_indices={11}, predecessors=[13], successors=[9]),\n    TupleNode(index=15, tuple_index=163, state_indices={7}, predecessors=[4, 13], successors=[])\n  ],\n  node_indices_by_distance=[\n    [0],\n    [1, 4, 6, 10, 13],\n    [5, 7, 11, 12, 14, 15],\n    [3, 9],\n    [2, 8]\n  ],\n  state_indices_by_distance=[\n    [0],\n    [1, 2, 3, 4, 5],\n    [6, 7, 8, 9, 10, 11],\n    [12, 13, 14, 15],\n    [16, 17, 18, 19, 20, 21]\n  ]\n)");

    //std::ofstream file("out.dot");
    //const auto tg = TupleGraph(novelty_base_2, state_space2, 2);
    //file << tg.to_dot(1);
    //file.close();
    //std::cout << tg.compute_repr() << std::endl;
}

}
