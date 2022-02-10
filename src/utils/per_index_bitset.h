#ifndef DLPLAN_INCLUDE_DLPLAN_PER_INDEX_BITSET_H_
#define DLPLAN_INCLUDE_DLPLAN_PER_INDEX_BITSET_H_

/**
 * Taken from fast-downward.org
 */

/**
 * An extended version with vector transformation and hashing.
 */

#include "per_index_array.h"

#include <cassert>
#include <limits>
#include <vector>


namespace dlplan::utils {


class BitsetMath {
public:
    using Block = unsigned int;

    static_assert(
        !std::numeric_limits<Block>::is_signed,
        "Block type must be unsigned");

    static const Block zeros = Block(0);
    // MSVC's bitwise negation always returns a signed type.
    static const Block ones = Block(~Block(0));
    static const int bits_per_block = std::numeric_limits<Block>::digits;

    static int compute_num_blocks(std::size_t num_bits);
    static std::size_t block_index(std::size_t pos);
    static std::size_t bit_index(std::size_t pos);
    static Block bit_mask(std::size_t pos);
};


class BitsetView {
    utils::ArrayView<BitsetMath::Block> data;
    size_t num_bits;

private:
    int count_bits_in_last_block() const;

    void zero_unused_bits();

public:
    BitsetView(utils::ArrayView<BitsetMath::Block> data, size_t num_bits);

    BitsetView(const BitsetView &other) = default;
    BitsetView &operator=(const BitsetView &other) = default;

    int count() const;
    bool none() const;

    void set();
    void reset();

    void set(const BitsetView& other);

    void set(int index);
    void reset(int index);
    bool test(int index) const;
    size_t size() const;

    BitsetView& operator&=(const BitsetView& other);

    BitsetView& operator|=(const BitsetView& other);

    BitsetView& operator-=(const BitsetView& other);

    BitsetView& operator~();

    bool intersects(const BitsetView &other) const;

    bool is_subset_of(const BitsetView &other) const;

    std::size_t compute_hash() const;
};


class PerIndexBitset {
    int num_bits_per_entry;
    utils::PerIndexArray<BitsetMath::Block> data;
public:
    explicit PerIndexBitset(const std::vector<bool> &default_bits);

    PerIndexBitset(const PerIndexBitset &) = delete;
    PerIndexBitset &operator=(const PerIndexBitset &) = delete;

    BitsetView operator[](int index);
};

}

#endif
