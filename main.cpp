#include "include/dlp_element_factory.h"


int main() {
    dlp::ElementFactory factory;
    std::shared_ptr<dlp::Element> element = factory.make_element();
};