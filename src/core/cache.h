#ifndef DLPLAN_SRC_CORE_CACHE_H_
#define DLPLAN_SRC_CORE_CACHE_H_

#include <unordered_map>
#include <memory>
#include <iostream>


namespace dlplan::core {

/*
Andrès:

Hey! Just some basic info on normal forms for description logic.
It seems that DL people often normalise KBs using negation normal forms (NNFs) or Tseitin’s transformations.
The latter requires only a linear increase in the size of the formulas but only preserves equisatisfiability,
so I don't think it'll do for this application.
NNFs are equivalence-preserving and the transformation of a concept into NNF can also be done in linear time.
You just push negation inwards, make use of de Morgan’s laws and the duality between existential and universal restrictions,
and between at-most and at-least number restrictions (<r.C, > n r.C). Basically, you follow the translation on the top of p.6 here:

https://arxiv.org/pdf/1202.0914.pdf

What's not so nice is that NNFs are not canonical,
so you can't detect if a concept C would be a duplicate by simply transforming it into NNF
and checking if C already occurs in the set of previously generated concepts.
If you insist on using canonical forms to check for duplicates,
then the NNFs can be transformed into CNF or DNF using distributivity.
But as you may know, this can be impractical, as the transformation may cause an exponential blowup in the size of the formulas.
*/

/**
 * A simple cache.
 */
template<typename KEY, typename VALUE>
class Cache {
private:
    std::unordered_map<KEY, std::weak_ptr<VALUE>> m_cache;

private:
    /**
     * Erases all pairs that are expired.
     */
    void erase_expired() {
        auto it = m_cache.begin();
        while (it != m_cache.end()) {
            if (it->second.expired()) {
                it = m_cache.erase(it);
            } else {
                ++it;
            }
        }
    }

public:
    /**
     * Retrieves a certain element.
     */
    std::shared_ptr<VALUE> at(const std::string& key) const {
        return std::shared_ptr<VALUE>(m_cache.at(key));
    }

    /**
     * Inserts a new element and returns a shared reference.
     */
    std::shared_ptr<VALUE> insert(std::unique_ptr<VALUE>&& element) {
        std::string key = element->compute_repr();
        std::shared_ptr<VALUE> value(std::move(element));
        // TODO(dominik): Looping over the whole cache before every insertion should be avoided.
        erase_expired();
        if (m_cache.emplace(key, value).first->second.expired()) {
            m_cache.at(key) = value;
        }
        // std::cout << m_cache.size() << std::endl;
        return value;
    }
};

}

#endif