#include "../../include/dlplan/core.h"

#include "element_factory.h"

#include "../utils/collections.h"

using namespace std::string_literals;


namespace dlplan::core {

VocabularyInfo::VocabularyInfo() { }

VocabularyInfo::VocabularyInfo(const VocabularyInfo& other) = default;

VocabularyInfo& VocabularyInfo::operator=(const VocabularyInfo& other) = default;

VocabularyInfo::VocabularyInfo(VocabularyInfo&& other) = default;

VocabularyInfo& VocabularyInfo::operator=(VocabularyInfo&& other) = default;

VocabularyInfo::~VocabularyInfo() = default;

const Predicate& VocabularyInfo::add_predicate(const std::string &predicate_name, int arity, bool is_static) {
    Predicate predicate = Predicate(predicate_name, m_predicates.size(), arity, is_static);
    auto result = m_predicate_name_to_predicate_idx.emplace(predicate_name, m_predicates.size());
    if (!result.second) {
        return m_predicates[result.first->second];
    }
    m_predicates.push_back(std::move(predicate));
    return m_predicates.back();
}

const Constant& VocabularyInfo::add_constant(const std::string& constant_name) {
    Constant constant = Constant(constant_name, m_constants.size());
    auto result = m_constant_name_to_constant_idx.emplace(constant_name, m_constants.size());
    if (!result.second) {
        return m_constants[result.first->second];
    }
    m_constants.push_back(std::move(constant));
    return m_constants.back();
}

bool VocabularyInfo::exists_predicate(const Predicate& predicate) const {
    if (!utils::in_bounds(predicate.get_index(), m_predicates)) {
        throw std::runtime_error("VocabularyInfo::exists_predicate: predicate index out of range.");
    }
    return (m_predicates[predicate.get_index()] == predicate) ? true : false;
}

bool VocabularyInfo::exists_predicate_name(const std::string& name) const {
    return m_predicate_name_to_predicate_idx.find(name) != m_predicate_name_to_predicate_idx.end();
}

const std::vector<Predicate>& VocabularyInfo::get_predicates_ref() const {
    return m_predicates;
}

int VocabularyInfo::get_predicate_idx(const std::string& name) const {
    if (!exists_predicate_name(name)) {
        throw std::runtime_error("VocabularyInfo::get_predicate_idx - no predicate with name ("s + name + ").");
    }
    return m_predicate_name_to_predicate_idx.at(name);
}

const Predicate& VocabularyInfo::get_predicate_ref(int predicate_idx) const {
    if (!utils::in_bounds(predicate_idx, m_predicates)) {
        throw std::runtime_error("VocabularyInfo::get_predicate - predicate index out of range.");
    }
    return m_predicates[predicate_idx];
}

bool VocabularyInfo::exists_constant(const Constant& constant) const {
    if (!utils::in_bounds(constant.get_index(), m_constants)) {
        throw std::runtime_error("VocabularyInfo::exists_constant: constant index out of range.");
    }
    // we only need to check the position with the corresponding index.
    return (m_constants[constant.get_index()] == constant) ? true : false;
}

bool VocabularyInfo::exists_constant_name(const std::string& name) const {
    return m_constant_name_to_constant_idx.find(name) != m_constant_name_to_constant_idx.end();
}

int VocabularyInfo::get_constant_idx(const std::string& name) const {
    if (!exists_constant_name(name)) {
        throw std::runtime_error("VocabularyInfo::get_constant_idx - no constant with name ("s + name + ").");
    }
    return m_constant_name_to_constant_idx.at(name);
}

const Constant& VocabularyInfo::get_constant_ref(int constant_idx) const {
    if (!utils::in_bounds(constant_idx, m_constants)) {
        throw std::runtime_error("VocabularyInfo::get_constant - constant index out of range.");
    }
    return m_constants[constant_idx];
}

const std::vector<Constant>& VocabularyInfo::get_constants_ref() const {
    return m_constants;
}

}
