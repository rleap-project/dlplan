#include "../../include/dlplan/core.h"
#include "../../include/dlplan/utils/hash.h"

#include "../utils/logging.h"

#include <sstream>


namespace dlplan::core {

ConceptDenotation::ConceptDenotation(int num_objects)
    : m_num_objects(num_objects), m_data(utils::DynamicBitset<unsigned>(num_objects)) { }

ConceptDenotation::ConceptDenotation(const ConceptDenotation& other) = default;

ConceptDenotation& ConceptDenotation::operator=(const ConceptDenotation& other) = default;

ConceptDenotation::ConceptDenotation(ConceptDenotation&& other) = default;

ConceptDenotation& ConceptDenotation::operator=(ConceptDenotation&& other) = default;

ConceptDenotation::~ConceptDenotation() = default;

bool ConceptDenotation::operator==(const ConceptDenotation& other) const {
    if (this != &other) {
        return this->m_data == other.m_data;
    }
    return true;
}

bool ConceptDenotation::operator!=(const ConceptDenotation& other) const {
    return !(*this == other);
}

ConceptDenotation& ConceptDenotation::operator&=(const ConceptDenotation& other) {
    m_data &= other.m_data;
    return *this;
}

ConceptDenotation& ConceptDenotation::operator|=(const ConceptDenotation& other) {
    m_data |= other.m_data;
    return *this;
}

ConceptDenotation& ConceptDenotation::operator-=(const ConceptDenotation& other) {
    m_data -= other.m_data;
    return *this;
}

ConceptDenotation& ConceptDenotation::operator~() {
    ~m_data;
    return *this;
}

bool ConceptDenotation::contains(ObjectIndex value) const {
    assert(value >= 0 && value < m_num_objects);
    return m_data.test(value);
}

void ConceptDenotation::set() {
    m_data.set();
}

void ConceptDenotation::insert(ObjectIndex value) {
    assert(value >= 0 && value < m_num_objects);
    m_data.set(value);
}

void ConceptDenotation::erase(ObjectIndex value) {
    assert(value >= 0 && value < m_num_objects);
    m_data.reset(value);
}

int ConceptDenotation::size() const {
    return m_data.count();
}

bool ConceptDenotation::empty() const {
    return m_data.none();
}

bool ConceptDenotation::intersects(const ConceptDenotation& other) const {
    return m_data.intersects(other.m_data);
}

bool ConceptDenotation::is_subset_of(const ConceptDenotation& other) const {
    return m_data.is_subset_of(other.m_data);
}

std::string ConceptDenotation::compute_repr() const {
    std::stringstream ss;
    ss << "ConceptDenotation("
       << "num_objects=" << m_num_objects << ", "
       << "object_indices=" << to_sorted_vector()
       << ")";
    return ss.str();
}

std::ostream& operator<<(std::ostream& os, const ConceptDenotation& denotation) {
    os << denotation.compute_repr();
    return os;
}

std::string ConceptDenotation::str() const {
    return compute_repr();
}

ObjectIndices ConceptDenotation::to_vector() const {
    // In the case of bitset, the to_sorted_vector has best runtime complexity.
    return to_sorted_vector();
}

ObjectIndices ConceptDenotation::to_sorted_vector() const {
    ObjectIndices result;
    result.reserve(m_num_objects);
    for (ObjectIndex i = 0; i < m_num_objects; ++i) {
        if (m_data.test(i)) {
            result.push_back(i);
        }
    }
    result.shrink_to_fit();
    return result;
}

std::size_t ConceptDenotation::hash() const {
    return m_data.hash();
}

int ConceptDenotation::get_num_objects() const {
    return m_num_objects;
}


}