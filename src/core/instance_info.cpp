#include "instance_info.h"

namespace dlp {
namespace core {

static bool exists(const std::string& name, std::unordered_map<std::string, unsigned>& mapping) {
    auto f = mapping.find(name);
    return (f != mapping.end());
}

static unsigned insert_or_retrieve(const std::string& name, std::unordered_map<std::string, unsigned>& mapping) {
    auto f = mapping.find(name);
    if (f == mapping.end()) {
        mapping.insert(std::make_pair(name, mapping.size()));
        return mapping.size() - 1;
    }
    return f->second;
}

const AtomImpl& InstanceInfoImpl::add_atom(const std::string &predicate_name, const Name_Vec &object_names, bool is_static) {
    bool predicate_exists = exists(predicate_name, m_predicate_name_to_predicate_idx);
    if (!predicate_exists) {
        m_predicate_arities.push_back(object_names.size());
    }
    int atom_idx = m_atoms.size();
    int predicate_idx = insert_or_retrieve(predicate_name, m_predicate_name_to_predicate_idx);
    Index_Vec object_idxs;
    for (const std::string& object_name : object_names) {
        object_idxs.push_back(insert_or_retrieve(object_name, m_object_name_to_object_idx));
    }
    if (is_static) {
        m_static_atom_idxs.push_back(atom_idx);
    }
    m_atoms.emplace_back(AtomImpl(atom_idx, predicate_name, predicate_idx, object_names, object_idxs, is_static));
    return m_atoms.back();
}

bool InstanceInfoImpl::exists_predicate_name(const std::string& name) const {
    return m_predicate_name_to_predicate_idx.find(name) != m_predicate_name_to_predicate_idx.end();
}

bool InstanceInfoImpl::predicate_idx(const std::string& name) const {
    return m_predicate_name_to_predicate_idx.at(name);
}

unsigned InstanceInfoImpl::predicate_arity(unsigned predicate_idx) const {
    return m_predicate_arities[predicate_idx];
}

}
}
