#ifndef DLP_SRC_CORE_VOCABULARY_INFO_IMPL_H_
#define DLP_SRC_CORE_VOCABULARY_INFO_IMPL_H_

#include <unordered_map>
#include <string>
#include <vector>

#include "../../include/dlp/core.h"


namespace dlp::core {
class SyntacticElementFactoryImpl;
class InstanceInfoImpl;

enum EXPRESSION_TYPE {
    // concepts
    C_ALL,
    C_AND,
    C_BOT,
    C_DIFF,
    C_NOT,
    C_ONE_OF,
    C_OR,
    C_SOME,
    C_SUBSET,
    C_TOP,
    // roles
    R_AND,
    R_COMPOSE,
    R_DIFF,
    R_IDENTITY,
    R_INVERSE,
    R_NOT,
    R_OR,
    R_RESTRICT,
    R_TOP,
    R_TRANSITIVE_CLOSURE,
    R_TRANSITIVE_REFLEXIVE_CLOSURE,
    // numerical
    N_COUNT,
    N_CONCEPT_DISTANCE,
    N_SUM_CONCEPT_DISTANCE,
    N_ROLE_DISTANCE,
    N_SUM_ROLE_DISTANCE,
    // boolean
    B_EMPTY,
};

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
    std::vector<Predicate> m_predicates;

    /**
     * Mappings between element names and enum.
     */
    static std::unordered_map<std::string, EXPRESSION_TYPE> m_element_name_to_expression_type;

public:
    VocabularyInfoImpl() = default;
    ~VocabularyInfoImpl() = default;

    /**
     * Adds an atom that may have varying evaluation depending on the state.
     */
    const Predicate& add_predicate(const VocabularyInfo& parent, const std::string &predicate_name, int arity);

    /**
     * Getters
     */
    bool exists_predicate_name(const std::string& name) const;
    const std::vector<Predicate>& get_predicates() const;
    int get_predicate_idx(const std::string& name) const;
    const Predicate& get_predicate(int predicate_idx) const;
    const VocabularyInfo* get_vocabulary_info() const;

    static bool exists_element_name(const std::string& name);
    static EXPRESSION_TYPE element_name_to_expression_type(const std::string &name);
};

}

#endif