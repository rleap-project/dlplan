#include "vocabulary_info.h"


namespace dlp {
namespace core {

static bool exists(const std::string& name, std::unordered_map<std::string, unsigned>& mapping) {
    auto f = mapping.find(name);
    return (f != mapping.end());
}

PredicateImpl VocabularyInfoImpl::add_predicate(const std::string &predicate_name, unsigned arity) {
    if (m_predicate_name_to_predicate_idx.find(predicate_name) != m_predicate_name_to_predicate_idx.end()) {
        throw std::runtime_error("VocabularyInfoImpl::add_predicate - predicate with name ("s + predicate_name + ") already exists.");
    }
    unsigned predicate_idx = m_predicates.size();
    m_predicates.push_back(PredicateImpl(predicate_name, predicate_idx, arity));
    m_predicate_name_to_predicate_idx.insert(std::make_pair(predicate_name, predicate_idx));
    return m_predicates.back();
}

bool VocabularyInfoImpl::exists_predicate_name(const std::string& name) const {
    return m_predicate_name_to_predicate_idx.find(name) != m_predicate_name_to_predicate_idx.end();
}

unsigned VocabularyInfoImpl::get_predicate_idx(const std::string& name) const {
    return m_predicate_name_to_predicate_idx.at(name);
}

const PredicateImpl& VocabularyInfoImpl::get_predicate(unsigned predicate_idx) const {
    return m_predicates[predicate_idx];
}

const std::vector<PredicateImpl>& VocabularyInfoImpl::get_predicates() const {
    return m_predicates;
}


}
}
