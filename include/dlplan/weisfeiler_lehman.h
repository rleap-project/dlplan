#ifndef DLPLAN_INCLUDE_DLPLAN_WEISFEILER_LEHMAN_H_
#define DLPLAN_INCLUDE_DLPLAN_WEISFEILER_LEHMAN_H_

#include <set>
#include <vector>

#include "state_space.h"


namespace dlplan::weisfeiler_lehman {

class WeisfeilerLehman {
private:
public:
    WeisfeilerLehman();

    std::vector<int> compute_coloring(const state_space::StateSpace& state_space) const;
};

}

#endif
