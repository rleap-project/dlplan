#ifndef DLP_SRC_GENERATOR_ELEMENT_HASH_TABLE_H_
#define DLP_SRC_GENERATOR_ELEMENT_HASH_TABLE_H_

#include "types.h"


namespace dlp {
namespace core {
    class Concept;
    class Role;
    class Numerical;
    class Boolean;
}
namespace generator {

/**
 * Evaluates an element on a collection of states.
 */
template<typename D>
std::vector<D> evaluate(core::Element<D>& element, const States& states) {
    std::vector<D> result;
    result.reserve(states.size());
    for (const auto& state : states) {
        result.push_back(element.evaluate(state));
    }
    return result;
}


/**
 * ElementHashTable provides an interface for storing elements
 * while trying to maintain uniqueness of their evaluations.
 */
template<typename D>
class ElementHashTable {
public:
    virtual ~ElementHashTable() = default;

    virtual bool insert(core::Element<D>& element, const States& states) = 0;
};

}
}

#endif
