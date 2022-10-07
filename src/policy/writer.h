#ifndef DLPLAN_SRC_POLICY_WRITER_H_
#define DLPLAN_SRC_POLICY_WRITER_H_

#include <string>


namespace dlplan::policy {
class Policy;

class PolicyWriterImpl {
public:
    std::string write(const Policy& policy) const;
};

}

#endif
