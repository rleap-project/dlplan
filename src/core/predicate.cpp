#include "../../include/dlplan/core.h"


namespace dlplan::core {

Predicate::Predicate(const std::string& name, int index, int arity)
    : m_name(name), m_index(index), m_arity(arity) { }

Predicate::~Predicate() { }

bool Predicate::operator==(const Predicate& other) const {
    return (get_index() == other.get_index()) && (get_name() == other.get_name() && get_arity() == other.get_arity());
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

std::string Predicate::str() const {
    return m_name + " " + std::to_string(m_arity);
}

}
