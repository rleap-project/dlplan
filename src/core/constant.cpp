#include "../../include/dlplan/core.h"

#include <sstream>


namespace dlplan::core {
Constant::Constant()
    : m_name(""), m_index(-1) { }

Constant::Constant(const std::string& name, ConstantIndex index)
    : m_name(name), m_index(index) { }

Constant::Constant(const Constant& other) = default;

Constant& Constant::operator=(const Constant& other) = default;

Constant::Constant(Constant&& other) = default;

Constant& Constant::operator=(Constant&& other) = default;

Constant::~Constant() = default;

std::string Constant::compute_repr() const {
    std::stringstream ss;
    ss << "Constant("
       << "index=" << m_index << ", "
       << "name=" << m_name
       << ")";
    return ss.str();
}

std::ostream& operator<<(std::ostream& os, const Constant& constant) {
    os << constant.compute_repr();
    return os;
}

std::string Constant::str() const {
    return compute_repr();
}

bool Constant::operator==(const Constant& other) const {
    return (get_name() == other.get_name()) && (get_index() == other.get_index());
}

bool Constant::operator!=(const Constant& other) const {
    return !(*this == other);
}

ConstantIndex Constant::get_index() const {
    return m_index;
}

const std::string& Constant::get_name() const {
    return m_name;
}

}
