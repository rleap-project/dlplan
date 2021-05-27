#include "state.h"


namespace dlp {
namespace core {

StateImpl::StateImpl(const InstanceInfoImpl* parent, const Index_Vec& atoms)
    : m_parent(parent), m_atoms(atoms) { }

StateImpl::StateImpl(const InstanceInfoImpl* parent, Index_Vec&& atoms)
    : m_parent(parent), m_atoms(std::move(atoms)) { }

}
}
