#ifndef DLP_SRC_CORE_ELEMENTS_ROLE_H_
#define DLP_SRC_CORE_ELEMENTS_ROLE_H_

#include "element.h"


namespace dlp {
namespace core {
namespace element {

class Role : public Element<RoleDenotation> {
public:
    Role(const VocabularyInfo& vocabulary, const std::string& name) : Element<RoleDenotation>(vocabulary, name) { }

    size_t size() const {
        return m_result.size();
    }
};

}
}
}

#endif
