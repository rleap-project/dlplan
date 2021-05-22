#include "include/dlp/feature_factory.h"


int main() {
    dlp::FeatureFactory factory;
    std::shared_ptr<dlp::ConceptElement> element = factory.make_concept_element("f_count(c_and(g_on(0),s_on(0)))");
};