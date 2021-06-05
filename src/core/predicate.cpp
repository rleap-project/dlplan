#include "predicate.h"


namespace dlp {
namespace core {

PredicateImpl::PredicateImpl(const VocabularyInfoImpl& parent, const std::string& predicate_name, unsigned predicate_idx, unsigned arity)
    : m_parent(&parent), m_predicate_name(predicate_name), m_predicate_idx(predicate_idx), m_arity(arity) { }

const VocabularyInfoImpl* PredicateImpl::get_parent() const {
    return m_parent;
}

}
}
