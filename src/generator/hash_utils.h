#ifndef DLPLAN_SRC_GENERATOR_HASH_UTILS_H_
#define DLPLAN_SRC_GENERATOR_HASH_UTILS_H_

#include <vector>
#include <array>

#include "../utils/MurmurHash3.h"


namespace dlplan::generator {


inline std::array<uint32_t, 4> compute_hash(const std::vector<int>& denotation) {
    std::array<uint32_t, 4> a;
    MurmurHash3_x86_128(denotation.begin().base(), sizeof(int) * denotation.size(), denotation.size(), a.begin());
    return a;
}

}


#endif
