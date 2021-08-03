#ifndef DLPLAN_SRC_CORE_VOCABULARY_INFO_IMPL_H_
#define DLPLAN_SRC_CORE_VOCABULARY_INFO_IMPL_H_

#include <unordered_map>
#include <string>
#include <vector>

#include "../../include/dlplan/core.h"


namespace dlplan::core {
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
    C_PROJECTION,
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
     * All predicates.
     */
    std::unordered_map<std::string, unsigned> m_predicate_name_to_predicate_idx;
    std::vector<Predicate> m_predicates;
    /**
     * All constants.
     */
    std::unordered_map<std::string, unsigned> m_constant_name_to_constant_idx;
    std::vector<Constant> m_constants;

    /**
     * Mappings between element names and enum.
     */
    static std::unordered_map<std::string, EXPRESSION_TYPE> m_element_name_to_expression_type;

public:
    VocabularyInfoImpl() = default;
    ~VocabularyInfoImpl() = default;

    const Predicate& add_predicate(const std::string &name, int arity);

    const Constant& add_constant(const std::string& object_name);

    /**
     * Getters
     */
    bool exists_predicate(const Predicate& predicate) const;
    bool exists_predicate_name(const std::string& name) const;
    const std::vector<Predicate>& get_predicates() const;
    int get_predicate_idx(const std::string& name) const;
    const Predicate& get_predicate(int index) const;
    bool exists_constant(const Constant& constant) const;
    bool exists_constant_name(const std::string& name) const;
    int get_constant_idx(const std::string& name) const;
    const Constant& get_constant(int constant_idx) const;
    const std::vector<Constant>& get_constants() const;

    static bool exists_element_name(const std::string& name);
    static EXPRESSION_TYPE element_name_to_expression_type(const std::string &name);
};

}

#endif