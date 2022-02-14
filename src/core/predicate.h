#ifndef DLPLAN_SRC_CORE_PREDICATE_H_
#define DLPLAN_SRC_CORE_PREDICATE_H_

#include <string>
#include <vector>


namespace dlplan::core {
class VocabularyInfo;

class PredicateImpl {
private:
    const std::string m_name;
    const int m_index;
    const int m_arity;

public:
    PredicateImpl(const std::string& name, int index, int arity);
    ~PredicateImpl() = default;

    const std::string& get_name() const;
    int get_index() const;
    int get_arity() const;

    std::string str() const;
};

}

#endif
