#include "../../include/dlplan/core.h"

#include "../utils/collections.h"
#include "../utils/logging.h"

#include <sstream>
#include <cassert>

using namespace std::string_literals;


namespace dlplan::core {

Atom::Atom()
    : m_name(""), m_index(-1), m_predicate_index(-1), m_object_indices(ObjectIndices()), m_is_static(false) { }

Atom::Atom(
    const std::string& name,
    AtomIndex index,
    PredicateIndex predicate_idx,
    const ObjectIndices &object_idxs,
    bool is_static)
    : m_name(name), m_index(index),
      m_predicate_index(predicate_idx), m_object_indices(object_idxs), m_is_static(is_static) {
}

Atom::Atom(const Atom& other) = default;

Atom& Atom::operator=(const Atom& other) = default;

Atom::Atom(Atom&& other) = default;

Atom& Atom::operator=(Atom&& other) = default;

Atom::~Atom() = default;

bool Atom::operator==(const Atom& other) const {
    return (get_name() == other.get_name()) && (get_index() == other.get_index()) && (is_static() == other.is_static());
}

bool Atom::operator!=(const Atom& other) const {
    return !(*this == other);
}

std::string Atom::compute_repr() const {
    std::stringstream ss;
    ss << "Atom("
       << "index=" << m_index << ", "
       << "name=" << m_name << ", "
       << "predicate_index=" << m_predicate_index << ", "
       << "object_indices=" << m_object_indices << ", "
       << "is_static=" << m_is_static
       << ")";
    return ss.str();
}

std::ostream& operator<<(std::ostream& os, const Atom& atom) {
    os << atom.compute_repr();
    return os;
}

std::string Atom::str() const {
    return compute_repr();
}

const std::string& Atom::get_name() const {
    return m_name;
}

AtomIndex Atom::get_index() const {
    return m_index;
}

PredicateIndex Atom::get_predicate_index() const {
    return m_predicate_index;
}

const ObjectIndices& Atom::get_object_indices() const {
    return m_object_indices;
}


bool Atom::is_static() const {
    return m_is_static;
}

}