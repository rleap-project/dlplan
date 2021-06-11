#ifndef DLP_SRC_CORE_CONSTANT_H_
#define DLP_SRC_CORE_CONSTANT_H_

#include <string>

#include "../../include/dlp/core.h"


namespace dlp {
namespace core {

class ConstantImpl {
private:
    const VocabularyInfo* m_vocabulary_info;
    const std::string m_name;
    const int m_index;

public:
    ConstantImpl(const VocabularyInfo& vocabulary_info, const std::string name, int index);
    ~ConstantImpl() = default;

    const VocabularyInfo* get_vocabulary_info() const;
    int get_index() const;
    const std::string& get_name() const;
};

}
}

#endif
