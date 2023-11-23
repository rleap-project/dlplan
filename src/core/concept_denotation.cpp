#include "../../include/dlplan/core.h"

#include "../utils/logging.h"
#include "../../include/dlplan/utils/hash.h"
#include "../../include/dlplan/utils/dynamic_bitset.h"

#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>

#include <sstream>


namespace dlplan::core {
// we assign index undefined since we do not care
ConceptDenotation::ConceptDenotation(int num_objects)
    : Base<ConceptDenotation>(std::numeric_limits<int>::max()), m_num_objects(num_objects), m_data(DynamicBitset<unsigned>(num_objects)) { }

ConceptDenotation::ConceptDenotation(const ConceptDenotation& other) = default;

ConceptDenotation& ConceptDenotation::operator=(const ConceptDenotation& other) = default;

ConceptDenotation::ConceptDenotation(ConceptDenotation&& other) = default;

ConceptDenotation& ConceptDenotation::operator=(ConceptDenotation&& other) = default;

ConceptDenotation::~ConceptDenotation() = default;

bool ConceptDenotation::are_equal_impl(const ConceptDenotation& other) const {
    if (this != &other) {
        return this->m_data == other.m_data;
    }
    return true;
}

void ConceptDenotation::str_impl(std::stringstream& out) const {
    out << "ConceptDenotation("
       << "num_objects=" << m_num_objects << ", "
       << "object_indices=" << to_sorted_vector()
       << ")";
}

std::size_t ConceptDenotation::hash_impl() const {
    return m_data.hash();
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

int ConceptDenotation::get_num_objects() const {
    return m_num_objects;
}

}
