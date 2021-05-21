#ifndef DLP_SRC_ELEMENT_FACTORY_ATOM_H_
#define DLP_SRC_ELEMENT_FACTORY_ATOM_H_

#include <string>
#include <vector>

namespace dlp {

class Atom {
private:
    const std::string m_predicate_name;
    const unsigned m_predicate_idx;
    const std::vector<std::string> m_object_names;
    const bool constant;
public:
    Atom(std::string &&predicate_name,
        std::vector<std::string> &&object_names,
        bool constant=false);

    Atom(const std::string &predicate_name,
        unsigned predicate_idx,
        const std::vector<const std::string> &object_names,
        const std::vector<unsigned> &object_idxs,
        bool constant=false);
};

}

#endif
