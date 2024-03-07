#include "tuple_graph_builder.h"

#include <algorithm>


namespace dlplan::novelty {
/// @brief Prints a vector to the output stream.
template<typename T>
std::ostream& operator<<(
    std::ostream& os,
    const std::vector<T>& vec)
{
    os << "[";
    for (size_t i = 0; i < vec.size(); ++i)
    {
        if (i != 0)
        {
            os << ", ";
        }
        os << vec[i];
    }
    os << "]";

    return os;
}

/// @brief Prints a vector to the output stream.
template<typename T>
std::ostream& operator<<(
    std::ostream& os,
    const std::unordered_set<T>& set)
{
    os << "{";
    size_t i = 0;
    for (const auto element : set)
    {
        if (i != 0)
        {
            os << ", ";
        }
        os << element;
        ++i;
    }
    os << "}";

    return os;
}

/// @brief Prints a vector to the output stream.
template<typename T>
std::ostream& operator<<(
    std::ostream& os,
    const std::set<T>& set)
{
    os << "{";
    size_t i = 0;
    for (const auto element : set)
    {
        if (i != 0)
        {
            os << ", ";
        }
        os << element;
        ++i;
    }
    os << "}";

    return os;
}

StateIndices TupleGraphBuilder::compute_state_layer(
    const StateIndices& current_layer,
    StateIndicesSet& visited_state_indices)
{
    std::unordered_set<StateIndex> layer_set;
    const auto& successors = m_state_space->get_forward_successor_state_indices();

    for (const auto source_index : current_layer)
    {
        assert(visited_state_indices.count(source_index));
        const auto it = successors.find(source_index);

        if (it != successors.end())
        {
            for (const auto target_index : it->second)
            {
                if (!visited_state_indices.count(target_index))
                {
                    visited_state_indices.insert(target_index);
                    layer_set.insert(target_index);
                }
            }
        }
    }
    return StateIndices(layer_set.begin(), layer_set.end());
}

TupleIndicesSet TupleGraphBuilder::compute_novel_tuple_indices_layer(const StateIndices& curr_state_layer)
{
    TupleIndicesSet novel_tuples_set;

    for (const auto state_index : curr_state_layer)
    {
        const TupleIndices state_novel_tuples = m_novelty_table.compute_novel_tuple_indices(
            {},
            m_state_space->get_states().at(state_index).get_atom_indices());
        novel_tuples_set.insert(state_novel_tuples.begin(), state_novel_tuples.end());
        m_state_index_to_novel_tuple_indices.emplace(state_index, state_novel_tuples);

        for (const auto tuple_index : state_novel_tuples)
        {
            m_novel_tuple_index_to_state_indices[tuple_index].insert(state_index);
        }
    }
    TupleIndices novel_tuples = TupleIndices(novel_tuples_set.begin(), novel_tuples_set.end());
    m_novelty_table.insert_tuple_indices(novel_tuples, false);
    return novel_tuples_set;
}

std::unordered_map<TupleIndex, StateIndicesSet>
TupleGraphBuilder::extend_states(TupleIndex cur_node_index) const {
    std::unordered_map<TupleIndex, StateIndicesSet> extended;
    const auto& successors = m_state_space->get_forward_successor_state_indices();

    for (const auto source_index : m_nodes[cur_node_index].get_state_indices())
    {
        const auto it = successors.find(source_index);

        if (it != successors.end())
        {
            for (const auto target_index : it->second)
            {
                const auto it = m_state_index_to_novel_tuple_indices.find(target_index);

                if (it != m_state_index_to_novel_tuple_indices.end())
                {
                    for (const auto target_tuple_index : it->second)
                    {
                        extended[target_tuple_index].insert(source_index);
                    }
                }
            }
        }
    }
    return extended;
}


struct SetHash {
    size_t operator()(const StateIndicesSet& set) const {
        return hash_container(set);
    }
};


struct SetEqual {
    bool operator()(const StateIndicesSet& lhs, const StateIndicesSet& rhs) const {
        return lhs == rhs;
    }
};


TupleNodeIndices TupleGraphBuilder::compute_nodes_layer(
    const TupleIndicesSet& curr_novel_tuple_indices,
    TupleNodeIndices& prev_tuple_layer)
{
    TupleIndicesSet curr_extended_tuple_indices;
    std::unordered_map<TupleIndex, TupleNodeIndicesSet> predecessor_tuple_nodes;
    for (auto& cur_node_index : prev_tuple_layer)
    {
        auto extended = extend_states(cur_node_index);

        TupleIndices extended_tuple_indices;
        for (const auto& [succ_tuple_index, cur_node_extended_states] : extended)
        {
            // Check whether all optimal plans for cur_node_index can be extended into optimal plans for succ_tuple_index
            if (curr_novel_tuple_indices.count(succ_tuple_index)
                && cur_node_extended_states.size() == m_nodes[cur_node_index].get_state_indices().size())
            {
                curr_extended_tuple_indices.insert(succ_tuple_index);

                predecessor_tuple_nodes[succ_tuple_index].insert(cur_node_index);
            }
        }
    }

    // Compute ordering "supset" t > t' iff S*(t) supset S*(t')
    std::unordered_map<TupleIndex, TupleIndicesSet> supset_subgoals;
    for (const auto tuple_index_1 : curr_extended_tuple_indices)
    {
        const auto& subgoals_1 = m_novel_tuple_index_to_state_indices.at(tuple_index_1);

        // create empty entry
        supset_subgoals[tuple_index_1] = TupleIndicesSet{};

        for (const auto tuple_index_2 : curr_extended_tuple_indices)
        {
            const auto& subgoals_2 = m_novel_tuple_index_to_state_indices.at(tuple_index_2);
            if ((subgoals_1.size() != subgoals_2.size())
                && std::includes(
                subgoals_1.begin(), subgoals_1.end(),
                subgoals_2.begin(), subgoals_2.end()))
            {
                supset_subgoals[tuple_index_1].insert(tuple_index_2);
            }
        }
    }

    // From each minimal element according to "supset" pick one representative
    std::unordered_map<StateIndicesSet, TupleIndex, SetHash, SetEqual> curr_tuple_indices;
    for (const auto& [tuple_index, supsets] : supset_subgoals)
    {
        if (supsets.empty())
        {
            const auto& subgoals = m_novel_tuple_index_to_state_indices.at(tuple_index);

            if (!curr_tuple_indices.count(subgoals))
            {
                curr_tuple_indices.emplace(subgoals, tuple_index);
            }
        }
    }

    // Create nodes
    TupleNodeIndices curr_tuple_layer;
    for (const auto& [_, tuple_index] : curr_tuple_indices)
    {
        auto node_index = m_nodes.size();
        const auto& subgoals = m_novel_tuple_index_to_state_indices.at(tuple_index);
        m_nodes.push_back(TupleNode(node_index, tuple_index, subgoals));
        curr_tuple_layer.push_back(node_index);

        for (const auto predecessor_node_index : predecessor_tuple_nodes[tuple_index])
        {
            m_nodes[predecessor_node_index].add_successor(node_index);
            m_nodes[node_index].add_predecessor(predecessor_node_index);
        }
    }

    return curr_tuple_layer;
}


void TupleGraphBuilder::build_width_equal_0_tuple_graph()
{
    TupleNodeIndex initial_node_index = m_nodes.size();
    m_node_indices_by_distance.push_back({initial_node_index});
    m_nodes.push_back({TupleNode(initial_node_index, initial_node_index, {m_root_state_index})});
    m_state_indices_by_distance.push_back({m_root_state_index});
    const auto& it = m_state_space->get_forward_successor_state_indices().find(m_root_state_index);

    if (it != m_state_space->get_forward_successor_state_indices().end())
    {
        TupleNodeIndices curr_tuple_layer;
        StateIndices curr_state_layer;

        for (const auto& target_index : it->second)
        {
            TupleNodeIndex node_index = m_nodes.size();
            curr_tuple_layer.push_back(node_index);
            auto tuple_node = TupleNode(node_index, node_index, {target_index});
            m_nodes.push_back(tuple_node);
            m_nodes[initial_node_index].add_successor(node_index);
            m_nodes[node_index].add_predecessor(initial_node_index);
            curr_state_layer.push_back(target_index);
        }
        m_node_indices_by_distance.push_back(curr_tuple_layer);
        m_state_indices_by_distance.push_back(curr_state_layer);
    }
}

void TupleGraphBuilder::build_width_greater_0_tuple_graph()
{
    StateIndicesSet visited_state_indices;

    // 1. Initialize root state with distance = 0
    m_state_indices_by_distance.push_back(StateIndices{m_root_state_index});
    TupleNodeIndex node_index = m_nodes.size();
    TupleIndex tuple_index = m_novelty_base->atom_indices_to_tuple_index({});
    TupleNode tuple_node = TupleNode(node_index, tuple_index, StateIndicesSet{m_root_state_index});
    m_nodes.push_back(tuple_node);
    m_node_indices_by_distance.push_back(TupleNodeIndices{node_index});
    TupleIndices tuple_indices = m_novelty_table.compute_novel_tuple_indices(m_state_space->get_states().at(m_root_state_index).get_atom_indices());
    m_novelty_table.insert_tuple_indices(tuple_indices, false);
    visited_state_indices.insert(m_root_state_index);

    // 2. Iterate distances > 0
    for (int distance = 1; ; ++distance)
    {
        StateIndices curr_state_layer = compute_state_layer(m_state_indices_by_distance[distance-1], visited_state_indices);
        auto novel_tuple_indices = compute_novel_tuple_indices_layer(curr_state_layer);

        if (novel_tuple_indices.empty())
        {
            break;
        }
        TupleNodeIndices curr_tuple_layer = compute_nodes_layer(novel_tuple_indices, m_node_indices_by_distance[distance-1]);

        if (curr_tuple_layer.empty())
        {
            break;
        }

        m_node_indices_by_distance.push_back(std::move(curr_tuple_layer));
        m_state_indices_by_distance.push_back(std::move(curr_state_layer));
    }
}

TupleGraphBuilder::TupleGraphBuilder(
    std::shared_ptr<const NoveltyBase> novelty_base,
    std::shared_ptr<const state_space::StateSpace> state_space,
    StateIndex root_state)
    : m_novelty_base(novelty_base),
      m_state_space(state_space),
      m_root_state_index(root_state),
      m_novelty_table(novelty_base)
{
    if (!m_novelty_base)
    {
        throw std::runtime_error("TupleGraphBuilder::TupleGraphBuilder - novelty_base is nullptr.");
    }

    if (!m_novelty_base)
    {
        throw std::runtime_error("TupleGraphBuilder::TupleGraphBuilder - state_space is nullptr.");
    }

    if (m_novelty_base->get_arity() == 0)
    {
        build_width_equal_0_tuple_graph();
    } else {
        build_width_greater_0_tuple_graph();
    }
}

TupleGraphBuilderResult TupleGraphBuilder::get_result()
{
    return TupleGraphBuilderResult{
        std::move(m_nodes),
        std::move(m_node_indices_by_distance),
        std::move(m_state_indices_by_distance)
    };
}

}