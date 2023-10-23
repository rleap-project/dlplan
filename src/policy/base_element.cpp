#include "include/dlplan/policy.h"

#include <sstream>

#include "include/dlplan/core.h"


namespace dlplan::policy {

NamedBaseElement::NamedBaseElement(const std::string& key) : m_key(key) { }

NamedBaseElement::NamedBaseElement(const NamedBaseElement& other) = default;

NamedBaseElement& NamedBaseElement::operator=(const NamedBaseElement& other) = default;

NamedBaseElement::NamedBaseElement(NamedBaseElement&& other) = default;

NamedBaseElement& NamedBaseElement::operator=(NamedBaseElement&& other) = default;

NamedBaseElement::~NamedBaseElement() = default;

const std::string& NamedBaseElement::get_key() const {
    return m_key;
}

}


namespace boost::serialization {
template<typename Archive>
void serialize(Archive& /* ar */ , dlplan::policy::NamedBaseElement& /* t */, const unsigned int /* version */ )
{
}

template<class Archive>
void save_construct_data(Archive& /* ar */ , const dlplan::policy::NamedBaseElement* /* t */ , const unsigned int /* version */ )
{
}

template<class Archive>
void load_construct_data(Archive& /* ar */ , dlplan::policy::NamedBaseElement* /* t */ , const unsigned int /* version */ )
{
}
}
