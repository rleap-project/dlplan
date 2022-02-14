#include "predicate.h"


namespace dlplan::core {

PredicateImpl::PredicateImpl(const std::string& name, int index, int arity)
    : m_name(name), m_index(index), m_arity(arity) { }

const std::string& PredicateImpl::get_name() const {
    return m_name;
}

int PredicateImpl::get_index() const {
    return m_index;
}

int PredicateImpl::get_arity() const {
    return m_arity;
}

std::string PredicateImpl::str() const {
    return m_name + " " + std::to_string(m_arity);
}

}
