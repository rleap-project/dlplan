#ifndef DLPLAN_SRC_UTILS_SEGMENTED_ARRAY_VECTOR_H_
#define DLPLAN_SRC_UTILS_SEGMENTED_ARRAY_VECTOR_H_

/**
 * Taken from fast-downward.org
 */

#include <vector>
#include <cassert>


namespace dlplan::utils {

template<class Element, class Allocator = std::allocator<Element>>
class SegmentedArrayVector {
    typedef typename Allocator::template rebind<Element>::other ElementAllocator;
    // TODO: Try to find a good value for SEGMENT_BYTES.
    static const size_t SEGMENT_BYTES = 8192;

    const size_t elements_per_array;
    const size_t arrays_per_segment;
    const size_t elements_per_segment;

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

    // No implementation to forbid copies and assignment
    SegmentedArrayVector(const SegmentedArrayVector<Element> &);
    SegmentedArrayVector &operator=(const SegmentedArrayVector<Element> &);
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
