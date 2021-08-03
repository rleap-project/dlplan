#ifndef DLPLAN_SRC_CORE_OBJECT_H_
#define DLPLAN_SRC_CORE_OBJECT_H_

#include <string>


namespace dlplan::core {
class InstanceInfo;

class ObjectImpl {
private:
    const std::string m_name;
    const int m_index;

public:
    ObjectImpl(const std::string& name, int index);
    ~ObjectImpl() = default;

    const std::string& get_name() const;
    int get_index() const;
};

}

#endif
