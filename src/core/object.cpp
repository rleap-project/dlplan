#include "../../include/dlplan/core.h"

#include <sstream>


namespace dlplan::core {

Object::Object(ObjectIndex index, const std::string& name)
    : Base<Object>(index), m_name(name) { }

Object::Object(const Object& other) = default;

Object& Object::operator=(const Object& other) = default;

Object::Object(Object&& other) = default;

Object& Object::operator=(Object&& other) = default;

Object::~Object() = default;

bool Object::are_equal_impl(const Object& other) const {
    // remove index comparison when we use factory.
    return (get_index() == other.get_index()) && (get_name() == other.get_name());
}

void Object::str_impl(std::stringstream& out) const {
    out << "Object("
       << "index=" << m_index << ", "
       << "name=" << m_name
       << ")";
}

size_t Object::hash_impl() const {
    return hash_combine(m_name);
}

const std::string& Object::get_name() const {
    return m_name;
}

}
