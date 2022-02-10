#include "per_index_bitset.h"

/**
 * Taken from fast-downward.org
 */

#include "hash_utils.h"


using namespace std;

namespace dlplan::utils {

int BitsetMath::compute_num_blocks(size_t num_bits) {
    return (num_bits + bits_per_block - 1) / bits_per_block;
}

size_t BitsetMath::block_index(size_t pos) {
    return pos / bits_per_block;
}

size_t BitsetMath::bit_index(size_t pos) {
    return pos % bits_per_block;
}

BitsetMath::Block BitsetMath::bit_mask(size_t pos) {
    return Block(1) << bit_index(pos);
}


BitsetView::BitsetView(utils::ArrayView<BitsetMath::Block> data, size_t num_bits) :
    data(data), num_bits(num_bits) {}

int BitsetView::count_bits_in_last_block() const {
    return BitsetMath::bit_index(num_bits);
}

void BitsetView::zero_unused_bits() {
    const int bits_in_last_block = count_bits_in_last_block();
    if (bits_in_last_block != 0) {
        assert(!data.empty());
        data.back() &= ~(BitsetMath::ones << bits_in_last_block);
    }
}

int BitsetView::count() const {
    int result = 0;
    for (std::size_t pos = 0; pos < num_bits; ++pos) {
        result += static_cast<int>(test(pos));
    }
    return result;
}

bool BitsetView::none() const {
    for (std::size_t i = 0; i < data.size(); ++i) {
        if (data[i]) return false;
    }
    return true;
}

void BitsetView::set() {
    std::fill(data.begin(), data.end(), BitsetMath::ones);
    zero_unused_bits();
}

void BitsetView::reset() {
    std::fill(data.begin(), data.end(), BitsetMath::zeros);
}

void BitsetView::set(const BitsetView& other) {
    assert(num_bits == other.num_bits);
    for (std::size_t i = 0; i < data.size(); ++i) {
        data[i] = other.data[i];
    }
}

void BitsetView::set(int index) {
    assert(index >= 0 && index < num_bits);
    int block_index = BitsetMath::block_index(index);
    data[block_index] |= BitsetMath::bit_mask(index);
}

void BitsetView::reset(int index) {
    assert(index >= 0 && index < num_bits);
    int block_index = BitsetMath::block_index(index);
    data[block_index] &= ~BitsetMath::bit_mask(index);
}

bool BitsetView::test(int index) const {
    assert(index >= 0 && index < num_bits);
    int block_index = BitsetMath::block_index(index);
    return (data[block_index] & BitsetMath::bit_mask(index)) != 0;
}

size_t BitsetView::size() const {
    return num_bits;
}

BitsetView& BitsetView::operator&=(const BitsetView& other) {
    assert(size() == other.size());
    for (std::size_t i = 0; i < data.size(); ++i) {
        data[i] &= other.data[i];
    }
    return *this;
}

BitsetView& BitsetView::operator|=(const BitsetView& other) {
    assert(size() == other.size());
    for (std::size_t i = 0; i < data.size(); ++i) {
        data[i] |= other.data[i];
    }
    return *this;
}

BitsetView& BitsetView::operator-=(const BitsetView& other) {
    assert(size() == other.size());
    for (std::size_t i = 0; i < data.size(); ++i) {
        data[i] = data[i] & ~other.data[i];
    }
    return *this;
}

BitsetView& BitsetView::operator~() {
    for (std::size_t i = 0; i < data.size(); ++i) {
        data[i] = ~data[i];
    }
    zero_unused_bits();
    return *this;
}

bool BitsetView::intersects(const BitsetView &other) const {
    assert(size() == other.size());
    for (std::size_t i = 0; i < data.size(); ++i) {
        if (data[i] & other.data[i])
            return true;
    }
    return false;
}

bool BitsetView::is_subset_of(const BitsetView &other) const {
    assert(size() == other.size());
    for (std::size_t i = 0; i < data.size(); ++i) {
        if (data[i] & ~other.data[i])
            return false;
    }
    return true;
}

std::size_t BitsetView::compute_hash() const {
    size_t seed = num_bits;
    for (std::size_t i = 0; i < data.size(); ++i) {
        utils::hashing::hash_combine(seed, data[i]);
    }
    return seed;
}


static vector<BitsetMath::Block> pack_bit_vector(const vector<bool> &bits) {
    int num_bits = bits.size();
    int num_blocks = BitsetMath::compute_num_blocks(num_bits);
    vector<BitsetMath::Block> packed_bits(num_blocks, 0);
    BitsetView bitset_view(utils::ArrayView<BitsetMath::Block>(packed_bits.data(), num_blocks), num_bits);
    for (int i = 0; i < num_bits; ++i) {
        if (bits[i]) {
            bitset_view.set(i);
        }
    }
    return packed_bits;
}


PerIndexBitset::PerIndexBitset(const vector<bool> &default_bits)
    : num_bits_per_entry(default_bits.size()),
      data(pack_bit_vector(default_bits)) {
}

BitsetView PerIndexBitset::operator[](int index) {
    return BitsetView(data[index], num_bits_per_entry);
}

}
