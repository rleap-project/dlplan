#include "atom.h"

#include <sstream>
#include <cassert>

#include "../utils/collections.h"

using namespace std::string_literals;

namespace dlp {
namespace core {

AtomImpl::AtomImpl(
    const InstanceInfo& instance_info,
    const std::string& name,
    int index,
    const Predicate& predicate,
    const std::vector<Object> &objects,
    bool is_static)
    : m_instance_info(&instance_info), m_name(name), m_index(index),
      m_predicate(predicate), m_objects(objects), m_is_static(is_static) { }

const InstanceInfo* AtomImpl::get_instance_info() const {
    return m_instance_info;
}

const std::string& AtomImpl::get_name() const {
    return m_name;
}

int AtomImpl::get_index() const {
    return m_index;
}

const Predicate& AtomImpl::get_predicate() const {
    return m_predicate;
}

const std::vector<Object>& AtomImpl::get_objects() const {
    return m_objects;
}

const Object& AtomImpl::get_object(int pos) const {
    assert(utils::in_bounds(pos, m_objects));
    if (!utils::in_bounds(pos, m_objects)) {
        throw std::runtime_error("Out of bounds (" + str() + ")");
    }
    return m_objects[pos];
}

bool AtomImpl::get_is_static() const {
    return m_is_static;
}

std::string AtomImpl::str() const {
    std::stringstream ss;
    ss << "<"
       << m_name << ","
       << m_index << ","
       << m_predicate.get_name() << ","
       << m_predicate.get_arity() << ","
       << m_objects.size() << ","
       << ">";
    return ss.str();
}

}
}
