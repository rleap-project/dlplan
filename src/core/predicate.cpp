#include "../../include/dlplan/core.h"


namespace dlplan::core {
Predicate::Predicate(const std::string& name, int index, int arity, bool is_static)
    : m_name(name), m_index(index), m_arity(arity), m_is_static(is_static) { }

Predicate::Predicate(const Predicate& other) = default;

Predicate& Predicate::operator=(const Predicate& other) = default;

Predicate::Predicate(Predicate&& other) = default;

Predicate& Predicate::operator=(Predicate&& other) = default;

Predicate::~Predicate() = default;

bool Predicate::operator==(const Predicate& other) const {
    return (get_index() == other.get_index()) && (get_name() == other.get_name()) && (get_arity() == other.get_arity()) && (is_static() == other.is_static());
}

bool Predicate::operator!=(const Predicate& other) const {
    return !(*this == other);
}

const std::string& Predicate::get_name() const {
    return m_name;
}

int Predicate::get_index() const {
    return m_index;
}

int Predicate::get_arity() const {
    return m_arity;
}

bool Predicate::is_static() const {
    return m_is_static;
}

}