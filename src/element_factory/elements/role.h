#ifndef DLP_SRC_ELEMENT_FACTORY_ELEMENTS_ROLE_H_
#define DLP_SRC_ELEMENT_FACTORY_ELEMENTS_ROLE_H_

#include "../element.h"
#include "../types.h"

namespace dlp {

class RoleElement : public Element<Roles> {
public:
    RoleElement(bool goal, const Index_Vec& goal_atoms)
    : Element<Roles>(goal, goal_atoms) { }

    size_t size() const override {
        return m_result.size();
    }
};

}

#endif
