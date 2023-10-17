#ifndef DLPLAN_SRC_CORE_ELEMENT_FACTORY_H_
#define DLPLAN_SRC_CORE_ELEMENT_FACTORY_H_

#include "cache.h"

#include <memory>
#include <string>

#include "include/dlplan/core.h"


namespace dlplan::core {
class SyntacticElementFactoryImpl;
}

namespace boost::serialization {
    class access;

    template<typename Archive>
    void serialize(Archive& ar, dlplan::core::SyntacticElementFactoryImpl& factory, const unsigned int version);
}


namespace dlplan::core {
class SyntacticElementFactoryImpl {
private:
    std::shared_ptr<VocabularyInfo> m_vocabulary_info;

    Caches m_caches;

    /// @brief Constructor for serialization.
    SyntacticElementFactoryImpl();

    friend class boost::serialization::access;
    template<typename Archive>
    friend void boost::serialization::serialize(Archive& ar, SyntacticElementFactoryImpl& factory, const unsigned int version);

public:
    SyntacticElementFactoryImpl(std::shared_ptr<VocabularyInfo> vocabulary_info);

    // Root calls
    std::shared_ptr<const Concept> parse_concept(SyntacticElementFactory& parent,
        const std::string &description, const std::string& filename);
    std::shared_ptr<const Role> parse_role(SyntacticElementFactory& parent,
        const std::string &description, const std::string& filename);
    std::shared_ptr<const Numerical> parse_numerical(SyntacticElementFactory& parent,
        const std::string &description, const std::string& filename);
    std::shared_ptr<const Boolean> parse_boolean(SyntacticElementFactory& parent,
        const std::string &description, const std::string& filename);

    // Nested calls
    std::shared_ptr<const Concept> parse_concept(SyntacticElementFactory& parent,
        std::string::const_iterator& iter, std::string::const_iterator end, const std::string& filename);
    std::shared_ptr<const Role> parse_role(SyntacticElementFactory& parent,
        std::string::const_iterator& iter, std::string::const_iterator end, const std::string& filename);
    std::shared_ptr<const Numerical> parse_numerical(SyntacticElementFactory& parent,
        std::string::const_iterator& iter, std::string::const_iterator end, const std::string& filename);
    std::shared_ptr<const Boolean> parse_boolean(SyntacticElementFactory& parent,
        std::string::const_iterator& iter, std::string::const_iterator end, const std::string& filename);

    std::shared_ptr<const Boolean> make_empty_boolean(const std::shared_ptr<const Concept>& concept);
    std::shared_ptr<const Boolean> make_empty_boolean(const std::shared_ptr<const Role>& role);
    std::shared_ptr<const Boolean> make_inclusion_boolean(const std::shared_ptr<const Concept>& concept_left, const std::shared_ptr<const Concept>& concept_right);
    std::shared_ptr<const Boolean> make_inclusion_boolean(const std::shared_ptr<const Role>& role_left, const std::shared_ptr<const Role>& role_right);
    std::shared_ptr<const Boolean> make_nullary_boolean(const Predicate& predicate);

    std::shared_ptr<const Concept> make_all_concept(const std::shared_ptr<const Role>& role, const std::shared_ptr<const Concept>& concept);
    std::shared_ptr<const Concept> make_and_concept(const std::shared_ptr<const Concept>& concept_left, const std::shared_ptr<const Concept>& concept_right);
    std::shared_ptr<const Concept> make_bot_concept();
    std::shared_ptr<const Concept> make_diff_concept(const std::shared_ptr<const Concept>& concept_left, const std::shared_ptr<const Concept>& concept_right);
    std::shared_ptr<const Concept> make_equal_concept(const std::shared_ptr<const Role>& role_left, const std::shared_ptr<const Role>& role_right);
    std::shared_ptr<const Concept> make_not_concept(const std::shared_ptr<const Concept>& concept);
    std::shared_ptr<const Concept> make_one_of_concept(const Constant& constant);
    std::shared_ptr<const Concept> make_or_concept(const std::shared_ptr<const Concept>& concept_left, const std::shared_ptr<const Concept>& concept_right);
    std::shared_ptr<const Concept> make_projection_concept(const std::shared_ptr<const Role>& role, int pos);
    std::shared_ptr<const Concept> make_primitive_concept(const Predicate& predicate, int pos);
    std::shared_ptr<const Concept> make_some_concept(const std::shared_ptr<const Role>& role, const std::shared_ptr<const Concept>& concept);
    std::shared_ptr<const Concept> make_subset_concept(const std::shared_ptr<const Role>& role_left, const std::shared_ptr<const Role>& role_right);
    std::shared_ptr<const Concept> make_top_concept();

    std::shared_ptr<const Numerical> make_concept_distance_numerical(const std::shared_ptr<const Concept>& concept_from, const std::shared_ptr<const Role>& role, const std::shared_ptr<const Concept>& concept_to);
    std::shared_ptr<const Numerical> make_count_numerical(const std::shared_ptr<const Concept>& concept);
    std::shared_ptr<const Numerical> make_count_numerical(const std::shared_ptr<const Role>& role);
    std::shared_ptr<const Numerical> make_role_distance_numerical(const std::shared_ptr<const Role>& role_from, const std::shared_ptr<const Role>& role, const std::shared_ptr<const Role>& role_to);
    std::shared_ptr<const Numerical> make_sum_concept_distance_numerical(const std::shared_ptr<const Concept>& concept_from, const std::shared_ptr<const Role>& role, const std::shared_ptr<const Concept>& concept_to);
    std::shared_ptr<const Numerical> make_sum_role_distance_numerical(const std::shared_ptr<const Role>& role_from, const std::shared_ptr<const Role>& role, const std::shared_ptr<const Role>& role_to);

    std::shared_ptr<const Role> make_and_role(const std::shared_ptr<const Role>& role_left, const std::shared_ptr<const Role>& role_right);
    std::shared_ptr<const Role> make_compose_role(const std::shared_ptr<const Role>& role_left, const std::shared_ptr<const Role>& role_right);
    std::shared_ptr<const Role> make_diff_role(const std::shared_ptr<const Role>& role_left, const std::shared_ptr<const Role>& role_right);
    std::shared_ptr<const Role> make_identity_role(const std::shared_ptr<const Concept>& concept);
    std::shared_ptr<const Role> make_inverse_role(const std::shared_ptr<const Role>& role);
    std::shared_ptr<const Role> make_not_role(const std::shared_ptr<const Role>& role);
    std::shared_ptr<const Role> make_or_role(const std::shared_ptr<const Role>& role_left, const std::shared_ptr<const Role>& role_right);
    std::shared_ptr<const Role> make_primitive_role(const Predicate& predicate, int pos_1, int pos_2);
    std::shared_ptr<const Role> make_restrict_role(const std::shared_ptr<const Role>& role, const std::shared_ptr<const Concept>& concept);
    std::shared_ptr<const Role> make_top_role();
    std::shared_ptr<const Role> make_transitive_closure(const std::shared_ptr<const Role>& role);
    std::shared_ptr<const Role> make_transitive_reflexive_closure(const std::shared_ptr<const Role>& role);

    /**
     * Getters.
     */
    std::shared_ptr<VocabularyInfo> get_vocabulary_info() const;
};

}

#endif
