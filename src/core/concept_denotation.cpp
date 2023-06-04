#include "../../include/dlplan/core.h"

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

void ConceptDenotation::const_iterator::seek_next() {
    while (++m_index < m_num_objects) {
        if (m_data.test(m_index)) break;
    }
}

ConceptDenotation::const_iterator::const_iterator(
    ConceptDenotation::const_iterator::const_reference data, int num_objects, bool end)
    : m_data(data), m_num_objects(num_objects), m_index(end ? num_objects : -1) {
    if (!end) seek_next();
}

bool ConceptDenotation::const_iterator::operator!=(const const_iterator& other) const {
    return !(*this == other);
}

bool ConceptDenotation::const_iterator::operator==(const const_iterator& other) const {
    return ((m_index == other.m_index) && (&m_data == &other.m_data));
}

const ObjectIndex& ConceptDenotation::const_iterator::operator*() const {
    return m_index;
}

ObjectIndex* ConceptDenotation::const_iterator::operator->() {
    return &m_index;
}

ConceptDenotation::const_iterator ConceptDenotation::const_iterator::operator++(int) {
    ConceptDenotation::const_iterator prev = *this;
    seek_next();
    return prev;
}

ConceptDenotation::const_iterator& ConceptDenotation::const_iterator::operator++() {
    seek_next();
    return *this;
}

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

ConceptDenotation::const_iterator ConceptDenotation::begin() const {
    return ConceptDenotation::const_iterator(m_data, m_num_objects);
}

ConceptDenotation::const_iterator ConceptDenotation::end() const {
    return ConceptDenotation::const_iterator(m_data, m_num_objects, true);
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

ObjectIndices ConceptDenotation::to_sorted_vector() const {
    ObjectIndices result;
    result.reserve(m_num_objects);
    for (int object_idx : *this) {
        result.push_back(object_idx);
    }
    result.shrink_to_fit();
    return result;
}

std::size_t ConceptDenotation::hash() const {
    return dlplan::core::hash<std::vector<unsigned>>()(m_data.get_blocks());
}

int ConceptDenotation::get_num_objects() const {
    return m_num_objects;
}


}