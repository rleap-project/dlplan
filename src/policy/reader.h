#ifndef DLPLAN_SRC_POLICY_READER_H_
#define DLPLAN_SRC_POLICY_READER_H_

#include <deque>
#include <istream>
#include <vector>
#include <memory>


namespace dlplan {
namespace core {
    class SyntacticElementFactory;
}
namespace policy {
template<typename T>
class Feature;
template<typename T>
class FeatureFactory;
class BaseCondition;
class BaseEffect;
class Rule;
class Policy;



class PolicyReaderImpl {
public:
    Policy read(const std::string& data, core::SyntacticElementFactory factory) const;
};

}
}

#endif
