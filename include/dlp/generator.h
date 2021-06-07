#ifndef DLP_INCLUDE_DLP_ELEMENT_GENERATOR_H_
#define DLP_INCLUDE_DLP_ELEMENT_GENERATOR_H_

#include "core.h"
#include "../../src/utils/pimpl.h"


namespace dlp {
namespace generator {
class NumericalImpl;
class BooleanImpl;
class FeatureCollectionImpl;
class FeatureCollectionWriterImpl;
class FeatureGeneratorImpl;
class Numerical;
class Boolean;

using States = std::vector<core::State>;

using NumericalFeatures = std::vector<Numerical>;
using BooleanFeatures = std::vector<Boolean>;

/**
 * Abstract base class of a Feature.
 */
template<typename T>
class Feature {
public:
    Feature() = default;
    virtual ~Feature() = default;

    virtual const std::string& get_repr() const = 0;
    virtual const std::vector<T>& get_state_evaluations() const = 0;
};


/**
 * Numerical stores information related to a core::Numerical.
 */
class Numerical : Feature<int> {
private:
    pimpl<NumericalImpl> m_pImpl;

public:
    Numerical(NumericalImpl&& impl);
    Numerical(const Numerical& other);
    virtual ~Numerical();

    virtual const std::string& get_repr() const override;
    virtual const std::vector<int>& get_state_evaluations() const override;
};


/**
 * Boolean stores information related to a core::Boolean.
 */
class Boolean : Feature<bool> {
private:
    pimpl<BooleanImpl> m_pImpl;

public:
    Boolean(BooleanImpl&& impl);
    Boolean(const Boolean& other);
    virtual ~Boolean();

    virtual const std::string& get_repr() const override;
    virtual const std::vector<bool>& get_state_evaluations() const override;
};


/**
 * FeatureCollection stores different types of Features.
 */
class FeatureCollection {
private:
    pimpl<FeatureCollectionImpl> m_pImpl;

public:
    FeatureCollection(FeatureCollectionImpl&& impl);
    FeatureCollection(const FeatureCollection& other);
    ~FeatureCollection();

    void add_numerical_feature(Numerical&& numerical_feature);
    void add_boolean_feature(Boolean&& boolean_feature);

    const NumericalFeatures& get_numerical_features() const;
    const BooleanFeatures& get_boolean_features() const;
};


/**
 * FeatureCollectionWriter for writing FeatureCollection to file.
 */
class FeatureCollectionWriter {
private:
    pimpl<FeatureCollectionWriterImpl> m_pImpl;
public:
    FeatureCollectionWriter();
    ~FeatureCollectionWriter();

    /**
     * Writes all information related to the features in a single file.
     */
    void write(const FeatureCollection& features, const std::string& filename) const;
};


/**
 * FeatureGenerator exhaustively generates features up to the complexity bound or until the time limit was reached.
 */
class FeatureGenerator {
private:
    pimpl<FeatureGeneratorImpl> m_pImpl;

public:
    FeatureGenerator(std::shared_ptr<core::SyntacticElementFactory> factory, int complexity, int time_limit);
    ~FeatureGenerator();

    /**
     * Exhaustively generates features with pairwise disjoint feature evaluations on the states.
     */
    FeatureCollection generate(const States& states) const;
};


}
}

#endif
