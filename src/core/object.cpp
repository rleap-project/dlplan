#include "../../include/dlplan/core.h"


namespace dlplan::core {
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

const std::string& Object::get_name() const {
    return m_name;
}

ObjectIndex Object::get_index() const {
    return m_index;
}

}
