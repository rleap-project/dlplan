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

public:
    template<typename T>
    Log &operator<<(const T &elem) {
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

        std::cout << f;
        return *this;
    }
};

extern Log g_log;

}

namespace std {
    template<class T>
    ostream &operator<<(ostream &stream, const vector<T> &vec) {
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
    ostream &operator<<(ostream &stream, const unordered_set<T> &set) {
        stream << "{";
        for (size_t i = 0; i < set.size(); ++i) {
            if (i != 0)
                stream << ", ";
            stream << set[i];
        }
        stream << "}";
        return stream;
    }
}

#endif