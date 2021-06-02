#include "object.h"


namespace dlp {
namespace core {

ObjectImpl::ObjectImpl(const InstanceInfoImpl& parent, const std::string& object_name, unsigned object_idx)
    : m_parent(&parent), m_object_name(object_name), m_object_idx(object_idx) { }

}
}
