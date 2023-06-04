#include "../../include/dlplan/core.h"

#include "../utils/logging.h"

#include <sstream>


namespace dlplan::core {

RoleDenotation::RoleDenotation(int num_objects)
    : m_num_objects(num_objects), m_data(utils::DynamicBitset<unsigned>(num_objects * num_objects)) { }

RoleDenotation::RoleDenotation(const RoleDenotation& other) = default;

RoleDenotation& RoleDenotation::operator=(const RoleDenotation& other) = default;

RoleDenotation::RoleDenotation(RoleDenotation&& other) = default;

RoleDenotation& RoleDenotation::operator=(RoleDenotation&& other) = default;

RoleDenotation::~RoleDenotation() = default;

void RoleDenotation::const_iterator::seek_next() {
    int& i = m_indices.first;
    int& j = m_indices.second;
    int offset = i * m_num_objects;
    while (i < m_num_objects) {
        ++j;
        if (j == m_num_objects) {  // end of row is reached
            ++i;
            offset += m_num_objects;
            j = 0;
            if (i == m_num_objects) break;  // reached last row
        }
        if (m_data.test(offset + j)) break;
    }
    assert(offset + j <= static_cast<int>(m_data.size()));
}

RoleDenotation::const_iterator::const_iterator(const_reference data, int num_objects, bool end)
    : m_data(data), m_num_objects(num_objects), m_indices(end ? std::pair<size_t, size_t>(num_objects, 0) : std::pair<size_t, size_t>(0, -1)) {
    if (!end) seek_next();
}

bool RoleDenotation::const_iterator::operator!=(const const_iterator& other) const {
    return !(*this == other);
}

bool RoleDenotation::const_iterator::operator==(const const_iterator& other) const {
    return ((m_indices == other.m_indices) && (&m_data == &other.m_data));
}

const PairOfObjectIndices& RoleDenotation::const_iterator::operator*() const {
    return m_indices;
}

PairOfObjectIndices* RoleDenotation::const_iterator::operator->() {
    return &m_indices;
}

RoleDenotation::const_iterator RoleDenotation::const_iterator::operator++(int) {
    RoleDenotation::const_iterator prev = *this;
    seek_next();
    return prev;
}

RoleDenotation::const_iterator& RoleDenotation::const_iterator::operator++() {
    seek_next();
    return *this;
}

bool RoleDenotation::operator==(const RoleDenotation& other) const {
    if (this != &other) {
        return this->m_data == other.m_data;
    }
    return true;
}

bool RoleDenotation::operator!=(const RoleDenotation& other) const {
    return !(*this == other);
}

RoleDenotation& RoleDenotation::operator&=(const RoleDenotation& other) {
    m_data &= other.m_data;
    return *this;
}

RoleDenotation& RoleDenotation::operator|=(const RoleDenotation& other) {
    m_data |= other.m_data;
    return *this;
}

RoleDenotation& RoleDenotation::operator-=(const RoleDenotation& other) {
    m_data -= other.m_data;
    return *this;
}

RoleDenotation& RoleDenotation::operator~() {
    ~m_data;
    return *this;
}

RoleDenotation::const_iterator RoleDenotation::begin() const {
    return RoleDenotation::const_iterator(m_data, m_num_objects);
}

RoleDenotation::const_iterator RoleDenotation::end() const {
    return RoleDenotation::const_iterator(m_data, m_num_objects, true);
}

void RoleDenotation::set() {
    m_data.set();
}

bool RoleDenotation::contains(const PairOfObjectIndices& value) const {
    return m_data.test(value.first * m_num_objects + value.second);
}

void RoleDenotation::insert(const PairOfObjectIndices& value) {
    return m_data.set(value.first * m_num_objects + value.second);
}

void RoleDenotation::erase(const PairOfObjectIndices& value) {
    return m_data.reset(value.first * m_num_objects + value.second);
}

int RoleDenotation::size() const {
    return m_data.count();
}

bool RoleDenotation::empty() const {
    return m_data.none();
}

bool RoleDenotation::intersects(const RoleDenotation& other) const {
    return m_data.intersects(other.m_data);
}

bool RoleDenotation::is_subset_of(const RoleDenotation& other) const {
    return m_data.is_subset_of(other.m_data);
}

std::string RoleDenotation::compute_repr() const {
    std::stringstream ss;
    ss << "RoleDenotation("
       << "num_objects=" << m_num_objects << ", "
       << "pairs_of_object_indices=" << to_sorted_vector()
       << ")";
    return ss.str();
}

std::ostream& operator<<(std::ostream& os, const RoleDenotation& denotation) {
    os << denotation.compute_repr();
    return os;
}

std::string RoleDenotation::str() const {
    return compute_repr();
}

PairsOfObjectIndices RoleDenotation::to_sorted_vector() const {
    PairsOfObjectIndices result;
    result.reserve(m_num_objects * m_num_objects);
    for (const auto& pair_of_object_indices : *this) {
        result.push_back(pair_of_object_indices);
    }
    result.shrink_to_fit();
    return result;
}

std::size_t RoleDenotation::hash() const {
    return dlplan::core::hash<std::vector<unsigned>>()(m_data.get_blocks());
}

int RoleDenotation::get_num_objects() const {
    return m_num_objects;
}

}
