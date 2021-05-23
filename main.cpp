#include "include/dlp/element_factory.h"


int main() {
    dlp::ElementFactory factory;
    dlp::NumericalElement_Ptr numerical = factory.make_numerical_element("f_count(c_and(g_on(0),s_on(0)))");
};
