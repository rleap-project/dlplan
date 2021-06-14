#ifndef DLPLAN_SRC_UTILS_TIMER_H
#define DLPLAN_SRC_UTILS_TIMER_H

/**
 * Taken from fast-downward.org
 */

#include "system.h"

#include <ostream>


namespace dlplan::utils {

class Duration {
    double seconds;
public:
    explicit Duration(double seconds) : seconds(seconds) {}
    operator double() const {
        return seconds;
    }
};

std::ostream &operator<<(std::ostream &os, const Duration &time);

class Timer {
    double last_start_clock;
    double collected_time;
    bool stopped;

    double current_clock() const;
public:
    explicit Timer(bool start = true);
    ~Timer() = default;
    Duration operator()() const;
    Duration stop();
    void resume();
    Duration reset();
};

std::ostream &operator<<(std::ostream &os, const Timer &timer);

extern Timer g_timer;

}

#endif