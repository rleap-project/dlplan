import dlplan


def main():
    # Generate and read state space files.
    dlplan.StateSpaceGenerator().generate_state_space("domain.pddl", "instance_2_1_0.pddl")
    state_space_2_1_0 = dlplan.StateSpaceReader().read(None, 0)
    vocabulary_info = state_space_2_1_0.get_instance_info()->get_vocabulary_info()
    print("State space of instance_2_1_0:")
    state_space_2_1_0.print()
    # Generate and read state space files over same VocabularyInfo.
    dlplan.StateSpaceGenerator().generate_state_space("domain.pddl", "instance_2_1_0.pddl")
    state_space_2_2_0 = dlplan.StateSpaceReader().read(vocabulary_info, 1)
    print("State space of instance_2_2_0:")
    state_space_2_2_0.print()
    print()

    # Compute goal distances
    goal_distance_info = state_space_2_1_0.compute_goal_distance_information()
    print("Goal distance information of instance_2_1_0:")
    print("Goal distances:")
    for state_index, distance in goal_distance_info.get_goal_distances().items():
        print("state_index=", state_index, "distance=", distance)

    print("Deadends:")
    for deadend in goal_distance_info.get_deadend_state_indices():
        print(deadend, end=" ")
    print()

    # State information to obtain states by index
    state_information = state_space_2_1_0.compute_state_information()
    print("State information of instance_2_1_0:")
    for state_index in state_space_2_1_0.get_state_indices():
        state = state_information.get_state(state_index)
        print("state_index=", state.get_index(), " atoms=", str(state))

    # Compute forward distances from states with options forward=True, stop_if_goal=False
    forward_distances = state_space_2_1_0.compute_distances({state_space_2_1_0.get_initial_state_index()}, True, False)
    print("Forward distances from initial state of instance_2_1_0:")
    for state_index, distance in forward_distances.items():
        print("state_index=", state_index, "distance=", distance)


if __name__ == "__main__":
    main()
