#ifndef DLP_SRC_CORE_PREDICATE_H_
#define DLP_SRC_CORE_PREDICATE_H_

#include <string>
#include <vector>


namespace dlp {
namespace core {
class VocabularyInfo;

/**
 * Predicate contains information regarding the predicates used to construct the atoms.
 */
class PredicateImpl {
private:
    const VocabularyInfo* m_vocabulary_info;
    const std::string m_predicate_name;
    const int m_predicate_idx;
    const int m_arity;

public:
    PredicateImpl(const VocabularyInfo& vocabulary_info, const std::string& predicate_name, int predicate_idx, int arity);
    ~PredicateImpl() = default;

    const VocabularyInfo* get_vocabulary_info() const;
    const std::string& get_predicate_name() const;
    int get_predicate_idx() const;
    int get_arity() const;
};

}
}

#endif
