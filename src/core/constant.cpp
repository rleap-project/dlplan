#include "../../include/dlplan/core.h"


namespace dlplan::core {
Constant::Constant(const std::string& name, int index)
    : m_name(name), m_index(index) { }

Constant::Constant(const Constant& other) = default;

Constant& Constant::operator=(const Constant& other) = default;

Constant::Constant(Constant&& other) = default;

Constant& Constant::operator=(Constant&& other) = default;

Constant::~Constant() = default;

bool Constant::operator==(const Constant& other) const {
    return (get_name() == other.get_name()) && (get_index() == other.get_index());
}

bool Constant::operator!=(const Constant& other) const {
    return !(*this == other);
}

int Constant::get_index() const {
    return m_index;
}

const std::string& Constant::get_name() const {
    return m_name;
}

}
