#ifndef DLP_INCLUDE_DLP_ELEMENT_GENERATOR_H_
#define DLP_INCLUDE_DLP_ELEMENT_GENERATOR_H_

#include "core.h"
#include "../../src/utils/pimpl.h"


namespace dlp {
namespace generator {
class FeatureGeneratorImpl;
class StateSpaceInfoImpl;

/**
 * Extends the functionality by allowing to add states.
 */
class StateSpaceInfo {
private:
    pimpl<StateSpaceInfoImpl> m_pImpl;

public:
    StateSpaceInfo();
    StateSpaceInfo(const StateSpaceInfo& other) = delete;
    ~StateSpaceInfo();

    /**
     * Adds a state to the instance and returns its index.
     */
    int add_state(const core::State& state);
};

/**
 * Exhaustively generates features up to certain complexity.
 */
class FeatureGenerator {
private:
    pimpl<FeatureGeneratorImpl> m_pImpl;

public:
    FeatureGenerator();
    ~FeatureGenerator();

    /**
     * Exhaustively generates features.
     */
    void generate(core::SyntacticElementFactory& factory, const std::vector<StateSpaceInfo>& instances, int complexity, int time_limit);

    /**
     * Dumps information of generated features to file.
     */
    void dump_to_file(const std::string& filename);
};

}
}

#endif
