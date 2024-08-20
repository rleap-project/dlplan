#include "memory.h"

#include <cassert>
#include <iostream>

namespace dlplan::utils {
static char *extra_memory_padding = nullptr;

// Save default out-of-memory handler.
static void (*default_out_of_memory_handler)() = nullptr;
// Save current out-of-memory handler.
static void (*current_out_of_memory_handler)() = nullptr;

static void continuing_out_of_memory_handler() {
    release_extra_memory_padding();
    std::cout << "Failed to allocate memory. Released extra memory padding." << std::endl;
}

void reserve_extra_memory_padding(int memory_in_mb) {
    assert(!extra_memory_padding);
    assert(!current_out_of_memory_handler);

    extra_memory_padding = new char[memory_in_mb * 1024 * 1024];
    std::set_new_handler(continuing_out_of_memory_handler);
    current_out_of_memory_handler = continuing_out_of_memory_handler;
}

void release_extra_memory_padding() {
    assert(extra_memory_padding);
    assert(current_out_of_memory_handler);

    delete[] extra_memory_padding;
    extra_memory_padding = nullptr;
    std::set_new_handler(default_out_of_memory_handler);
    current_out_of_memory_handler = nullptr;
}

bool extra_memory_padding_is_reserved() {
    return extra_memory_padding;
}

}