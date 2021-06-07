#include "../../include/dlp/generator.h"

#include "feature_collection_writer.h"
#include "feature_collection.h"
#include "feature_generator.h"
#include "numerical.h"
#include "boolean.h"

namespace dlp {
namespace generator {

Numerical::Numerical(NumericalImpl&& impl) : m_pImpl(std::move(impl)) { }

Numerical::Numerical(const Numerical& other) : m_pImpl(*other.m_pImpl) { }

Numerical::~Numerical() { }

const std::string& Numerical::get_repr() const {
    m_pImpl->get_repr();
}

const std::vector<int>& Numerical::get_state_evaluations() const {
    m_pImpl->get_state_evaluations();
}


Boolean::Boolean(BooleanImpl&& impl) : m_pImpl(std::move(impl)) { }

Boolean::Boolean(const Boolean& other) : m_pImpl(*other.m_pImpl) { }

Boolean::~Boolean() { }

const std::string& Boolean::get_repr() const {
    m_pImpl->get_repr();
}

const std::vector<bool>& Boolean::get_state_evaluations() const {
    m_pImpl->get_state_evaluations();
}

FeatureCollection::FeatureCollection() : m_pImpl() { }

FeatureCollection::FeatureCollection(const FeatureCollection& other) : m_pImpl(*other.m_pImpl) { }

FeatureCollection::~FeatureCollection() { }

void FeatureCollection::add_numerical_feature(Numerical&& numerical_feature) {
    return m_pImpl->add_numerical_feature(std::move(numerical_feature));
}

void FeatureCollection::add_boolean_feature(Boolean&& boolean_feature) {
    return m_pImpl->add_boolean_feature(std::move(boolean_feature));
}

const NumericalFeatures& FeatureCollection::get_numerical_features() const {
    return m_pImpl->get_numerical_features();
}

const BooleanFeatures& FeatureCollection::get_boolean_features() const {
    return m_pImpl->get_boolean_features();
}


FeatureCollectionWriter::FeatureCollectionWriter() { }

FeatureCollectionWriter::~FeatureCollectionWriter() { }

void FeatureCollectionWriter::write(const FeatureCollection& features, const std::string& filename) const {
    m_pImpl->write(features, filename);
}


FeatureGenerator::FeatureGenerator(std::shared_ptr<core::SyntacticElementFactory> factory, int complexity, int time_limit)
    : m_pImpl(FeatureGeneratorImpl(factory, complexity, time_limit)) { }

FeatureGenerator::~FeatureGenerator() { }

FeatureCollection FeatureGenerator::generate(const States& states) const {
    return m_pImpl->generate(states);
}


}
}
