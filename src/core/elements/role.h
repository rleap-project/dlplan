#ifndef DLPLAN_SRC_CORE_ELEMENTS_ROLE_H_
#define DLPLAN_SRC_CORE_ELEMENTS_ROLE_H_

#include "element.h"


namespace dlplan::core::element {

class Role : public Element<RoleDenotation> {
public:
    Role(const VocabularyInfo& vocabulary, const std::string& name) : Element<RoleDenotation>(vocabulary, name) { }
    ~Role() override = default;
};

}

#endif
