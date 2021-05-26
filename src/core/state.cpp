#include "state.h"


namespace dlp {
namespace core {

StateImpl::StateImpl(const Index_Vec& atoms) : m_atoms(atoms) { }

StateImpl::StateImpl(Index_Vec&& atoms) : m_atoms(std::move(atoms)) { }

}
}
