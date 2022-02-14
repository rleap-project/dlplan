#include "object.h"


namespace dlplan::core {

ObjectImpl::ObjectImpl(const std::string& name, int index)
    : m_name(name), m_index(index) { }

const std::string& ObjectImpl::get_name() const {
    return m_name;
}

int ObjectImpl::get_index() const {
    return m_index;
}

std::string ObjectImpl::str() const {
    return m_name;
}

}
