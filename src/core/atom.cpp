#include "../../include/dlplan/core.h"

#include "../utils/collections.h"

#include <sstream>
#include <cassert>

using namespace std::string_literals;


namespace dlplan::core {

Atom::Atom(
    const std::string& name,
    int index,
    const Predicate& predicate,
    const std::vector<Object> &objects,
    bool is_static)
    : m_name(name), m_index(index),
      m_predicate(predicate), m_objects(objects), m_is_static(is_static) {
}

Atom::Atom(const Atom& other) = default;

Atom& Atom::operator=(const Atom& other) = default;

Atom::Atom(Atom&& other) = default;

Atom& Atom::operator=(Atom&& other) = default;

Atom::~Atom() = default;

bool Atom::operator==(const Atom& other) const {
    // our construction ensures that there are not two atoms with same index and same root.
    return get_index() == other.get_index();
}

bool Atom::operator!=(const Atom& other) const {
    return !(*this == other);
}

const std::string& Atom::get_name_ref() const {
    return m_name;
}

int Atom::get_index() const {
    return m_index;
}

const Predicate& Atom::get_predicate_ref() const {
    return m_predicate;
}

const std::vector<Object>& Atom::get_objects_ref() const {
    return m_objects;
}

const Object& Atom::get_object_ref(int pos) const {
    assert(utils::in_bounds(pos, m_objects));
    if (!utils::in_bounds(pos, m_objects)) {
        throw std::runtime_error("Out of bounds (" + get_name_ref() + ")");
    }
    return m_objects[pos];
}

bool Atom::get_is_static() const {
    return m_is_static;
}

}