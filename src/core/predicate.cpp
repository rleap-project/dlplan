#include "predicate.h"


namespace dlplan::core {

PredicateImpl::PredicateImpl(const VocabularyInfo& vocabulary_info, const std::string& name, int index, int arity)
    : m_vocabulary_info(&vocabulary_info), m_name(name), m_index(index), m_arity(arity) { }

const VocabularyInfo* PredicateImpl::get_vocabulary_info() const {
    return m_vocabulary_info;
}

const std::string& PredicateImpl::get_name() const {
    return m_name;
}

int PredicateImpl::get_index() const {
    return m_index;
}

int PredicateImpl::get_arity() const {
    return m_arity;
}

}
