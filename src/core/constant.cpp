#include "constant.h"


namespace dlplan {
namespace core {

ConstantImpl::ConstantImpl(const std::string name, int index)
    : m_name(name), m_index(index) { }

int ConstantImpl::get_index() const {
    return m_index;
}

const std::string& ConstantImpl::get_name() const {
    return m_name;
}

std::string ConstantImpl::str() const {
    return m_name;
}

}
}
