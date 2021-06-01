#ifndef DLP_SRC_CORE_OBJECT_H_
#define DLP_SRC_CORE_OBJECT_H_

#include <string>


namespace dlp {
namespace core {


struct ObjectImpl {
    const std::string m_object_name;
    const unsigned m_object_idx;

    ObjectImpl(const std::string& object_name, unsigned object_idx);
    ~ObjectImpl() = default;
};

}
}

#endif
