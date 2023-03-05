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
    return (get_index() == other.get_index()) && (get_name_ref() == other.get_name_ref() && get_arity() == other.get_arity() && get_is_static() == other.get_is_static());
}

bool Predicate::operator!=(const Predicate& other) const {
    return !(*this == other);
}

const std::string& Predicate::get_name_ref() const {
    return m_name;
}

int Predicate::get_index() const {
    return m_index;
}

int Predicate::get_arity() const {
    return m_arity;
}

bool Predicate::get_is_static() const {
    return m_is_static;
}

}