#ifndef SRC_UTILS_COUNTDOWN_TIMER_H
#define SRC_UTILS_COUNTDOWN_TIMER_H

/**
 * Taken from fast-downward.org
 */

#include "timer.h"

namespace dlp {
namespace utils {

class CountdownTimer {
    Timer timer;
    double max_time;
public:
    explicit CountdownTimer(double max_time);
    ~CountdownTimer();
    bool is_expired() const;
    Duration get_elapsed_time() const;
    Duration get_remaining_time() const;
};

}
}

#endif
