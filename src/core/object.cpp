#include "object.h"


namespace dlp {
namespace core {

ObjectImpl::ObjectImpl(const std::string& object_name, unsigned object_idx)
    : m_object_name(object_name), m_object_idx(object_idx) { }

}
}
