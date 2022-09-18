#ifndef DLPLAN_INCLUDE_DLPLAN_UTILS_SEGMENTED_ARRAY_VECTOR_H_
#define DLPLAN_INCLUDE_DLPLAN_UTILS_SEGMENTED_ARRAY_VECTOR_H_

/**
 * Taken from fast-downward.org
 */

#include <vector>
#include <cassert>
#include <cstddef>
#include <iostream>


namespace dlplan::utils {

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
  time.
*/
template<class Element, class Allocator = std::allocator<Element>>
class SegmentedArrayVector {
    typedef typename Allocator::template rebind<Element>::other ElementAllocator;
    // TODO: Try to find a good value for SEGMENT_BYTES.
    static const size_t SEGMENT_BYTES = 8192;

    size_t elements_per_array;
    size_t arrays_per_segment;
    size_t elements_per_segment;

    ElementAllocator element_allocator;

    std::vector<Element *> segments;
    size_t the_size;

    size_t get_segment(size_t index) const {
        return index / arrays_per_segment;
    }

    size_t get_offset(size_t index) const {
        return (index % arrays_per_segment) * elements_per_array;
    }

    void add_segment() {
        Element *new_segment = element_allocator.allocate(elements_per_segment);
        segments.push_back(new_segment);
    }

public:
    SegmentedArrayVector(size_t elements_per_array_)
        : elements_per_array(elements_per_array_),
          arrays_per_segment(
              std::max(SEGMENT_BYTES / (elements_per_array * sizeof(Element)), size_t(1))),
          elements_per_segment(elements_per_array * arrays_per_segment),
          the_size(0) {
    }

    SegmentedArrayVector(size_t elements_per_array_, const ElementAllocator &allocator_)
        : element_allocator(allocator_),
          elements_per_array(elements_per_array_),
          arrays_per_segment(
              std::max(SEGMENT_BYTES / (elements_per_array * sizeof(Element)), size_t(1))),
          elements_per_segment(elements_per_array * arrays_per_segment),
          the_size(0) {
    }

    SegmentedArrayVector(const SegmentedArrayVector<Element> &other)
        : elements_per_array(other.elements_per_array),
          arrays_per_segment(other.arrays_per_segment),
          elements_per_segment(other.elements_per_segment),
          element_allocator(other.element_allocator),
          the_size(other.the_size) {
            for (const Element* element : other.segments) {
                push_back(element);
            }
    }

    SegmentedArrayVector &operator=(const SegmentedArrayVector<Element> &other) {
        if (this != &other) {
            elements_per_array = other.elements_per_array;
            arrays_per_segment = other.arrays_per_segment;
            elements_per_segment = other.elements_per_segment;
            element_allocator = other.element_allocator;
            segments.clear();
            the_size = 0;
            for (const Element* element : other.segments) {
                push_back(element);
            }
        }
        return *this;
    }

    ~SegmentedArrayVector() {
        // TODO Factor out common code with SegmentedVector. In particular
        //      we could destroy the_size * elements_per_array elements here
        //      wihtout looping over the arrays first.
        for (size_t i = 0; i < the_size; ++i) {
            for (size_t offset = 0; offset < elements_per_array; ++offset) {
                element_allocator.destroy(operator[](i) + offset);
            }
        }
        for (size_t i = 0; i < segments.size(); ++i) {
            element_allocator.deallocate(segments[i], elements_per_segment);
        }
    }

    Element *operator[](size_t index) {
        assert(index < the_size);
        size_t segment = get_segment(index);
        size_t offset = get_offset(index);
        return segments[segment] + offset;
    }

    const Element *operator[](size_t index) const {
        assert(index < the_size);
        size_t segment = get_segment(index);
        size_t offset = get_offset(index);
        return segments[segment] + offset;
    }

    size_t size() const {
        return the_size;
    }

    void push_back(const Element *entry) {
        size_t segment = get_segment(the_size);
        size_t offset = get_offset(the_size);
        if (segment == segments.size()) {
            assert(offset == 0);
            // Must add a new segment.
            add_segment();
        }
        Element *dest = segments[segment] + offset;
        for (size_t i = 0; i < elements_per_array; ++i)
            element_allocator.construct(dest++, *entry++);
        ++the_size;
    }

    void pop_back() {
        for (size_t offset = 0; offset < elements_per_array; ++offset) {
            element_allocator.destroy(operator[](the_size - 1) + offset);
        }
        --the_size;
        // If the removed element was the last in its segment, the segment
        // is not removed (memory is not deallocated). This way a subsequent
        // push_back does not have to allocate the memory again.
    }

    void resize(size_t new_size, const Element *entry) {
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