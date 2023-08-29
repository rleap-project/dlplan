#include "count.h"

#include <boost/serialization/shared_ptr.hpp>
#include <boost/serialization/serialization.hpp>


namespace boost::serialization {

template<typename Archive, typename T>
void serialize(Archive& /* ar */ , dlplan::core::CountNumerical<T>& t, const unsigned int /* version */ )
{
    // boost::serialization::base_object<dlplan::core::Numerical>(t);
}

template<class Archive, typename T>
void save_construct_data(Archive & ar, const dlplan::core::CountNumerical<T>* t, const unsigned int /* version */ )
{
    ar << t->m_vocabulary_info;
    ar << t->m_index;
    ar << t->m_element;
}

template<class Archive, typename T>
void load_construct_data(Archive & ar, dlplan::core::CountNumerical<T>* t, const unsigned int /* version */ )
{
    std::shared_ptr<dlplan::core::VocabularyInfo> vocabulary;
    int index;
    std::shared_ptr<const T> element;
    ar >> vocabulary;
    ar >> index;
    ar >> element;
    ::new(t)dlplan::core::CountNumerical<T>(vocabulary, index, element);
}

template void serialize(boost::archive::text_iarchive& ar,
    dlplan::core::CountNumerical<dlplan::core::Concept>& t, const unsigned int version);
template void serialize(boost::archive::text_oarchive& ar,
    dlplan::core::CountNumerical<dlplan::core::Concept>& t, const unsigned int version);
template void save_construct_data(boost::archive::text_oarchive& ar,
    const dlplan::core::CountNumerical<dlplan::core::Concept>* t, const unsigned int version);
template void load_construct_data(boost::archive::text_iarchive& ar,
    dlplan::core::CountNumerical<dlplan::core::Concept>* t, const unsigned int version);

template void serialize(boost::archive::text_iarchive& ar,
    dlplan::core::CountNumerical<dlplan::core::Role>& t, const unsigned int version);
template void serialize(boost::archive::text_oarchive& ar,
    dlplan::core::CountNumerical<dlplan::core::Role>& t, const unsigned int version);
template void save_construct_data(boost::archive::text_oarchive& ar,
    const dlplan::core::CountNumerical<dlplan::core::Role>* t, const unsigned int version);
template void load_construct_data(boost::archive::text_iarchive& ar,
    dlplan::core::CountNumerical<dlplan::core::Role>* t, const unsigned int version);
}

BOOST_CLASS_EXPORT_IMPLEMENT(dlplan::core::BaseElement)
BOOST_CLASS_EXPORT_IMPLEMENT(dlplan::core::Numerical)
BOOST_CLASS_EXPORT_IMPLEMENT(dlplan::core::CountNumerical<dlplan::core::Concept>)
BOOST_CLASS_EXPORT_IMPLEMENT(dlplan::core::CountNumerical<dlplan::core::Role>)
