#include "../../include/dlplan/core.h"

#include <sstream>
#include <iostream>


namespace dlplan::core {

Predicate::Predicate(std::shared_ptr<const VocabularyInfoRoot> root, const std::string& name, int index, int arity)
    : m_root(root), m_name(name), m_index(index), m_arity(arity) { }

Predicate::Predicate(const Predicate& other) = default;

Predicate& Predicate::operator=(const Predicate& other) = default;

Predicate::Predicate(Predicate&& other) = default;

Predicate& Predicate::operator=(Predicate&& other) = default;

Predicate::~Predicate() = default;

bool Predicate::operator==(const Predicate& other) const {
    return (get_index() == other.get_index()) && (get_name() == other.get_name() && get_arity() == other.get_arity());
}

bool Predicate::operator!=(const Predicate& other) const {
    return !(*this == other);
}

const std::string& Predicate::get_name() const {
    return m_name;
}

int Predicate::get_index() const {
    return m_index;
}

int Predicate::get_arity() const {
    return m_arity;
}

std::shared_ptr<const VocabularyInfoRoot> Predicate::get_vocabulary_info_root() const {
    return m_root;
}

}