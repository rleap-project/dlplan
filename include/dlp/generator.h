#ifndef DLPLAN_INCLUDE_DLP_ELEMENT_GENERATOR_H_
#define DLPLAN_INCLUDE_DLP_ELEMENT_GENERATOR_H_

#include "core.h"
#include "../../src/utils/pimpl.h"


namespace dlplan::generator {
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
    Numerical(const std::string& repr, std::vector<int>& denotation);
    Numerical(const Numerical& other);
    ~Numerical() override;

    const std::string& get_repr() const override;
    const std::vector<int>& get_state_evaluations() const override;
};


/**
 * Boolean stores information related to a core::Boolean.
 */
class Boolean : Feature<bool> {
private:
    pimpl<BooleanImpl> m_pImpl;

public:
    Boolean(const std::string& repr, std::vector<bool>& denotation);
    Boolean(const Boolean& other);
    ~Boolean() override;

    const std::string& get_repr() const override;
    const std::vector<bool>& get_state_evaluations() const override;
};


/**
 * FeatureCollection stores different types of Features.
 */
class FeatureCollection {
private:
    pimpl<FeatureCollectionImpl> m_pImpl;

public:
    FeatureCollection();
    FeatureCollection(const FeatureCollection& other);
    ~FeatureCollection();

    void add_numerical_feature(Numerical&& numerical_feature);
    void add_boolean_feature(Boolean&& boolean_feature);

    const NumericalFeatures& get_numerical_features() const;
    const BooleanFeatures& get_boolean_features() const;
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
    FeatureCollection generate(const States& states);
};


}

#endif
