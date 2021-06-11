#include "constant.h"


namespace dlp {
namespace core {

ConstantImpl::ConstantImpl(const VocabularyInfo& vocabulary_info, const std::string name, int index)
    : m_vocabulary_info(&vocabulary_info), m_name(name), m_index(index) { }

const VocabularyInfo* ConstantImpl::get_vocabulary_info() const {
    return m_vocabulary_info;
}

int ConstantImpl::get_index() const {
    return m_index;
}

const std::string& ConstantImpl::get_name() const {
    return m_name;
}

}
}
