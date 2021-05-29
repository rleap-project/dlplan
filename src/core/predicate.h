#ifndef DLP_SRC_CORE_PREDICATE_H_
#define DLP_SRC_CORE_PREDICATE_H_

#include <string>
#include <vector>

#include "types.h"

namespace dlp {
namespace core {
class InstanceInfoImpl;

/**
 * Predicate contains information regarding the predicates used to construct the atoms.
 */
struct PredicateImpl {
    const std::string m_predicate_name;
    const unsigned m_predicate_idx;
    const unsigned m_arity;

    PredicateImpl(const std::string& predicate_name, unsigned predicate_idx, unsigned arity);
    ~PredicateImpl() = default;
};

}
}

#endif
