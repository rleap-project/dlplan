#include "object.h"


namespace dlp {
namespace core {

ObjectImpl::ObjectImpl(const InstanceInfo& instance_info, const std::string& object_name, int object_idx)
    : m_instance_info(&instance_info), m_object_name(object_name), m_object_idx(object_idx) { }

const InstanceInfo* ObjectImpl::get_instance_info() const {
    return m_instance_info;
}

const std::string& ObjectImpl::get_object_name() const {
    return m_object_name;
}

int ObjectImpl::get_object_idx() const {
    return m_object_idx;
}


}
}
