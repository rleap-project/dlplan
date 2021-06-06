#include "../../include/dlp/generator.h"

#include "feature_generator.h"


namespace dlp {
namespace generator {


FeatureGenerator::FeatureGenerator(std::shared_ptr<core::SyntacticElementFactory> factory, int complexity, int time_limit)
    : m_pImpl(FeatureGeneratorImpl(factory, complexity, time_limit)) { }

FeatureGenerator::~FeatureGenerator() { }

FeatureCollection FeatureGenerator::generate(const States& states) const {
    return m_pImpl->generate(states);
}


}
}
