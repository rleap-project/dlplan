#include "../../include/dlplan/core.h"

#include <sstream>


namespace dlplan::core {

Object::Object(std::shared_ptr<const InstanceInfoRoot> root, const std::string& name, int index)
    : m_root(root), m_name(name), m_index(index) { }

Object::Object(const Object& other) = default;

Object& Object::operator=(const Object& other) = default;

Object::Object(Object&& other) = default;

Object& Object::operator=(Object&& other) = default;

Object::~Object() = default;

bool Object::operator==(const Object& other) const {
    // our construction ensures that there are not two objects with same index and same root.
    return (get_index() == other.get_index()) && (get_name() == other.get_name());
}

bool Object::operator!=(const Object& other) const {
    return !(*this == other);
}

const std::string& Object::get_name() const {
    return m_name;
}

int Object::get_index() const {
    return m_index;
}

std::shared_ptr<const InstanceInfoRoot> Object::get_instance_info_root() const {
    return m_root;
}

}
