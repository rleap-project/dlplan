#ifndef DLPLAN_SRC_CORE_NUMERICAL_H_
#define DLPLAN_SRC_CORE_NUMERICAL_H_


namespace dlplan::core {

class NumericalImpl {
public:
    NumericalImpl(std::shared_ptr<const element::Numerical>& element)  {
        if (!element) {
            throw std::runtime_error("NumericalImpl::NumericalImpl - tried to construct Numerical from nullptr");
        }
    }

    int evaluate(const Numerical* parent, const State& state) const {
        if (state.get_instance_info()->get_vocabulary_info() != parent->get_vocabulary_info()) {
            throw std::runtime_error("NumericalImpl::evaluate - mismatched vocabularies of Numerical and State.");
        }
        return parent->get_element()->evaluate(state);
    }
};

}

#endif
