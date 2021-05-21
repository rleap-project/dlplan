#include "include/dlp/element_factory.h"


int main() {
    dlp::ElementFactory factory;
    std::shared_ptr<dlp::ConceptElement> element = factory.make_concept_element("asd");
};