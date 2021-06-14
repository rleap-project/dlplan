#include "object.h"


namespace dlplan::core {

ObjectImpl::ObjectImpl(const InstanceInfo& instance_info, const std::string& name, int index)
    : m_instance_info(&instance_info), m_name(name), m_index(index) { }

const InstanceInfo* ObjectImpl::get_instance_info() const {
    return m_instance_info;
}

const std::string& ObjectImpl::get_name() const {
    return m_name;
}

int ObjectImpl::get_index() const {
    return m_index;
}


}
