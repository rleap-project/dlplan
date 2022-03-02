#include "../../include/dlplan/core.h"

#include <sstream>
#include <cassert>

#include "../utils/collections.h"


using namespace std::string_literals;

namespace dlplan::core {

Atom::Atom(
    std::shared_ptr<const InstanceInfoRoot> root,
    const std::string& name,
    int index,
    const Predicate& predicate,
    const std::vector<Object> &objects,
    bool is_static)
    : m_root(root), m_name(name), m_index(index),
      m_predicate(predicate), m_objects(objects), m_is_static(is_static) {
}

Atom::Atom(const Atom& other) = default;

Atom& Atom::operator=(const Atom& other) = default;

Atom::Atom(Atom&& other) = default;

Atom& Atom::operator=(Atom&& other) = default;

Atom::~Atom() = default;

bool Atom::operator==(const Atom& other) const {
    // our construction ensures that there are not two atoms with same index and same root.
    return (get_instance_info_root() == other.get_instance_info_root() &&
        get_index() == other.get_index());
}

bool Atom::operator!=(const Atom& other) const {
    return !(*this == other);
}

std::string Atom::get_name() const {
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
        throw std::runtime_error("Out of bounds (" + get_name() + ")");
    }
    return m_objects[pos];
}

bool Atom::get_is_static() const {
    return m_is_static;
}

std::shared_ptr<const InstanceInfoRoot> Atom::get_instance_info_root() const {
    return m_root;
}

}