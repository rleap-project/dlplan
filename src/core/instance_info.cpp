#include "../../include/dlplan/core.h"

#include "../utils/collections.h"

#include <string>
#include <algorithm>
#include <iostream>
#include <sstream>

using namespace std::string_literals;


namespace dlplan::core {
static std::string compute_atom_name(const Predicate& predicate, const std::vector<Object>& objects) {
    std::stringstream ss;
    ss << predicate.get_name_ref() << "(";
    for (size_t i = 0; i < objects.size(); ++i) {
        const auto& object = objects[i];
        ss << object.get_name_ref();
        if (i < objects.size() - 1) ss << ",";
    }
    ss << ")";
    return ss.str();
}

InstanceInfo::InstanceInfo(std::shared_ptr<const VocabularyInfo> vocabulary_info, int index)
    : m_vocabulary_info(vocabulary_info), m_index(index) {
}

InstanceInfo::InstanceInfo(const InstanceInfo& other) = default;

InstanceInfo& InstanceInfo::operator=(const InstanceInfo& other) = default;

InstanceInfo::InstanceInfo(InstanceInfo&& other) = default;

InstanceInfo& InstanceInfo::operator=(InstanceInfo&& other) = default;

InstanceInfo::~InstanceInfo() = default;

const Atom& InstanceInfo::add_atom(const std::string &predicate_name, const Name_Vec &object_names, bool is_static) {
    if (m_vocabulary_info->get_predicate_ref(m_vocabulary_info->get_predicate_idx(predicate_name)).get_arity() != static_cast<int>(object_names.size())) {
        throw std::runtime_error("InstanceInfo::add_atom - predicate arity does not match the number of objects ("s + std::to_string(m_vocabulary_info->get_predicate_ref(m_vocabulary_info->get_predicate_idx(predicate_name)).get_arity()) + " != " + std::to_string(object_names.size()));
    }
    // predicate related
    int predicate_idx = m_vocabulary_info->get_predicate_idx(predicate_name);
    const Predicate& predicate = m_vocabulary_info->get_predicate_ref(predicate_idx);
    // object related
    std::vector<Object> objects;
    for (int i = 0; i < static_cast<int>(object_names.size()); ++i) {
        const std::string& object_name = object_names[i];
        auto result = m_object_name_to_object_idx.emplace(object_name, m_objects.size());
        int object_idx = result.first->second;
        bool newly_inserted = result.second;
        if (newly_inserted) {
            m_objects.push_back(Object(object_name, object_idx));
        }
        objects.push_back(m_objects[object_idx]);
    }
    return add_atom(predicate, objects, is_static);
}

const Atom& InstanceInfo::add_atom(const Predicate& predicate, const std::vector<Object>& objects, bool is_static) {
    if (predicate.get_arity() != static_cast<int>(objects.size())) {
        throw std::runtime_error("InstanceInfo::add_atom - predicate arity does not match the number of objects ("s + std::to_string(predicate.get_arity()) + " != " + std::to_string(objects.size()));
    }
    if (is_static) {
        Atom atom = Atom(compute_atom_name(predicate, objects), m_static_atoms.size(), predicate, objects, is_static);
        auto result = m_static_atom_name_to_static_atom_idx.emplace(atom.get_name_ref(), m_static_atoms.size());
        bool newly_inserted = result.second;
        if (!newly_inserted) {
            throw std::runtime_error("InstanceInfo::add_atom - atom with name ("s + atom.get_name_ref() + ") already exists.");
        }
        m_per_predicate_idx_static_atom_idxs[predicate.get_index()].push_back(atom.get_index());
        m_static_atoms.push_back(std::move(atom));
        return m_static_atoms.back();
    } else {
        Atom atom = Atom(compute_atom_name(predicate, objects), m_atoms.size(), predicate, objects, is_static);
        auto result = m_atom_name_to_atom_idx.emplace(atom.get_name_ref(), m_atoms.size());
        bool newly_inserted = result.second;
        if (!newly_inserted) {
            return m_atoms[result.first->second];
        }
        m_atoms.push_back(std::move(atom));
        return m_atoms.back();
    }
}

const Object& InstanceInfo::add_object(const std::string& object_name) {
    Object object = Object(object_name, m_objects.size());
    auto result = m_object_name_to_object_idx.emplace(object.get_name_ref(), m_objects.size());
    if (!result.second) {
        throw std::runtime_error("InstanceInfo::add_object - object with name ("s + object.get_name_ref() + ") already exists.");
    }
    m_objects.push_back(std::move(object));
    return m_objects.back();
}

const Atom& InstanceInfo::add_atom(const Predicate& predicate, const std::vector<Object>& objects) {
    return add_atom(predicate, objects, false);
}

const Atom& InstanceInfo::add_static_atom(const Predicate& predicate, const std::vector<Object>& objects) {
    return add_atom(predicate, objects, true);
}

const Atom& InstanceInfo::add_atom(const std::string &predicate_name, const Name_Vec &object_names) {
    return add_atom(predicate_name, object_names, false);
}

const Atom& InstanceInfo::add_static_atom(const std::string& predicate_name, const Name_Vec& object_names) {
    return add_atom(predicate_name, object_names, true);
}

void InstanceInfo::set_index(int index) {
    m_index = index;
}

int InstanceInfo::get_index() const {
    return m_index;
}

bool InstanceInfo::exists_atom(const Atom& atom) const {
    if (!utils::in_bounds(atom.get_index(), m_atoms)) {
        throw std::runtime_error("InstanceInfo::exists_atom: atom index out of range.");
    }
    // we only need to check the position with the corresponding index.
    return (m_atoms[atom.get_index()] == atom) ? true : false;
}

const std::vector<Atom>& InstanceInfo::get_atoms_ref() const {
    return m_atoms;
}

const std::vector<Atom>& InstanceInfo::get_static_atoms_ref() const {
    return m_static_atoms;
}

const Atom& InstanceInfo::get_atom_ref(int atom_idx) const {
    if (!utils::in_bounds(atom_idx, m_atoms)) {
        throw std::runtime_error("InstanceInfo::get_atom - atom index out of range.");
    }
    return m_atoms[atom_idx];
}

int InstanceInfo::get_atom_idx(const std::string& name) const {
    if (m_atom_name_to_atom_idx.find(name) == m_atom_name_to_atom_idx.end()) {
        throw std::runtime_error("InstanceInfo::get_atom_idx - no atom with name ("s + name + ").");
    }
    return m_atom_name_to_atom_idx.at(name);
}

bool InstanceInfo::exists_object(const Object& object) const {
    if (!utils::in_bounds(object.get_index(), m_objects)) {
        throw std::runtime_error("InstanceInfo::exists_object: object index out of range.");
    }
    // we only need to check the position with the corresponding index.
    return (m_objects[object.get_index()] == object) ? true : false;
}

bool InstanceInfo::exists_object(const std::string name) const {
    return m_object_name_to_object_idx.find(name) != m_object_name_to_object_idx.end();
}

const std::vector<Object>& InstanceInfo::get_objects_ref() const {
    return m_objects;
}

const Object& InstanceInfo::get_object_ref(int object_idx) const {
    if (!utils::in_bounds(object_idx, m_objects)) {
        throw std::runtime_error("InstanceInfo::get_object - object index out of range.");
    }
    return m_objects[object_idx];
}

int InstanceInfo::get_object_idx(const std::string& object_name) const {
    if (m_object_name_to_object_idx.find(object_name) == m_object_name_to_object_idx.end()) {
        throw std::runtime_error("InstanceInfo::get_object_idx - no object with name ("s + object_name + ").");
    }
    return m_object_name_to_object_idx.at(object_name);
}

int InstanceInfo::get_num_objects() const {
    return m_objects.size();
}

const VocabularyInfo& InstanceInfo::get_vocabulary_info_ref() const {
    return *m_vocabulary_info;
}

std::shared_ptr<const VocabularyInfo> InstanceInfo::get_vocabulary_info() const {
    return m_vocabulary_info;
}

const phmap::flat_hash_map<int, std::vector<int>>& InstanceInfo::get_per_predicate_idx_static_atom_idxs_ref() const {
    return m_per_predicate_idx_static_atom_idxs;
}

}
