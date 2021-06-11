#include "object.h"


namespace dlp::core {

ObjectImpl::ObjectImpl(const InstanceInfo& instance_info, const std::string& object_name, int object_idx)
    : m_instance_info(&instance_info), m_name(object_name), m_index(object_idx) { }

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
