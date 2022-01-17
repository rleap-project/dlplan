#include "timer.h"

#include <ctime>
#include <ostream>

#include <sys/time.h>

using namespace std;

namespace dlplan::utils {
ostream &operator<<(ostream &os, const Duration &time) {
    os << static_cast<double>(time) << "s";
    return os;
}

static double compute_sanitized_duration(double start_clock, double end_clock) {
    /*
        Sometimes we measure durations that are closer to 0 than should be physically possible
        with measurements on a single CPU. Note that with a CPU frequency less than 10 GHz,
        each clock cycle will take more than 1e-10 seconds. Even worse, these close-to-zero durations
        are sometimes negative. We sanitize them to 0.
    */
    double duration = end_clock - start_clock;
    if (duration > -1e-10 && duration < 1e-10)
        duration = 0.0;
    return duration;
}


Timer::Timer(bool start) {
    collected_time = 0;
    stopped = !start;
    last_start_clock = start ? current_clock() : 0.;
}

double Timer::current_clock() const {
    timespec tp;
    // CLOCK_MONOTONIC_RAW for wallclock, CLOCK_PROCESS_CPUTIME_ID for accumulate over threads
    clock_gettime(CLOCK_MONOTONIC_RAW, &tp);
    return tp.tv_sec + tp.tv_nsec / 1e9;
}

Duration Timer::stop() {
    collected_time = (*this)();
    stopped = true;
    return Duration(collected_time);
}

Duration Timer::operator()() const {
    if (stopped)
        return Duration(collected_time);
    else
        return Duration(collected_time
                        + compute_sanitized_duration(last_start_clock, current_clock()));
}

void Timer::resume() {
    if (stopped) {
        stopped = false;
        last_start_clock = current_clock();
    }
}

Duration Timer::reset() {
    double result = (*this)();
    collected_time = 0;
    last_start_clock = current_clock();
    return Duration(result);
}

ostream &operator<<(ostream &os, const Timer &timer) {
    os << timer();
    return os;
}

Timer g_timer;
}