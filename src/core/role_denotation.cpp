#include "../../include/dlplan/core.h"

#include "../utils/logging.h"
#include "../../include/dlplan/utils/hash.h"
#include "../../include/dlplan/utils/dynamic_bitset.h"

#include <sstream>


namespace dlplan::core {
// we assign index undefined since we do not care
RoleDenotation::RoleDenotation(int num_objects)
    : Base<RoleDenotation>(std::numeric_limits<int>::max()), m_num_objects(num_objects), m_data(DynamicBitset<unsigned>(num_objects * num_objects)) { }

RoleDenotation::RoleDenotation(const RoleDenotation& other) = default;

RoleDenotation& RoleDenotation::operator=(const RoleDenotation& other) = default;

RoleDenotation::RoleDenotation(RoleDenotation&& other) = default;

RoleDenotation& RoleDenotation::operator=(RoleDenotation&& other) = default;

RoleDenotation::~RoleDenotation() = default;

bool RoleDenotation::are_equal_impl(const RoleDenotation& other) const {
    if (this != &other) {
        return this->m_data == other.m_data;
    }
    return true;
}

void RoleDenotation::str_impl(std::stringstream& out) const {
    out << "RoleDenotation("
       << "num_objects=" << m_num_objects << ", "
       << "pairs_of_object_indices=" << to_sorted_vector()
       << ")";
}

std::size_t RoleDenotation::hash_impl() const {
    return m_data.hash();
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

int RoleDenotation::get_num_objects() const {
    return m_num_objects;
}

}
