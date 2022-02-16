#include "../../include/dlplan/core.h"

#include <iostream>
#include <memory>

using namespace dlplan::core;

static std::shared_ptr<VocabularyInfo> construct_vocabulary_info() {
    std::shared_ptr<VocabularyInfo> vocabulary_info = std::make_shared<VocabularyInfo>();
    // Add predicates and constants of the domain.
    // Note that there are no constants in Blocksworld.
    vocabulary_info->add_predicate("on", 2);
    vocabulary_info->add_predicate("on_g", 2);
    vocabulary_info->add_predicate("ontable", 1);
    vocabulary_info->add_predicate("holding", 1);
    vocabulary_info->add_predicate("clear", 1);
    vocabulary_info->add_predicate("arm-empty", 0);
    return vocabulary_info;
}

static std::shared_ptr<InstanceInfo> construct_instance_info(std::shared_ptr<VocabularyInfo> vocabulary_info) {
    std::shared_ptr<InstanceInfo> instance_info = std::make_shared<InstanceInfo>(vocabulary_info);
    // Add dynamic atoms
    instance_info->add_atom("on", {"a", "b"});
    instance_info->add_atom("on", {"b", "a"});
    instance_info->add_atom("ontable", {"a"});
    instance_info->add_atom("ontable", {"b"});
    instance_info->add_atom("holding", {"a"});
    instance_info->add_atom("holding", {"b"});
    instance_info->add_atom("clear", {"a"});
    instance_info->add_atom("clear", {"b"});
    instance_info->add_atom("arm-empty", {});
    // Add static goal atoms
    instance_info->add_static_atom("on_g", {"a", "b"});
    // Add static atoms
    // Note that there are no static atoms in Blocksworld.
    return instance_info;
}


/**
 * The simplest example to illustrate the construction and evaluation of elements.
 */
int main() {
    // 1. Initialize VocabularyInfo
    auto vocabulary_info = construct_vocabulary_info();
    // 2. Initialize InstanceInfo
    auto instance_info = construct_instance_info(vocabulary_info);
    // 3. Initialize SyntacticElementFactory
    SyntacticElementFactory factory(vocabulary_info);

    // 4. Construct a state.
    const auto& atoms = instance_info->get_atoms();
    const Atom& a0 = atoms[0];
    const Atom& a3 = atoms[3];
    const Atom& a6 = atoms[6];

    // 5. Construct cache where memory for evaluations is allocated.
    PerElementEvaluationCache cache(instance_info->get_num_objects());
    PerElementEvaluationContext context(cache, std::make_shared<State>(State(instance_info, {a0, a3, a6})));

    // 6. Parse and evaluate elements.
    Numerical numerical = factory.parse_numerical("n_count(c_and(c_primitive(on_g,0),c_primitive(on,0)))");
    std::cout << "repr: " << numerical.compute_repr() << std::endl;
    std::cout << "value: " << numerical.evaluate(context) << std::endl;

    Boolean boolean = factory.parse_boolean("b_empty(c_and(c_primitive(on_g,0),c_primitive(on,0)))");
    std::cout << "repr: " << boolean.compute_repr() << std::endl;
    std::cout << "value: " << boolean.evaluate(context) << std::endl;

    return 0;
}