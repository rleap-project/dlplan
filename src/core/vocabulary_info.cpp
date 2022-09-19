#include "vocabulary_info.h"

#include "instance_info.h"
#include "element_factory.h"

#include "../utils/collections.h"
#include "../utils/hash_utils.h"

namespace dlplan::core {

VocabularyInfoImpl::VocabularyInfoImpl() { }

const Predicate& VocabularyInfoImpl::add_predicate(const std::string &predicate_name, int arity) {
    Predicate predicate = Predicate(predicate_name, m_predicates.size(), arity);
    auto result = m_predicate_name_to_predicate_idx.emplace(predicate_name, m_predicates.size());
    if (!result.second) {
        return m_predicates[result.first->second];
    }
    m_predicates.push_back(std::move(predicate));
    return m_predicates.back();
}

const Constant& VocabularyInfoImpl::add_constant(const std::string& constant_name) {
    Constant constant = Constant(constant_name, m_constants.size());
    auto result = m_constant_name_to_constant_idx.emplace(constant_name, m_constants.size());
    if (!result.second) {
        return m_constants[result.first->second];
    }
    m_constants.push_back(std::move(constant));
    return m_constants.back();
}

bool VocabularyInfoImpl::exists_predicate(const Predicate& predicate) const {
    if (!utils::in_bounds(predicate.get_index(), m_predicates)) {
        throw std::runtime_error("VocabularyInfoImpl::exists_predicate: predicate index out of range.");
    }
    return (m_predicates[predicate.get_index()] == predicate) ? true : false;
}

bool VocabularyInfoImpl::exists_predicate_name(const std::string& name) const {
    return m_predicate_name_to_predicate_idx.find(name) != m_predicate_name_to_predicate_idx.end();
}

const std::vector<Predicate>& VocabularyInfoImpl::get_predicates() const {
    return m_predicates;
}

int VocabularyInfoImpl::get_predicate_idx(const std::string& name) const {
    if (!exists_predicate_name(name)) {
        throw std::runtime_error("VocabularyInfoImpl::get_predicate_idx - no predicate with name ("s + name + ").");
    }
    return m_predicate_name_to_predicate_idx.at(name);
}

const Predicate& VocabularyInfoImpl::get_predicate_ref(int predicate_idx) const {
    if (!utils::in_bounds(predicate_idx, m_predicates)) {
        throw std::runtime_error("VocabularyInfoImpl::get_predicate - predicate index out of range.");
    }
    return m_predicates[predicate_idx];
}

bool VocabularyInfoImpl::exists_constant(const Constant& constant) const {
    if (!utils::in_bounds(constant.get_index(), m_constants)) {
        throw std::runtime_error("VocabularyInfoImpl::exists_constant: constant index out of range.");
    }
    // we only need to check the position with the corresponding index.
    return (m_constants[constant.get_index()] == constant) ? true : false;
}

bool VocabularyInfoImpl::exists_constant_name(const std::string& name) const {
    return m_constant_name_to_constant_idx.find(name) != m_constant_name_to_constant_idx.end();
}

int VocabularyInfoImpl::get_constant_idx(const std::string& name) const {
    if (!exists_constant_name(name)) {
        throw std::runtime_error("VocabularyInfoImpl::get_constant_idx - no constant with name ("s + name + ").");
    }
    return m_constant_name_to_constant_idx.at(name);
}

const Constant& VocabularyInfoImpl::get_constant(int constant_idx) const {
    if (!utils::in_bounds(constant_idx, m_constants)) {
        throw std::runtime_error("VocabularyInfoImpl::get_constant - constant index out of range.");
    }
    return m_constants[constant_idx];
}

const std::vector<Constant>& VocabularyInfoImpl::get_constants() const {
    return m_constants;
}

}
