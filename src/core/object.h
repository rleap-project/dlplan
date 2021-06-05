#ifndef DLP_SRC_CORE_OBJECT_H_
#define DLP_SRC_CORE_OBJECT_H_

#include <string>
#include <memory>


namespace dlp {
namespace core {
class InstanceInfoImpl;

class ObjectImpl {
private:
    const InstanceInfoImpl* m_instance_info;
    const std::string m_object_name;
    const int m_object_idx;

public:
    ObjectImpl(const InstanceInfoImpl& instance_info, const std::string& object_name, int object_idx);
    ~ObjectImpl() = default;

    const InstanceInfoImpl* get_instance_info() const;
    const std::string& get_object_name() const;
    int get_object_idx() const;
};

}
}

#endif
