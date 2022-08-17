#include "../../include/dlplan/core.h"

#include <algorithm>
#include <stdexcept>

#include "instance_info.h"
#include "../utils/collections.h"
#include "../utils/hash_utils.h"


namespace dlplan::core {

State::State(std::shared_ptr<const InstanceInfo> instance_info, const std::vector<Atom>& atoms)
    : m_instance_info(instance_info) {
    if (!std::all_of(atoms.begin(), atoms.end(), [&](const auto& atom){ return instance_info->exists_atom(atom); })) {
        throw std::runtime_error("State::State - atom does not exist in InstanceInfo.");
    }
    if (!std::all_of(atoms.begin(), atoms.end(), [&](const auto& atom){ return !atom.get_is_static(); })) {
        throw std::runtime_error("State::State - static atom is not allowed in State.");
    }
    m_atom_idxs.reserve(atoms.size());
    for (const auto& atom : atoms) {
        int atom_idx = atom.get_index();
        int predicate_idx = atom.get_predicate().get_index();
        m_atom_idxs.push_back(atom_idx);
        m_per_predicate_idx_atom_idxs[predicate_idx].push_back(atom_idx);
    }
}

State::State(std::shared_ptr<const InstanceInfo> instance_info, const Index_Vec& atom_idxs)
    : m_instance_info(instance_info) {
    const auto& atoms = instance_info->get_atoms();
    if (!std::all_of(atom_idxs.begin(), atom_idxs.end(), [&](int atom_idx){ return utils::in_bounds(atom_idx, atoms); })) {
        throw std::runtime_error("State::State - atom index out of range.");
    }
    if (!std::all_of(atom_idxs.begin(), atom_idxs.end(), [&](int atom_idx){ return !atoms[atom_idx].get_is_static(); })) {
        throw std::runtime_error("State::State - static atom is not allowed in State.");
    }
    m_atom_idxs.reserve(atoms.size());
    for (int atom_idx : atom_idxs) {
        const auto& atom = atoms[atom_idx];
        int predicate_idx = atom.get_predicate().get_index();
        m_atom_idxs.push_back(atom_idx);
        m_per_predicate_idx_atom_idxs[predicate_idx].push_back(atom_idx);
    }
}

State::State(const State&) = default;

State& State::operator=(const State&) = default;

State::State(State&& other) = default;

State& State::operator=(State&& other) = default;

State::~State() = default;

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

const phmap::flat_hash_map<int, std::vector<int>>& State::get_per_predicate_idx_atom_idxs() const {
    return m_per_predicate_idx_atom_idxs;
}

std::string State::str() const {
    std::string res("{");
    for (int i = 0; i < static_cast<int>(m_atom_idxs.size()); ++i) {
        const auto& atom = m_instance_info->get_atom(m_atom_idxs[i]);
        res += atom.get_name();
        if (i < static_cast<int>(m_atom_idxs.size()) - 1) {
            res += ", ";
        }
    }
    res += "}";
    return res;
}

size_t State::compute_hash() const {
    std::array<uint32_t, 4> a;
    int total_size = (sizeof(int) * m_atom_idxs.size() + sizeof(std::shared_ptr<InstanceInfo>));
    char* data = new char[total_size];
    // copy atom_idxs
    char* cur_dest = data;
    const char* src = static_cast<const char*>(static_cast<const void*>(m_atom_idxs.begin().base()));
    size_t amount = sizeof(int) * m_atom_idxs.size();
    memcpy(cur_dest, src, amount);
    // copy instance_info ptr
    cur_dest = cur_dest + amount;
    src = static_cast<const char*>(static_cast<const void*>(m_instance_info.get()));
    amount = sizeof(std::shared_ptr<InstanceInfo>);
    memcpy(cur_dest, src, amount);
    MurmurHash3_x64_128(data, total_size, m_atom_idxs.size(), a.begin());
    delete[] data;
    return std::hash<std::array<uint32_t, 4>>()(a);
}

}
