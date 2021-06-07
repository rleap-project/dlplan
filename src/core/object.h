#ifndef DLP_SRC_CORE_OBJECT_H_
#define DLP_SRC_CORE_OBJECT_H_

#include <string>


namespace dlp {
namespace core {
class InstanceInfo;

class ObjectImpl {
private:
    const InstanceInfo* m_instance_info;
    const std::string m_object_name;
    const int m_object_idx;

public:
    ObjectImpl(const InstanceInfo& instance_info, const std::string& object_name, int object_idx);
    ~ObjectImpl() = default;

    const InstanceInfo* get_instance_info() const;
    const std::string& get_object_name() const;
    int get_object_idx() const;
};

}
}

#endif
