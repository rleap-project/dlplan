#include "../../include/dlplan/core.h"

#include "../utils/collections.h"
#include "../../include/dlplan/utils/hashing.h"

#include <algorithm>
#include <stdexcept>
#include <sstream>


namespace dlplan::core {

State::State(std::shared_ptr<const InstanceInfo> instance_info, const std::vector<Atom>& atoms, int index)
    : m_instance_info(instance_info), m_index(index) {
    if (!std::all_of(atoms.begin(), atoms.end(), [&](const auto& atom){ return instance_info->exists_atom(atom); })) {
        throw std::runtime_error("State::State - atom does not exist in InstanceInfo.");
    }
    if (!std::all_of(atoms.begin(), atoms.end(), [&](const auto& atom){ return !atom.get_is_static(); })) {
        throw std::runtime_error("State::State - static atom is not allowed in State.");
    }
    m_atom_idxs.reserve(atoms.size());
    for (const auto& atom : atoms) {
        int atom_idx = atom.get_index();
        m_atom_idxs.push_back(atom_idx);
    }
}

State::State(std::shared_ptr<const InstanceInfo> instance_info, const Index_Vec& atom_idxs, int index)
    : m_instance_info(instance_info), m_atom_idxs(atom_idxs), m_index(index) {
    const auto& atoms = instance_info->get_atoms_ref();
    if (!std::all_of(atom_idxs.begin(), atom_idxs.end(), [&](int atom_idx){ return utils::in_bounds(atom_idx, atoms); })) {
        throw std::runtime_error("State::State - atom index out of range.");
    }
    if (!std::all_of(atom_idxs.begin(), atom_idxs.end(), [&](int atom_idx){ return !atoms[atom_idx].get_is_static(); })) {
        throw std::runtime_error("State::State - static atom is not allowed in State.");
    }
}

State::State(const State&) = default;

State& State::operator=(const State&) = default;

State::State(State&& other) = default;

State& State::operator=(State&& other) = default;

State::~State() = default;

bool State::operator==(const State& other) const {
    return (compute_sorted_atom_idxs() == other.compute_sorted_atom_idxs()) && (&get_instance_info_ref() == &other.get_instance_info_ref());
}

bool State::operator!=(const State& other) const {
    return !(*this == other);
}

const InstanceInfo& State::get_instance_info_ref() const {
    return *m_instance_info;
}

std::shared_ptr<const InstanceInfo> State::get_instance_info() const {
    return m_instance_info;
}

const Index_Vec& State::get_atom_idxs_ref() const {
    return m_atom_idxs;
}

Index_Vec State::compute_sorted_atom_idxs() const {
    Index_Vec sorted_atom_idxs(m_atom_idxs);
    std::sort(sorted_atom_idxs.begin(), sorted_atom_idxs.end());
    return sorted_atom_idxs;
}

int State::get_index() const {
    return m_index;
}

std::string State::str() const {
    std::stringstream result;
    result << "(instance index=" << get_instance_info_ref().get_index()
           << ", state index=" << get_index()
           << ", atoms={";
    for (size_t i = 0; i < m_atom_idxs.size(); ++i) {
        if (i != 0) {
            result << ", ";
        }
        const auto& atom = m_instance_info->get_atom_ref(m_atom_idxs[i]);
        result << atom.get_name_ref();
    }
    result << "})";
    return result.str();
}

size_t State::compute_hash() const {
    Index_Vec sorted_atom_idxs = compute_sorted_atom_idxs();
    size_t seed = sorted_atom_idxs.size();
    for (int atom_idx : sorted_atom_idxs) {
        utils::hash_combine(seed, atom_idx);
    }
    utils::hash_combine(seed, m_instance_info.get());
    return seed;
}

void State::set_index(int index) {
    m_index = index;
}

}
