#include "../../include/dlplan/state_space.h"


namespace dlplan::state_space {

StateInformation::StateInformation(StateMapping&& state_mapping)
    : m_state_mapping(std::move(state_mapping)) { }

StateInformation::StateInformation(const StateInformation& other) = default;

StateInformation& StateInformation::operator=(const StateInformation& other) = default;

StateInformation::StateInformation(StateInformation&& other) = default;

StateInformation& StateInformation::operator=(StateInformation&& other) = default;

StateInformation::~StateInformation() = default;

const core::State& StateInformation::get_state_ref(StateIndex state) const {
    auto result = m_state_mapping.find(state);
    if (result == m_state_mapping.end()) {
        throw std::runtime_error("StateInformation::get_state_ref - state out of bounds.");
    }
    return result->second;
}

}