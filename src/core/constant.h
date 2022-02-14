#ifndef DLPLAN_SRC_CORE_CONSTANT_H_
#define DLPLAN_SRC_CORE_CONSTANT_H_

#include <string>

#include "../../include/dlplan/core.h"


namespace dlplan {
namespace core {

class ConstantImpl {
private:
    const std::string m_name;
    const int m_index;

public:
    ConstantImpl(const std::string name, int index);
    ~ConstantImpl() = default;

    int get_index() const;
    const std::string& get_name() const;

    std::string str() const;
};

}
}

#endif
