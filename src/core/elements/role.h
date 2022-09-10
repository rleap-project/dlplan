#ifndef DLPLAN_SRC_CORE_ELEMENTS_ROLE_H_
#define DLPLAN_SRC_CORE_ELEMENTS_ROLE_H_

#include "element.h"


namespace dlplan::core::element {

class Role : public Element<RoleDenotation> {
public:
    explicit Role(const VocabularyInfo& vocabulary) : Element<RoleDenotation>(vocabulary) { }
    ~Role() override = default;
};

}

#endif
