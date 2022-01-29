#include "state.h"

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

StateImpl::StateImpl(std::shared_ptr<const InstanceInfo> instance_info, const std::vector<Atom>& atoms)
    : StateImpl(instance_info, atoms, -1) { }

StateImpl::StateImpl(std::shared_ptr<const InstanceInfo> instance_info, const std::vector<Atom>& atoms, int index)
    : m_index(index), m_instance_info(instance_info), m_atom_idxs(convert_atoms(*instance_info, atoms)) { }

StateImpl::StateImpl(std::shared_ptr<const InstanceInfo> instance_info, const Index_Vec& atom_idxs)
    : StateImpl(instance_info, atom_idxs, -1) { }

StateImpl::StateImpl(std::shared_ptr<const InstanceInfo> instance_info, const Index_Vec& atom_idxs, int index)
    : m_index(index), m_instance_info(instance_info), m_atom_idxs(convert_atoms(*instance_info, atom_idxs)) { }


std::shared_ptr<const InstanceInfo> StateImpl::get_instance_info() const {
    return m_instance_info;
}

const Index_Vec& StateImpl::get_atom_idxs() const {
    return m_atom_idxs;
}

int StateImpl::get_index() const {
    return m_index;
}

void StateImpl::set_index(int index) {
    m_index = index;
}

std::string StateImpl::str() const {
    std::string res("{");
    for (int i=0, n=m_atom_idxs.size(); i<n; ++i) {
        const auto& atom = m_instance_info->get_atom(m_atom_idxs[i]);
        res += atom.get_name();
        if (i < n-1) res += ", ";
    }
    res += "}";
    return res;
}

size_t StateImpl::compute_hash() const {
    std::size_t seed = 0;
    utils::hashing::hash_combine(seed, std::hash<std::vector<int>>()(m_atom_idxs));
    utils::hashing::hash_combine(seed, m_instance_info->compute_hash());
    return seed;
}


}
