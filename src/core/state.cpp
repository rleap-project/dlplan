#include "state.h"


namespace dlp {
namespace core {

StateImpl::StateImpl(std::shared_ptr<InstanceInfoImpl> parent, const Index_Vec& atoms)
    : m_parent(parent), m_atoms(atoms) { }

StateImpl::StateImpl(std::shared_ptr<InstanceInfoImpl> parent, Index_Vec&& atoms)
    : m_parent(parent), m_atoms(std::move(atoms)) { }

}
}
