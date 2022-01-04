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
    /**
     * Tries to add an element with a specific hash.
     * Returns true if element is newly inserted.
     */
    void push_back(T&& element) {
        // std::lock_guard<std::mutex> hold(m_mutex);
        m_elements.push_back(std::move(element));
    }

    /**
     * Apply function to all elements with in the iteration.
     */
    void for_each(std::function<void(const T&)> function) const {
        // std::lock_guard<std::mutex> hold(m_mutex);
        for (const auto& c : m_elements) {
            function(c);
        }
    }

    size_t size() const {
        // std::lock_guard<std::mutex> hold(m_mutex);
        return m_elements.size();
    }
};

}

#endif
