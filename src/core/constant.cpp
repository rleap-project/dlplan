#include "../../include/dlplan/core.h"

#include <sstream>


namespace dlplan::core {

Constant::Constant(std::shared_ptr<const VocabularyInfoRoot> root, const std::string& name, int index)
    : m_root(root), m_name(name), m_index(index) { }

Constant::Constant(const Constant& other) = default;

Constant& Constant::operator=(const Constant& other) = default;

Constant::Constant(Constant&& other) = default;

Constant& Constant::operator=(Constant&& other) = default;

Constant::~Constant() = default;

bool Constant::operator==(const Constant& other) const {
    // our construction ensures that there are not two constants with same index and same root.
    return (get_vocabulary_info_root() == other.get_vocabulary_info_root() &&
        get_index() == other.get_index());
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

std::shared_ptr<const VocabularyInfoRoot> Constant::get_vocabulary_info_root() const {
    return m_root;
}

}
