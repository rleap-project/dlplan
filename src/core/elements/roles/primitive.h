#ifndef DLP_SRC_CORE_ELEMENTS_ROLES_PRIMITIVE_H_
#define DLP_SRC_CORE_ELEMENTS_ROLES_PRIMITIVE_H_

#include "../role.h"


namespace dlp {
namespace core {
namespace element {

class PrimitiveRole : public Role {
protected:
    unsigned m_pos_1;
    unsigned m_pos_2;

protected:
    virtual const RoleDenotation& evaluate_impl(const StateImpl& state) override {
        const InstanceInfoImpl& info = *state.m_parent;
        // 1. Perform error checking.
        if (!info.exists_predicate_name(m_name)) {
            throw std::runtime_error("PrimitiveRole::evaluate_impl - predicate ("s + m_name + ") is missing in InstanceInfo.");
        }
        unsigned predicate_idx = info.get_predicate_idx(m_name);
        unsigned predicate_arity = info.get_predicate(predicate_idx).m_arity;
        if (m_pos_1 >= predicate_arity || m_pos_2 >= predicate_arity) {
            throw std::runtime_error("PrimitiveRole::evaluate_impl - object index does not match predicate arity ("s + std::to_string(m_pos_1) + " or " + std::to_string(m_pos_2)  + " > " + std::to_string(predicate_arity) + ").");
        }
        // 2. Compute the result.
        m_result.clear();
        for (unsigned atom_idx : state.m_atoms) {
            const AtomImpl& atom = info.get_atom(atom_idx);
            if (atom.get_predicate_idx() == predicate_idx) {
                m_result.push_back(std::make_pair(atom.get_object_idx(m_pos_1), atom.get_object_idx(m_pos_2)));
            }
        }
        return m_result;
    }

public:
    PrimitiveRole(const std::string& name, unsigned pos_1, unsigned pos_2)
    : Role(name), m_pos_1(pos_1), m_pos_2(pos_2) { }

    virtual unsigned compute_complexity() const override {
        return 1;
    }

    virtual std::string compute_repr() const override {
        std::stringstream ss;
        ss << m_name << "(" << std::to_string(m_pos_1) << "," << std::to_string(m_pos_2) << ")";
        return ss.str();
    }
};

}
}
}

#endif
