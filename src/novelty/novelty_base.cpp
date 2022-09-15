#include "../../include/dlplan/novelty.h"

#include <vector>


namespace dlplan::novelty {

NoveltyBase::NoveltyBase(int width) : m_width(width) {
    m_factors = std::vector<int>(width);
    for (int i = 0; i < m_width; ++i) {
        m_factors[i] = std::pow(2, i);
    }
}

}