#ifndef DLPLAN_SRC_GENERATOR_FEATURE_H_
#define DLPLAN_SRC_GENERATOR_FEATURE_H_

#include <string>
#include <vector>


namespace dlplan::generator {

/**
 * A Features consists of
 *   (1) the string-like representation of the compositional structure, and
 *   (2) the state evaluations.
 */
template<typename T>
class FeatureImpl {
protected:
    const std::string m_repr;
    const std::vector<T> m_state_evaluations;

public:
    FeatureImpl(const std::string& repr, const std::vector<T>& state_evaluations)
        : m_repr(repr), m_state_evaluations(state_evaluations) { }
    ~FeatureImpl() = default;

    virtual const std::string& get_repr() const {
        return m_repr;
    }

    virtual const std::vector<T>& get_state_evaluations() const {
        return m_state_evaluations;
    }
};

}

#endif
