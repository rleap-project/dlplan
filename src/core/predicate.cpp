#include "predicate.h"


namespace dlp {
namespace core {

PredicateImpl::PredicateImpl(const VocabularyInfo& vocabulary_info, const std::string& predicate_name, int predicate_idx, int arity)
    : m_vocabulary_info(&vocabulary_info), m_predicate_name(predicate_name), m_index(predicate_idx), m_arity(arity) { }

const VocabularyInfo* PredicateImpl::get_vocabulary_info() const {
    return m_vocabulary_info;
}

const std::string& PredicateImpl::get_name() const {
    return m_predicate_name;
}

int PredicateImpl::get_index() const {
    return m_index;
}

int PredicateImpl::get_arity() const {
    return m_arity;
}

}
}
