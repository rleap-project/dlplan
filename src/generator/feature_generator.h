#ifndef DLP_SRC_GENERATOR_FEATURE_GENERATOR_IMPL_H_
#define DLP_SRC_GENERATOR_FEATURE_GENERATOR_IMPL_H_

#include <unordered_map>
#include <memory>

#include "types.h"


namespace dlp {
namespace core {
    class Concept;
    class Role;
    class Numerical;
    class Boolean;
    class SyntacticElementFactory;
}
namespace generator {
class FeatureCollection;

class FeatureGeneratorImpl {
private:
    const std::shared_ptr<core::SyntacticElementFactory> m_factory;
    const int m_complexity;
    const int m_time_limit;

private:
    /**
     * Generates all Elements with complexity 1.
     */
    void generate_base(const States& states) const;

    /**
     * Inductively generate Elements of higher complexity.
     */
    void generate_inductively(const States& states) const;

public:
    FeatureGeneratorImpl(std::shared_ptr<core::SyntacticElementFactory> factory, int complexity, int time_limit);

    /**
     * Exhaustively generates features with pairwise disjoint feature evaluations on the states.
     */
    FeatureCollection generate(const States& states) const;
};

}
}


#endif
