#ifndef DLPLAN_SRC_GENERATOR_ELEMENT_HASH_TABLE_LOSSLESS_H_
#define DLPLAN_SRC_GENERATOR_ELEMENT_HASH_TABLE_LOSSLESS_H_

#include <unordered_set>
#include <vector>

#include "element_hash_table.h"

#include "../../include/dlplan/core.h"


namespace dlplan {
namespace core {
    class Concept;
    class Role;
    class Numerical;
    class Boolean;
}
namespace generator {

/**
 * In this version, we do not compress the denotations.
 * TODO(dominik): Compress the denotations.
 */

template<typename D>
class ElementHashTableSha2 : public ElementHashTable<D> {
private:
    std::unordered_set<std::vector<D>> m_denotation_cache;

public:
    ElementHashTableLossLess() : ElementHashTable<D>() { }
    virtual ~ElementHashTableLossLess() = default;

    bool insert(core::Element<D>& element, const States& states) override {
        const std::vector<D>& denotation = evaluate(element, states);
        return m_denotation_cache.insert(denotation).second;
    }
};

}
}

#endif
