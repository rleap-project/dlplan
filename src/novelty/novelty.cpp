#include "../../include/dlplan/novelty.h"

#include "../../include/dlplan/utils/hashing.h"


namespace std {
    size_t hash<dlplan::novelty::TupleEdge>::operator()(const dlplan::novelty::TupleEdge& edge) const noexcept {
        std::size_t result = 0;
        dlplan::utils::hash_combine(result, edge.first);
        dlplan::utils::hash_combine(result, edge.second);
        return result;
    }
}


namespace dlplan::novelty {

}
