#include "../../include/dlplan/core.h"

#include <sstream>


namespace dlplan::core {

Constant::Constant(ConstantIndex index, const std::string& name)
    : Base<Constant>(index), m_name(name) { }

Constant::Constant(const Constant& other) = default;

Constant& Constant::operator=(const Constant& other) = default;

Constant::Constant(Constant&& other) = default;

Constant& Constant::operator=(Constant&& other) = default;

Constant::~Constant() = default;

bool Constant::are_equal_impl(const Constant& other) const {
    // remove index comparison when we use factory.
    return (get_name() == other.get_name()) && (get_index() == other.get_index());
}

void Constant::str_impl(std::stringstream& out) const {
    out << "Constant("
       << "index=" << m_index << ", "
       << "name=" << m_name
       << ")";
}

size_t Constant::hash_impl() const {
    return hash_combine(m_name, m_index);
}

const std::string& Constant::get_name() const {
    return m_name;
}

}
