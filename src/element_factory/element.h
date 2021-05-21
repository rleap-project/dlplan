#ifndef DLP_SRC_ELEMENT_FACTORY_ELEMENT_H_
#define DLP_SRC_ELEMENT_FACTORY_ELEMENT_H_

#include <string>

namespace dlp {

class Element {
public:
    Element();

    /**
     * Computes a hash value.
     */
    virtual std::size_t hash_value() const = 0;
};

}

#endif
