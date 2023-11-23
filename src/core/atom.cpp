#include "../../include/dlplan/core.h"

#include "../utils/collections.h"
#include "../utils/logging.h"

#include <sstream>
#include <cassert>

using namespace std::string_literals;


namespace dlplan::core {

Atom::Atom(
    AtomIndex index,
    const std::string& name,
    PredicateIndex predicate_idx,
    const ObjectIndices &object_idxs,
    bool is_static)
    : Base<Atom>(index), m_name(name),
      m_predicate_index(predicate_idx), m_object_indices(object_idxs), m_is_static(is_static) {
}

Atom::Atom(const Atom& other) = default;

Atom& Atom::operator=(const Atom& other) = default;

Atom::Atom(Atom&& other) = default;

Atom& Atom::operator=(Atom&& other) = default;

Atom::~Atom() = default;

bool Atom::are_equal_impl(const Atom& other) const {
    // remove index comparison when we use factory.
    return (get_name() == other.get_name()) && (get_index() == other.get_index()) && (is_static() == other.is_static());
}

void Atom::str_impl(std::stringstream& out) const {
    out << "Atom("
       << "index=" << m_index << ", "
       << "name=" << m_name << ", "
       << "predicate_index=" << m_predicate_index << ", "
       << "object_indices=" << m_object_indices << ", "
       << "is_static=" << m_is_static
       << ")";
}

size_t Atom::hash_impl() const {
    return hash_combine(
        m_name,
        hash_vector(m_object_indices),
        m_is_static);
}

const std::string& Atom::get_name() const {
    return m_name;
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
