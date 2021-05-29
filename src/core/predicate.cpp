#include "predicate.h"


namespace dlp {
namespace core {

PredicateImpl::PredicateImpl(const std::string& predicate_name, unsigned predicate_idx, unsigned arity)
    : m_predicate_name(predicate_name), m_predicate_idx(predicate_idx), m_arity(arity) { }

}
}
