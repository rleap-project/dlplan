#ifndef DLPLAN_SRC_CORE_VOCABULARY_INFO_IMPL_H_
#define DLPLAN_SRC_CORE_VOCABULARY_INFO_IMPL_H_

#include <unordered_map>
#include <string>
#include <vector>

#include "../../include/dlplan/core.h"


namespace dlplan::core {
class SyntacticElementFactoryImpl;
class InstanceInfoImpl;


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

public:
    VocabularyInfoImpl();
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
    const Predicate& get_predicate_ref(int index) const;
    bool exists_constant(const Constant& constant) const;
    bool exists_constant_name(const std::string& name) const;
    int get_constant_idx(const std::string& name) const;
    const Constant& get_constant(int constant_idx) const;
    const std::vector<Constant>& get_constants() const;
};

}

#endif