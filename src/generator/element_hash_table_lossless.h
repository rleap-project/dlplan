#ifndef DLPLAN_SRC_GENERATOR_ELEMENT_HASH_TABLE_LOSSLESS_H_
#define DLPLAN_SRC_GENERATOR_ELEMENT_HASH_TABLE_LOSSLESS_H_

#include <unordered_set>
#include <vector>

#include "element_hash_table.h"

#include "../../include/dlp/core.h"


namespace std {
    /**
     * For combining hash value we use the boost::hash_combine one-liner.
     * https://stackoverflow.com/questions/20511347/a-good-hash-function-for-a-vector
     *
     * We provide custom specialization of std::hash that are injected in the namespace std.
     * https://en.cppreference.com/w/cpp/utility/hash
     */
    template<> struct hash<std::vector<int>> {
        std::size_t operator()(const std::vector<int>& denotation) const noexcept {
            std::size_t seed = denotation.size();
            for(const auto & i : denotation) {
                seed ^= i + 0x9e3779b9 + (seed << 6) + (seed >> 2);
            }
            return seed;
        }
    };

    template<> struct hash<std::vector<dlplan::core::ConceptDenotation>>
    {
        std::size_t operator()(const std::vector<dlplan::core::ConceptDenotation>& denotation) const noexcept {
            std::size_t seed = denotation.size();
            for (const auto& v : denotation) {
                for(const auto& i : v) {
                    seed ^= i + 0x9e3779b9 + (seed << 6) + (seed >> 2);
                }
            }
            return seed;
        }
    };

    template<> struct hash<std::vector<dlplan::core::RoleDenotation>> {
        std::size_t operator()(const std::vector<dlplan::core::RoleDenotation>& denotation) const noexcept {
            std::size_t seed = denotation.size();
            for (const auto& v : denotation) {
                for(const auto& i : v) {
                    seed ^= i.first + 0x9e3779b9 + (seed << 6) + (seed >> 2);
                    seed ^= i.second + 0x9e3779b9 + (seed << 6) + (seed >> 2);
                }
            }
            return seed;
        }
    };
}

namespace dlplan {
namespace core {
    class Concept;
    class Role;
    class Numerical;
    class Boolean;
}
namespace generator {

/**
 * In this version, we do not compress the denotations.
 * TODO(dominik): Compress the denotations.
 */

template<typename D>
class ElementHashTableLossLess : public ElementHashTable<D> {
private:
    std::unordered_set<std::vector<D>> m_denotation_cache;

public:
    ElementHashTableLossLess() : ElementHashTable<D>() { }
    virtual ~ElementHashTableLossLess() = default;

    bool insert(core::Element<D>& element, const States& states) override {
        const std::vector<D>& denotation = evaluate(element, states);
        return m_denotation_cache.insert(denotation).second;
    }
};

}
}

#endif
