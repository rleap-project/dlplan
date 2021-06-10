#include "state.h"

#include <algorithm>

#include "instance_info.h"
#include "../utils/collections.h"


namespace dlp {
namespace core {

static Index_Vec convert_atoms(const InstanceInfo& instance_info, const std::vector<Atom>& atoms) {
    if (!std::all_of(atoms.begin(), atoms.end(), [&](const Atom& atom){ return atom.get_instance_info() == &instance_info; })) {
        throw std::runtime_error("State::convert_atoms - atom does not belong to the same instance.");
    }
    Index_Vec atom_indices;
    atom_indices.reserve(atoms.size() + instance_info.get_static_atom_idxs().size());
    for (const auto& atom : atoms) {
        atom_indices.push_back(atom.get_index());
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
    atom_indices.insert(atom_indices.end(), atom_idxs.begin(), atom_idxs.end());
    atom_indices.insert(atom_indices.end(), instance_info.get_static_atom_idxs().begin(), instance_info.get_static_atom_idxs().end());
    return atom_indices;
}

static Index_Vec convert_atoms(const InstanceInfo& instance_info, const Name_Vec& atom_names) {
    Index_Vec atom_indices;
    atom_indices.reserve(atom_names.size() + instance_info.get_static_atom_idxs().size());
    for (const auto& atom_name : atom_names) {
        atom_indices.push_back(instance_info.get_atom_idx(atom_name));
    }
    atom_indices.insert(atom_indices.end(), instance_info.get_static_atom_idxs().begin(), instance_info.get_static_atom_idxs().end());
    return atom_indices;
}

StateImpl::StateImpl(std::shared_ptr<const InstanceInfo> instance_info, const std::vector<Atom>& atoms)
    : m_instance_info(instance_info), m_atom_idxs(convert_atoms(*instance_info, atoms)) { }

StateImpl::StateImpl(std::shared_ptr<const InstanceInfo> instance_info, const Index_Vec& atom_idxs)
    : m_instance_info(instance_info), m_atom_idxs(convert_atoms(*instance_info, atom_idxs)) { }

StateImpl::StateImpl(std::shared_ptr<const InstanceInfo> instance_info, const Name_Vec& atom_names)
    : m_instance_info(instance_info), m_atom_idxs(convert_atoms(*instance_info, atom_names)) { }

const InstanceInfo* StateImpl::get_instance_info() const {
    return m_instance_info.get();
}

const Index_Vec& StateImpl::get_atom_idxs() const {
    return m_atom_idxs;
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


}
}
