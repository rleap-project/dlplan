import dlplan


def main():
    # Generate and read state space files.
    state_space_2_1_0 = dlplan.generate_state_space("domain.pddl", "instance_2_1_0.pddl").state_space
    vocabulary_info = state_space_2_1_0.get_instance_info().get_vocabulary_info()
    print("State space of instance_2_1_0:")
    state_space_2_1_0.print()
    # Generate and read state space files over same VocabularyInfo.
    state_space_2_2_0 = dlplan.generate_state_space("domain.pddl", "instance_2_1_0.pddl").state_space
    print("State space of instance_2_2_0:")
    state_space_2_2_0.print()
    print()

    # Compute goal distances
    goal_distances = state_space_2_1_0.compute_goal_distances()
    print("Goal distances of instance_2_1_0:")
    for state_index, distance in goal_distances.items():
        print("state_index=", state_index, "distance=", distance)

    print("Deadends:")
    for state_index, state in state_space_2_1_0.get_states().items():
        if state_index not in goal_distances:
            print(state_index, end=" ")
    print()

    # Compute forward distances from states with options forward=True, stop_if_goal=False
    forward_distances = state_space_2_1_0.compute_distances({state_space_2_1_0.get_initial_state_index()}, True, False)
    print("Forward distances from initial state of instance_2_1_0:")
    for state_index, distance in forward_distances.items():
        print("state_index=", state_index, "distance=", distance)


if __name__ == "__main__":
    main()
