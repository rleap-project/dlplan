#ifndef DLP_SRC_CORE_PREDICATE_H_
#define DLP_SRC_CORE_PREDICATE_H_

#include <string>
#include <vector>
#include <memory>

#include "types.h"


namespace dlp {
namespace core {
class VocabularyInfoImpl;

/**
 * Predicate contains information regarding the predicates used to construct the atoms.
 */
struct PredicateImpl {
    const VocabularyInfoImpl* m_parent;
    const std::string m_predicate_name;
    const unsigned m_predicate_idx;
    const unsigned m_arity;

    PredicateImpl(const VocabularyInfoImpl& parent, const std::string& predicate_name, unsigned predicate_idx, unsigned arity);
    ~PredicateImpl() = default;
};

}
}

#endif
