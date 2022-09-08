#include "color.h"

#include "../../src/utils/MurmurHash3.h"


namespace dlplan::weisfeiler_lehman {

std::size_t Color::compute_hash() const {
    std::vector<int> data(begin(), end());
    std::size_t hash_value = 0;
    MurmurHash3_x64_128(data.begin().base(), sizeof(dlplan::weisfeiler_lehman::CompressedColor) * size(), size(), &hash_value);
    return hash_value;
}

}
