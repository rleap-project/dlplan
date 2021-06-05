#ifndef DLP_SRC_CORE_OBJECT_H_
#define DLP_SRC_CORE_OBJECT_H_

#include <string>
#include <memory>


namespace dlp {
namespace core {
class InstanceInfoImpl;

struct ObjectImpl {
    const InstanceInfoImpl* m_parent;
    const std::string m_object_name;
    const unsigned m_object_idx;

    ObjectImpl(const InstanceInfoImpl& parent, const std::string& object_name, unsigned object_idx);
    ~ObjectImpl() = default;

    const InstanceInfoImpl* get_parent() const;
};

}
}

#endif
