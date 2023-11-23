#include "../../include/dlplan/core.h"

#include <sstream>


namespace dlplan::core {

Predicate::Predicate(PredicateIndex index, const std::string& name, int arity, bool is_static)
    : Base<Predicate>(index), m_name(name), m_arity(arity), m_is_static(is_static) { }

Predicate::Predicate(const Predicate& other) = default;

Predicate& Predicate::operator=(const Predicate& other) = default;

Predicate::Predicate(Predicate&& other) = default;

Predicate& Predicate::operator=(Predicate&& other) = default;

Predicate::~Predicate() = default;

bool Predicate::are_equal_impl(const Predicate& other) const {
    // remove index comparison when we use factory.
    return (get_index() == other.get_index()) && (get_name() == other.get_name()) && (get_arity() == other.get_arity()) && (is_static() == other.is_static());
}

void Predicate::str_impl(std::stringstream& out) const {
    out << "Predicate("
       << "index=" << m_index << ", "
       << "name=" << m_name << ", "
       << "arity=" << m_arity << ", "
       << "is_static=" << m_is_static
       << ")";
}

size_t Predicate::hash_impl() const {
    return hash_combine(m_name, m_index, m_arity, m_is_static);
}

const std::string& Predicate::get_name() const {
    return m_name;
}

int Predicate::get_arity() const {
    return m_arity;
}

bool Predicate::is_static() const {
    return m_is_static;
}

}
