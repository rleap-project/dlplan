#ifndef DLP_SRC_GENERATOR_FEATURE_COLLECTION_WRITER_H_
#define DLP_SRC_GENERATOR_FEATURE_COLLECTION_WRITER_H_

#include <vector>

#include "numerical.h"
#include "boolean.h"

namespace dlp {
namespace generator {
class FeatureCollection;

class FeatureCollectionWriterImpl {
private:
public:
    FeatureCollectionWriterImpl() = default;
    ~FeatureCollectionWriterImpl() = default;

    void write(const FeatureCollection& features, const std::string& filename) const;
};

}
}

#endif
