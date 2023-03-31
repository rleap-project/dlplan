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
    ss << predicate.get_name() << "(";
    for (size_t i = 0; i < objects.size(); ++i) {
        const auto& object = objects[i];
        ss << object.get_name();
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

const Atom& InstanceInfo::add_atom(int predicate_idx, const Index_Vec& object_idxs, bool is_static) {
    // predicate related
    const Predicate& predicate = m_vocabulary_info->get_predicates()[predicate_idx];
    // object related
    std::vector<Object> objects;
    for (int object_idx : object_idxs) {
        objects.push_back(get_objects()[object_idx]);
    }
    return add_atom(predicate, objects, is_static);
}

const Atom& InstanceInfo::add_atom(const Predicate& predicate, const std::vector<Object>& objects, bool is_static) {
    if (predicate.get_arity() != static_cast<int>(objects.size())) {
        throw std::runtime_error("InstanceInfo::add_atom - predicate arity does not match the number of objects ("s + std::to_string(predicate.get_arity()) + " != " + std::to_string(objects.size()));
    }
    std::string name = compute_atom_name(predicate, objects);
    std::vector<int> object_idxs;
    std::for_each(objects.begin(), objects.end(), [&](const auto& object){ object_idxs.push_back(object.get_index()); });
    if (is_static) {
        Atom atom = Atom(name, m_static_atoms.size(), predicate.get_index(), object_idxs, is_static);
        auto result = m_static_atom_name_to_index.emplace(atom.get_name(), m_static_atoms.size());
        bool newly_inserted = result.second;
        if (!newly_inserted) {
            throw std::runtime_error("InstanceInfo::add_atom - atom with name ("s + atom.get_name() + ") already exists.");
        }
        m_static_atoms.push_back(std::move(atom));
        return m_static_atoms.back();
    } else {
        Atom atom = Atom(name, m_atoms.size(), predicate.get_index(), object_idxs, is_static);
        auto result = m_atom_name_to_index.emplace(atom.get_name(), m_atoms.size());
        bool newly_inserted = result.second;
        if (!newly_inserted) {
            return m_atoms[result.first->second];
        }
        m_atoms.push_back(std::move(atom));
        return m_atoms.back();
    }
}

const Atom& InstanceInfo::add_atom(const std::string &predicate_name, const std::vector<std::string> &object_names, bool is_static) {
    // predicate related
    int predicate_idx = m_vocabulary_info->get_predicate(predicate_name).get_index();
    // object related
    std::vector<int> object_idxs;
    for (int i = 0; i < static_cast<int>(object_names.size()); ++i) {
        const std::string& object_name = object_names[i];
        auto result = m_object_name_to_index.emplace(object_name, m_objects.size());
        int object_idx = result.first->second;
        bool newly_inserted = result.second;
        if (newly_inserted) {
            m_objects.push_back(Object(object_name, object_idx));
        }
        object_idxs.push_back(object_idx);
    }
    return add_atom(predicate_idx, object_idxs, is_static);
}


const Object& InstanceInfo::add_object(const std::string& object_name) {
    Object object = Object(object_name, m_objects.size());
    auto result = m_object_name_to_index.emplace(object.get_name(), m_objects.size());
    if (!result.second) {
        throw std::runtime_error("InstanceInfo::add_object - object with name ("s + object.get_name() + ") already exists.");
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

const Atom& InstanceInfo::add_atom(int predicate_idx, const std::vector<int>& object_idxs) {
    return add_atom(predicate_idx, object_idxs, false);
}

const Atom& InstanceInfo::add_static_atom(int predicate_idx, const std::vector<int>& object_idxs) {
    return add_atom(predicate_idx, object_idxs, true);
}

const Atom& InstanceInfo::add_atom(const std::string& predicate_name, const std::vector<std::string>& object_names) {
    return add_atom(predicate_name, object_names, false);
}

const Atom& InstanceInfo::add_static_atom(const std::string& predicate_name, const std::vector<std::string>& object_names) {
    return add_atom(predicate_name, object_names, true);
}

void InstanceInfo::set_index(int index) {
    m_index = index;
}

int InstanceInfo::get_index() const {
    return m_index;
}

const std::vector<Atom>& InstanceInfo::get_atoms() const {
    return m_atoms;
}

const std::vector<Atom>& InstanceInfo::get_static_atoms() const {
    return m_static_atoms;
}

const std::vector<Object>& InstanceInfo::get_objects() const {
    return m_objects;
}

std::shared_ptr<const VocabularyInfo> InstanceInfo::get_vocabulary_info() const {
    return m_vocabulary_info;
}

const Object& InstanceInfo::get_object(const std::string& name) const {
    if (m_object_name_to_index.count(name) == 0) {
        throw std::runtime_error("InstanceInfo::get_object - object " + name + " does not exist.");
    }
    return m_objects[m_object_name_to_index.at(name)];
}

const Atom& InstanceInfo::get_atom(const std::string& name) const {
    if (m_atom_name_to_index.count(name) == 0) {
        throw std::runtime_error("InstanceInfo::get_atom - atom " + name + " does not exist.");
    }
    return m_atoms[m_atom_name_to_index.at(name)];
}

}
