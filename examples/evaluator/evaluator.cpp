#include "../include/dlplan/policy.h"

#include <fstream>
#include <cassert>
#include <sstream>

#include "../include/dlplan/core.h"


using namespace dlplan::policy;
using namespace dlplan::core;

int main() {
    // Construct SyntacticElementFactory
    std::shared_ptr<dlplan::core::VocabularyInfo> vocabulary_info = std::make_shared<dlplan::core::VocabularyInfo>();
    vocabulary_info->add_predicate("unary", 1);

    dlplan::core::SyntacticElementFactory factory(vocabulary_info);
    dlplan::core::Boolean boolean = factory.parse_boolean("b_empty(c_primitive(unary,0))");
    dlplan::core::Numerical numerical = factory.parse_numerical("n_count(c_primitive(unary,0))");

    // Construct InstanceInfo and States
    std::shared_ptr<dlplan::core::InstanceInfo> instance_info = std::make_shared<dlplan::core::InstanceInfo>(vocabulary_info);
    const auto a0 = instance_info->add_atom("unary", {"A"});
    const auto a1 = instance_info->add_atom("unary", {"B"});
    dlplan::core::State s0(instance_info, std::vector<dlplan::core::Atom>(), 0);
    dlplan::core::State s1(instance_info, {a0}, 1);
    dlplan::core::State s2(instance_info, {a0, a1}, 2);

    // Construct a cache to avoid redundant reevaluations
    // We want to cache 1 Boolean and 1 Numerical
    dlplan::evaluator::EvaluationCache evaluation_cache(1, 1);
    // Set index corresponding to the position in the cache
    boolean.set_index(0);
    numerical.set_index(0);

    std::cout << "boolean(s0): " << evaluation_cache.retrieve_or_evaluate(boolean, s0) << std::endl;
    std::cout << "boolean(s0): " << evaluation_cache.retrieve_or_evaluate(boolean, s0) << std::endl;
    std::cout << "boolean(s1): " << evaluation_cache.retrieve_or_evaluate(boolean, s1) << std::endl;
    std::cout << "boolean(s1): " << evaluation_cache.retrieve_or_evaluate(boolean, s1) << std::endl;
    std::cout << "boolean(s2): " << evaluation_cache.retrieve_or_evaluate(boolean, s2) << std::endl;
    std::cout << "boolean(s2): " << evaluation_cache.retrieve_or_evaluate(boolean, s2) << std::endl;
    std::cout << "numerical(s0): " << evaluation_cache.retrieve_or_evaluate(numerical, s0) << std::endl;
    std::cout << "numerical(s0): " << evaluation_cache.retrieve_or_evaluate(numerical, s0) << std::endl;
    std::cout << "numerical(s1): " << evaluation_cache.retrieve_or_evaluate(numerical, s1) << std::endl;
    std::cout << "numerical(s1): " << evaluation_cache.retrieve_or_evaluate(numerical, s1) << std::endl;
    std::cout << "numerical(s2): " << evaluation_cache.retrieve_or_evaluate(numerical, s2) << std::endl;
    std::cout << "numerical(s2): " << evaluation_cache.retrieve_or_evaluate(numerical, s2) << std::endl;
    return 0;
}
