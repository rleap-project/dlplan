#include "../../include/dlplan/core.h"
#include "../../include/dlplan/utils/hash.h"

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

PairsOfObjectIndices RoleDenotation::to_vector() const {
    // In the case of bitset, the to_sorted_vector has best runtime complexity.
    return to_sorted_vector();
}

PairsOfObjectIndices RoleDenotation::to_sorted_vector() const {
    PairsOfObjectIndices result;
    result.reserve(m_num_objects * m_num_objects);
    for (ObjectIndex i = 0; i < m_num_objects; ++i) {
        int offset = m_num_objects * i;
        for (ObjectIndex j = 0; j < m_num_objects; ++j) {
            if (m_data.test(offset + j)) {
                result.emplace_back(i, j);
            }
        }
    }
    result.shrink_to_fit();
    return result;
}

std::size_t RoleDenotation::hash() const {
    return m_data.hash();
}

int RoleDenotation::get_num_objects() const {
    return m_num_objects;
}

}
