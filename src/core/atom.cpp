#include "../../include/dlplan/core.h"

#include <sstream>
#include <cassert>

#include "../utils/collections.h"


using namespace std::string_literals;

namespace dlplan::core {

Atom::Atom(
    const std::string& name,
    int index,
    const Predicate& predicate,
    const std::vector<Object> &objects,
    bool is_static)
    : m_name(name), m_index(index),
      m_predicate(predicate), m_objects(objects), m_is_static(is_static) { }

Atom::Atom(const Atom& other) = default;

Atom& Atom::operator=(const Atom& other) = default;

Atom::~Atom() = default;

bool Atom::operator==(const Atom& other) const {
    return (get_index() == other.get_index()) && (get_name() == other.get_name());
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

const Predicate& Atom::get_predicate() const {
    return m_predicate;
}

const std::vector<Object>& Atom::get_objects() const {
    return m_objects;
}

const Object& Atom::get_object(int pos) const {
    assert(utils::in_bounds(pos, m_objects));
    if (!utils::in_bounds(pos, m_objects)) {
        throw std::runtime_error("Out of bounds (" + str() + ")");
    }
    return m_objects[pos];
}

bool Atom::get_is_static() const {
    return m_is_static;
}

std::string Atom::str() const {
    return get_name();
}

}