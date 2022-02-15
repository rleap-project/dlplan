#include "../../include/dlplan/core.h"

#include <algorithm>
#include <stdexcept>

#include "instance_info.h"
#include "../utils/collections.h"
#include "../utils/hash_utils.h"


namespace dlplan::core {

static Index_Vec convert_atoms(const InstanceInfo& instance_info, const std::vector<Atom>& atoms) {
    for (const Atom& atom : atoms) {
        if (!instance_info.exists_atom(atom)) {
            throw std::runtime_error("State::convert_atoms - atom (" + atom.get_name() + ") does not exist in InstanceInfo.");
        }
    }
    Index_Vec atom_indices;
    atom_indices.reserve(atoms.size() + instance_info.get_static_atom_idxs().size());
    for (const auto& atom : atoms) {
        if (!atom.get_is_static()) {
            atom_indices.push_back(atom.get_index());
        }
    }
    atom_indices.insert(atom_indices.end(), instance_info.get_static_atom_idxs().begin(), instance_info.get_static_atom_idxs().end());
    return atom_indices;
}


static Index_Vec convert_atoms(const InstanceInfo& instance_info, const Index_Vec& atom_idxs) {
    if (!std::all_of(atom_idxs.begin(), atom_idxs.end(), [&](int atom_idx){ return utils::in_bounds(atom_idx, instance_info.get_atoms()); })) {
        throw std::runtime_error("State::convert_atoms - atom index out of range.");
    }
    Index_Vec atom_indices;
    atom_indices.reserve(atom_idxs.size() + instance_info.get_static_atom_idxs().size());
    const auto& atoms = instance_info.get_atoms();
    for (int atom_idx : atom_idxs) {
        if (!atoms[atom_idx].get_is_static()) {
            atom_indices.push_back(atom_idx);
        }
    }
    atom_indices.insert(atom_indices.end(), instance_info.get_static_atom_idxs().begin(), instance_info.get_static_atom_idxs().end());
    return atom_indices;
}


State::State(std::shared_ptr<const InstanceInfo> instance_info, const std::vector<Atom>& atoms)
    : m_instance_info(instance_info), m_atom_idxs(convert_atoms(*instance_info, atoms)) { }

State::State(std::shared_ptr<const InstanceInfo> instance_info, const Index_Vec& atom_idxs)
    : m_instance_info(instance_info), m_atom_idxs(convert_atoms(*instance_info, atom_idxs)) { }

State::State(const State&) = default;

State& State::operator=(const State&) = default;

State::~State() = default;

State::State(State&& other) = default;

State& State::operator=(State&& other) = default;

bool State::operator==(const State& other) const {
    return (get_atom_idxs() == other.get_atom_idxs()) && (get_instance_info() == other.get_instance_info());
}

bool State::operator!=(const State& other) const {
    return !(*this == other);
}

std::shared_ptr<const InstanceInfo> State::get_instance_info() const {
    return m_instance_info;
}

const Index_Vec& State::get_atom_idxs() const {
    return m_atom_idxs;
}


std::string State::str() const {
    std::string res("{");
    for (int i = 0; i < static_cast<int>(m_atom_idxs.size()); ++i) {
        const auto& atom = m_instance_info->get_atom(m_atom_idxs[i]);
        res += atom.str();
        if (i < static_cast<int>(m_atom_idxs.size()) - 1) {
            res += ", ";
        }
    }
    res += "}";
    return res;
}

size_t State::compute_hash() const {
    std::size_t seed = 0;
    utils::hashing::hash_combine(seed, std::hash<std::vector<int>>()(m_atom_idxs));
    utils::hashing::hash_combine(seed, m_instance_info);
    return seed;
}


}
