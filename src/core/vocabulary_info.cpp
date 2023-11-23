#include "../../include/dlplan/core.h"

#include "../utils/collections.h"
#include "../utils/logging.h"

#include <sstream>

using namespace std::string_literals;


namespace dlplan::core {
// we assign index undefined since we do not care
VocabularyInfo::VocabularyInfo()
    : Base<VocabularyInfo>(std::numeric_limits<int>::max()) { }

VocabularyInfo::VocabularyInfo(const VocabularyInfo& other) = default;

VocabularyInfo& VocabularyInfo::operator=(const VocabularyInfo& other) = default;

VocabularyInfo::VocabularyInfo(VocabularyInfo&& other) = default;

VocabularyInfo& VocabularyInfo::operator=(VocabularyInfo&& other) = default;

VocabularyInfo::~VocabularyInfo() = default;

bool VocabularyInfo::are_equal_impl(const VocabularyInfo& other) const {
    if (this != &other) {
        return m_predicates == other.m_predicates
            && m_constants == other.m_constants;
    }
    return true;
}

void VocabularyInfo::str_impl(std::stringstream& out) const {
    out << "VocabularyInfo("
       << "constants=" << m_constants << ", "
       << "predicates=" << m_predicates
       << ")";
}

size_t VocabularyInfo::hash_impl() const {
    return hash_combine(
        hash_vector(m_predicates),
        hash_vector(m_constants));
}

const Predicate& VocabularyInfo::add_predicate(const std::string &predicate_name, int arity, bool is_static) {
    Predicate predicate = Predicate(m_predicates.size(), predicate_name, arity, is_static);
    auto result = m_predicate_name_to_index.emplace(predicate_name, m_predicates.size());
    if (!result.second) {
        return m_predicates[result.first->second];
    }
    m_predicates.push_back(std::move(predicate));
    return m_predicates.back();
}

const Constant& VocabularyInfo::add_constant(const std::string& constant_name) {
    Constant constant = Constant(m_constants.size(), constant_name);
    auto result = m_constant_name_to_index.emplace(constant_name, m_constants.size());
    if (!result.second) {
        return m_constants[result.first->second];
    }
    m_constants.push_back(std::move(constant));
    return m_constants.back();
}

const std::unordered_map<std::string, PredicateIndex>&
VocabularyInfo::get_predicates_mapping() const {
    return m_predicate_name_to_index;
}

const std::unordered_map<std::string, ConstantIndex>&
VocabularyInfo::get_constants_mapping() const {
    return m_constant_name_to_index;
}

const std::vector<Predicate>& VocabularyInfo::get_predicates() const {
    return m_predicates;
}

const std::vector<Constant>& VocabularyInfo::get_constants() const {
    return m_constants;
}

const Predicate& VocabularyInfo::get_predicate(const std::string& name) const {
    if (m_predicate_name_to_index.count(name) == 0) {
        throw std::runtime_error("VocabularyInfo::get_predicate - predicate " + name + " does not exist.");
    }
    return m_predicates[m_predicate_name_to_index.at(name)];
}

const Constant& VocabularyInfo::get_constant(const std::string& name) const {
    if (m_constant_name_to_index.count(name) == 0) {
        throw std::runtime_error("VocabularyInfo::get_constant - constant " + name + " does not exist.");
    }
    return m_constants[m_constant_name_to_index.at(name)];
}

}
