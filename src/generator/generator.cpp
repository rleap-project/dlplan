#include "../../include/dlp/generator.h"
#include "state_space_info.h"
#include "feature_generator.h"


namespace dlp {
namespace generator {

StateSpaceInfo::StateSpaceInfo() { }

StateSpaceInfo::~StateSpaceInfo() { }

int StateSpaceInfo::add_state(const core::State& state) {
    return m_pImpl->add_state(state);
}

FeatureGenerator::FeatureGenerator() { }

FeatureGenerator::~FeatureGenerator() { }

void FeatureGenerator::generate(core::SyntacticElementFactory& factory, const std::vector<StateSpaceInfo>& instances, int complexity, int time_limit) {

}

void FeatureGenerator::dump_to_file(const std::string& filename) {

}

}
}
