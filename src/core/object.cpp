#include "../../include/dlplan/core.h"

#include <sstream>

#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/serialization/serialization.hpp>


namespace dlplan::core {
Object::Object(const std::string& name, ObjectIndex index)
    : m_name(name), m_index(index) { }

Object::Object(const Object& other) = default;

Object& Object::operator=(const Object& other) = default;

Object::Object(Object&& other) = default;

Object& Object::operator=(Object&& other) = default;

Object::~Object() = default;

bool Object::operator==(const Object& other) const {
    return (get_index() == other.get_index()) && (get_name() == other.get_name());
}

bool Object::operator!=(const Object& other) const {
    return !(*this == other);
}

std::string Object::compute_repr() const {
    std::stringstream ss;
    ss << "Object("
       << "index=" << m_index << ", "
       << "name=" << m_name
       << ")";
    return ss.str();
}

std::ostream& operator<<(std::ostream& os, const Object& object) {
    os << object.compute_repr();
    return os;
}

std::string Object::str() const {
    return compute_repr();
}

const std::string& Object::get_name() const {
    return m_name;
}

ObjectIndex Object::get_index() const {
    return m_index;
}

}


namespace boost::serialization {
template<typename Archive>
inline void serialize(Archive& /* ar */ , dlplan::core::Object& /* t */, const unsigned int /* version */) {
}

template<class Archive>
inline void save_construct_data(
    Archive & ar, const dlplan::core::Object* t, const unsigned int /* version */ ){
    ar << t->m_name;
    ar << t->m_index;
}

template<class Archive>
inline void load_construct_data(
    Archive & ar, dlplan::core::Object* t, const unsigned int /* version */ ){
    std::string name;
    dlplan::core::ObjectIndex index;
    ar >> name;
    ar >> index;
    ::new(t)dlplan::core::Object(name, index);
}

template void serialize(boost::archive::text_iarchive& ar,
    dlplan::core::Object& t, const unsigned int version);
template void serialize(boost::archive::text_oarchive& ar,
    dlplan::core::Object& t, const unsigned int version);
template void save_construct_data(boost::archive::text_oarchive& ar,
    const dlplan::core::Object* t, const unsigned int version);
template void load_construct_data(boost::archive::text_iarchive& ar,
    dlplan::core::Object* t, const unsigned int version);
}