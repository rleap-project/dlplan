#include "state.h"
#include "instance_info.h"


namespace dlp {
namespace core {

StateImpl::StateImpl(const InstanceInfoImpl& instance_info, const Index_Vec& atom_idxs)
    : m_instance_info(instance_info.shared_from_this()), m_atom_idxs(atom_idxs) { }

StateImpl::StateImpl(const InstanceInfoImpl& instance_info, Index_Vec&& atom_idxs)
    : m_instance_info(instance_info.shared_from_this()), m_atom_idxs(std::move(atom_idxs)) { }

const std::shared_ptr<const InstanceInfoImpl>& StateImpl::get_instance_info() const {
    return m_instance_info;
}

const Index_Vec& StateImpl::get_atom_idxs() const {
    return m_atom_idxs;
}

}
}
