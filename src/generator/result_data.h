#ifndef DLPLAN_SRC_GENERATOR_RESULT_DATA_H_
#define DLPLAN_SRC_GENERATOR_RESULT_DATA_H_

#include <string>
#include <mutex>
#include <vector>

#include "../utils/countdown_timer.h"


namespace dlplan::generator {

class ResultData {
private:
    std::vector<std::string> m_reprs;

    mutable std::mutex m_mutex;

public:

    void add_repr(const std::string& repr) {
        std::lock_guard<std::mutex> hold(m_mutex);
        m_reprs.push_back(repr);
    }

    std::vector<std::string> get_reprs() const {
        std::lock_guard<std::mutex> hold(m_mutex);
        return m_reprs;
    }

    size_t size() const {
        std::lock_guard<std::mutex> hold(m_mutex);
        return m_reprs.size();
    }
};

}

#endif
