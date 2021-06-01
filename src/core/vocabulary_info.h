#ifndef DLP_SRC_CORE_VOCABULARY_INFO_IMPL_H_
#define DLP_SRC_CORE_VOCABULARY_INFO_IMPL_H_

#include <unordered_map>
#include <string>
#include <vector>

#include "predicate.h"
#include "atom.h"
#include "types.h"


namespace dlp {
namespace core {

/**
 * VocabularyInfoImpl stores information related to the predicates in the planning domain.
 */
class VocabularyInfoImpl {
private:
    /**
     * Mappings between names and indices of predicates and objects.
     */
    std::unordered_map<std::string, unsigned> m_predicate_name_to_predicate_idx;
    std::vector<std::string> m_predicate_idx_to_predicate_name;
    /**
     * All predicates.
     */
    std::vector<PredicateImpl> m_predicates;

public:
    VocabularyInfoImpl() = default;
    ~VocabularyInfoImpl() = default;

    /**
     * Adds an atom that may have varying evaluation depending on the state.
     */
    PredicateImpl add_predicate(const std::string &predicate_name, unsigned arity);

    bool exists_predicate_name(const std::string& name) const;
    unsigned get_predicate_idx(const std::string& name) const;

    const PredicateImpl& get_predicate(unsigned predicate_idx) const;
    const std::vector<PredicateImpl>& get_predicates() const;
};

}
}

#endif