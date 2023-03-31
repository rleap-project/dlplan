#include "../../include/dlplan/core.h"

#include "../utils/collections.h"

#include <sstream>
#include <cassert>

using namespace std::string_literals;


namespace dlplan::core {

Atom::Atom(
    const std::string& name,
    int index,
    int predicate_idx,
    const std::vector<int> &object_idxs,
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

const std::string& Atom::get_name() const {
    return m_name;
}

int Atom::get_index() const {
    return m_index;
}

int Atom::get_predicate_index() const {
    return m_predicate_index;
}

const std::vector<int>& Atom::get_object_indices() const {
    return m_object_indices;
}


bool Atom::is_static() const {
    return m_is_static;
}

}