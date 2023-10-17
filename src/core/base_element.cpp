#include "include/dlplan/core.h"

#include <sstream>

#include <boost/serialization/base_object.hpp>


namespace dlplan::core {

BaseElement::BaseElement(std::shared_ptr<VocabularyInfo> vocabulary_info, ElementIndex index, bool is_static)
    : m_vocabulary_info(vocabulary_info), m_index(index), m_is_static(is_static) { }

BaseElement::~BaseElement() { }

std::string BaseElement::compute_repr() const {
    std::stringstream ss;
    compute_repr(ss);
    return ss.str();
}

std::ostream& operator<<(std::ostream& os, const BaseElement& element) {
    os << element.compute_repr();
    return os;
}

std::string BaseElement::str() const {
    return compute_repr();
}

ElementIndex BaseElement::get_index() const {
    return m_index;
}

std::shared_ptr<VocabularyInfo> BaseElement::get_vocabulary_info() const {
    return m_vocabulary_info;
}

bool BaseElement::is_static() const {
    return m_is_static;
}

}


namespace boost::serialization {
template<typename Archive>
void serialize(Archive& /* ar */ , dlplan::core::BaseElement& /* t */, const unsigned int /* version */ )
{
}

template<class Archive>
void save_construct_data(Archive& /* ar */ , const dlplan::core::BaseElement* /* t */ , const unsigned int /* version */ )
{
}

template<class Archive>
void load_construct_data(Archive& /* ar */ , dlplan::core::BaseElement* /* t */ , const unsigned int /* version */ )
{
}
}
