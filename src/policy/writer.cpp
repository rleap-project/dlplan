#include "writer.h"

#include "../../include/dlplan/policy.h"


namespace dlplan::policy {

std::string PolicyWriterImpl::write(const Policy& policy) const {
    return policy.str();
}

}
