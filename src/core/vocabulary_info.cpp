#include "include/dlplan/core.h"

#include <sstream>

#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/serialization/unordered_map.hpp>

#include "src/utils/collections.h"
#include "src/utils/logging.h"

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
    auto result = m_predicate_name_to_index.emplace(predicate_name, m_predicates.size());
    if (!result.second) {
        return m_predicates[result.first->second];
    }
    m_predicates.push_back(std::move(predicate));
    return m_predicates.back();
}

const Constant& VocabularyInfo::add_constant(const std::string& constant_name) {
    Constant constant = Constant(constant_name, m_constants.size());
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

std::string VocabularyInfo::compute_repr() const {
    std::stringstream ss;
    ss << "VocabularyInfo("
       << "constants=" << m_constants << ", "
       << "predicates=" << m_predicates
       << ")";
    return ss.str();
}

std::ostream& operator<<(std::ostream& os, const VocabularyInfo& vocabulary) {
    os << vocabulary.compute_repr();
    return os;
}

std::string VocabularyInfo::str() const {
    return compute_repr();
}

}


namespace boost::serialization {
template<typename Archive>
void serialize( Archive& ar, dlplan::core::VocabularyInfo& t, const unsigned int /* version */ )
{
    ar & t.m_constants;
    ar & t.m_constant_name_to_index;
    ar & t.m_predicates;
    ar & t.m_predicate_name_to_index;
}

template void serialize(boost::archive::text_iarchive& ar,
    dlplan::core::VocabularyInfo& t, const unsigned int version);
template void serialize(boost::archive::text_oarchive& ar,
    dlplan::core::VocabularyInfo& t, const unsigned int version);
}
