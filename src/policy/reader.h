#ifndef DLPLAN_SRC_POLICY_READER_H_
#define DLPLAN_SRC_POLICY_READER_H_

#include <string>
#include <memory>


namespace dlplan {
namespace core {
    class SyntacticElementFactory;
}
namespace policy {
class Policy;
class PolicyBuilder;


class PolicyReaderImpl {
public:
    std::shared_ptr<const Policy> read(const std::string& data, PolicyBuilder& builder, core::SyntacticElementFactory& factory) const;
};

}
}

#endif
