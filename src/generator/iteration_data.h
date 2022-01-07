#ifndef DLPLAN_SRC_GENERATOR_ITERATION_DATA_H_
#define DLPLAN_SRC_GENERATOR_ITERATION_DATA_H_

#include <functional>
#include <memory>
#include <mutex>


namespace dlplan::generator {

template<typename T>
class IterationData {
private:
    std::vector<T> m_elements;

    mutable std::mutex m_mutex;

public:
    IterationData() : m_elements(), m_mutex() { }

    /**
     * Tries to add an element with a specific hash.
     * Returns true if element is newly inserted.
     */
    void push_back(T&& element) {
        std::lock_guard<std::mutex> hold(m_mutex);
        m_elements.push_back(element);
    }

    std::vector<T> get_elements() const {
        std::lock_guard<std::mutex> hold(m_mutex);
        return m_elements;
    }

    size_t size() const {
        std::lock_guard<std::mutex> hold(m_mutex);
        return m_elements.size();
    }
};

}

#endif
