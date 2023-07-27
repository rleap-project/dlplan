#include "../../include/dlplan/core.h"

#include <sstream>


namespace dlplan::core {
Object::Object()
    : m_name(""), m_index(-1) { }

Object::Object(const std::string& name, ObjectIndex index)
    : m_name(name), m_index(index) { }

Object::Object(const Object& other) = default;

Object& Object::operator=(const Object& other) = default;

Object::Object(Object&& other) = default;

Object& Object::operator=(Object&& other) = default;

Object::~Object() = default;

bool Object::operator==(const Object& other) const {
    return (get_index() == other.get_index()) && (get_name() == other.get_name());
}

bool Object::operator!=(const Object& other) const {
    return !(*this == other);
}

std::string Object::compute_repr() const {
    std::stringstream ss;
    ss << "Object("
       << "index=" << m_index << ", "
       << "name=" << m_name
       << ")";
    return ss.str();
}

std::ostream& operator<<(std::ostream& os, const Object& object) {
    os << object.compute_repr();
    return os;
}

std::string Object::str() const {
    return compute_repr();
}

const std::string& Object::get_name() const {
    return m_name;
}

ObjectIndex Object::get_index() const {
    return m_index;
}

}
