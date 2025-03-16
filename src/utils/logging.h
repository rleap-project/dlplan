#ifndef DLPLAN_SRC_UTILS_LOGGING_H
#define DLPLAN_SRC_UTILS_LOGGING_H

/**
 * Taken from fast-downward.org
 */

#include "system.h"
#include "timer.h"

#include <iostream>
#include <ostream>
#include <string>
#include <vector>
#include <unordered_set>
#include <set>


namespace dlplan::utils {
/*
  Simple logger that prepends time and peak memory info to messages.
  Logs are written to stdout.
  Usage:
        utils::g_log << "States: " << num_states << endl;
*/
class Log {
private:
    bool line_has_started = false;
    const bool silent = true;

public:
    template<typename T>
    Log &operator<<(const T &elem) {
        if (silent) {
          return *this;
        }
        if (!line_has_started) {
            line_has_started = true;
            std::cout << "[t=" << g_timer << ", "
                      << get_peak_memory_in_kb() << " KB] ";
        }

        std::cout << elem;
        return *this;
    }

    using manip_function = std::ostream &(*)(std::ostream &);
    Log &operator<<(manip_function f) {
        if (f == static_cast<manip_function>(&std::endl)) {
            line_has_started = false;
        }
        if (silent) {
          return *this;
        }

        std::cout << f;
        return *this;
    }
};

extern Log g_log;

}

namespace std {

template<class T, size_t C>
std::ostream &operator<<(std::ostream &stream, const std::array<T, C> &arr) {
    stream << "[";
    for (size_t i = 0; i < arr.size(); ++i) {
        if (i != 0)
            stream << ", ";
        stream << arr[i];
    }
    stream << "]";
    return stream;
}


template<class T>
std::ostream &operator<<(std::ostream &stream, const std::vector<T> &vec) {
    stream << "[";
    for (size_t i = 0; i < vec.size(); ++i) {
        if (i != 0)
            stream << ", ";
        stream << vec[i];
    }
    stream << "]";
    return stream;
}

template<class T>
std::ostream &operator<<(std::ostream &stream, const std::unordered_set<T> &set) {
    stream << "{";
    size_t i = 0;
    for (const auto& element : set) {
        if (i != 0) stream << ", ";
        stream << element;
        ++i;
    }
    stream << "}";
    return stream;
}

template<class T>
std::ostream &operator<<(std::ostream &stream, const std::set<T> &set) {
    stream << "{";
    size_t i = 0;
    for (const auto& element : set) {
        if (i != 0) stream << ", ";
        stream << element;
        ++i;
    }
    stream << "}";
    return stream;
}

template<class T, class U>
std::ostream &operator<<(std::ostream &stream, const std::pair<T, U> &pair) {
    stream << "<" << pair.first << "," << pair.second << ">";
    return stream;
}

}


#endif
