#!/usr/bin/python3

"""
Example illustrating the state_space component.
"""

from dlplan.state_space import generate_state_space


def main():
    state_space_2_1_0 = generate_state_space("domain.pddl", "instance_2_1_0.pddl").state_space
    print("State space of instance_2_1_0:")
    print(str(state_space_2_1_0))
    state_space_2_2_0 = generate_state_space("domain.pddl", "instance_2_1_0.pddl").state_space
    print("State space of instance_2_2_0:")
    print(str(state_space_2_2_0))
    print()

    goal_distances = state_space_2_1_0.compute_goal_distances()
    print("Goal distances of instance_2_1_0:")
    for state_index, distance in goal_distances.items():
        print("state_index=", state_index, "distance=", distance)

    print("Deadends:")
    for state_index in state_space_2_1_0.get_states().keys():
        if state_index not in goal_distances:
            print(state_index, end=" ")
    print()

    forward_distances = state_space_2_1_0.compute_distances(
        {state_space_2_1_0.get_initial_state_index()},
        forward=True,
        stop_if_goal=False)
    print("Forward distances from initial state of instance_2_1_0:")
    for state_index, distance in forward_distances.items():
        print("state_index=", state_index, "distance=", distance)


if __name__ == "__main__":
    main()
