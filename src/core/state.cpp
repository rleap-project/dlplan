#include "state.h"
#include "instance_info.h"


namespace dlp {
namespace core {

StateImpl::StateImpl(const InstanceInfoImpl& instance_info, const Index_Vec& atoms)
    : m_instance_info(instance_info.shared_from_this()), m_atoms(atoms) { }

StateImpl::StateImpl(const InstanceInfoImpl& instance_info, Index_Vec&& atoms)
    : m_instance_info(instance_info.shared_from_this()), m_atoms(std::move(atoms)) { }

const std::shared_ptr<const InstanceInfoImpl> StateImpl::get_instance_info() const {
    return m_instance_info;
}

}
}
