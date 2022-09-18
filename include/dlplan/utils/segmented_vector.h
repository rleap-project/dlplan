#ifndef DLPLAN_INCLUDE_DLPLAN_UTILS_SEGMENTED_VECTOR_H_
#define DLPLAN_INCLUDE_DLPLAN_UTILS_SEGMENTED_VECTOR_H_

/**
 * Taken from fast-downward.org
 */

#include <algorithm>
#include <cassert>
#include <iostream>
#include <vector>

/*
  SegmentedVector is a vector-like class with the following advantages over
  vector:
    1. Resizing has no memory spike. (*)
    2. Should work more nicely with fragmented memory because data is
       partitioned into fixed-size chunks of size SEGMENT_BYTES.
    3. Overallocation is only additive (by SEGMENT_BYTES), not multiplicative
       as in vector. (*)
    4. References stay stable forever, so there is no need to be careful about
       invalidating references upon growing the vector.
  (*) Assumes that the size of the "segments" vector can be neglected, which is
  true if SEGMENT_BYTES isn't chosen too small. For example, with 1 GB of data
  and SEGMENT_BYTES = 8192, we can have 131072 segments.
  The main disadvantage to vector is that there is an additional indirection
  for each lookup, but we hope that the first lookup will usually hit the cache.
  The implementation is basically identical to that of deque (at least the
  g++ version), but with the advantage that we can control SEGMENT_BYTES. A
  test on all optimal planning instances with several planner configurations
  showed a modest advantage over deque.
  The class can also be used as a simple "memory pool" to reduce allocation
  costs (time and memory) when allocating many objects of the same type.
  SegmentedArrayVector is a similar class that can be used for compactly
  storing many fixed-size arrays. It's essentially a variant of SegmentedVector
  where the size of the stored data is only known at runtime, not at compile
  time. Note that we do not support 0-length arrays (checked with an assertion).
*/

// TODO: Get rid of the code duplication here. How to do it without
// paying a performance penalty? issue388.

// For documentation on classes relevant to storing and working with registered
// states see the file state_registry.h.

namespace dlplan::utils {
template<class Entry, class Allocator = std::allocator<Entry>>
class SegmentedVector {
    typedef typename Allocator::template rebind<Entry>::other EntryAllocator;
    // TODO: Try to find a good value for SEGMENT_BYTES.
    static const size_t SEGMENT_BYTES = 8192;

    static const size_t SEGMENT_ELEMENTS =
        (SEGMENT_BYTES / sizeof(Entry)) >= 1 ?
        (SEGMENT_BYTES / sizeof(Entry)) : 1;

    EntryAllocator entry_allocator;

    std::vector<Entry *> segments;
    size_t the_size;

    size_t get_segment(size_t index) const {
        return index / SEGMENT_ELEMENTS;
    }

    size_t get_offset(size_t index) const {
        return index % SEGMENT_ELEMENTS;
    }

    void add_segment() {
        Entry *new_segment = entry_allocator.allocate(SEGMENT_ELEMENTS);
        segments.push_back(new_segment);
    }

    SegmentedVector &operator=(const SegmentedVector<Entry> &);
    // SegmentedVector(const SegmentedVector<Entry> &);
public:
    SegmentedVector()
        : the_size(0) {
    }
    // We need copy constructor to be able to use this nested.
    SegmentedVector(const SegmentedVector<Entry> &other)
        : entry_allocator(other.entry_allocator),
          the_size(0) {
        for (size_t i = 0; i < the_size; ++i) {
            push_back(other[i]);
        }
    }

    SegmentedVector(const EntryAllocator &allocator_)
        : entry_allocator(allocator_),
          the_size(0) {
    }

    ~SegmentedVector() {
        for (size_t i = 0; i < the_size; ++i) {
            entry_allocator.destroy(&operator[](i));
        }
        for (size_t segment = 0; segment < segments.size(); ++segment) {
            entry_allocator.deallocate(segments[segment], SEGMENT_ELEMENTS);
        }
    }

    Entry &operator[](size_t index) {
        assert(index < the_size);
        size_t segment = get_segment(index);
        size_t offset = get_offset(index);
        return segments[segment][offset];
    }

    const Entry &operator[](size_t index) const {
        assert(index < the_size);
        size_t segment = get_segment(index);
        size_t offset = get_offset(index);
        return segments[segment][offset];
    }

    size_t size() const {
        return the_size;
    }

    void push_back(const Entry &entry) {
        size_t segment = get_segment(the_size);
        size_t offset = get_offset(the_size);
        if (segment == segments.size()) {
            assert(offset == 0);
            // Must add a new segment.
            add_segment();
        }
        entry_allocator.construct(segments[segment] + offset, entry);
        ++the_size;
    }

    void pop_back() {
        entry_allocator.destroy(&operator[](the_size - 1));
        --the_size;
        // If the removed element was the last in its segment, the segment
        // is not removed (memory is not deallocated). This way a subsequent
        // push_back does not have to allocate the memory again.
    }

    void resize(size_t new_size, Entry entry = Entry()) {
        // NOTE: We currently grow/shrink one element at a time.
        //       Revision 6ee5ff7b8873 contains an implementation that can
        //       handle other resizes more efficiently.
        while (new_size < the_size) {
            pop_back();
        }
        while (new_size > the_size) {
            push_back(entry);
        }
    }
};

}

#endif