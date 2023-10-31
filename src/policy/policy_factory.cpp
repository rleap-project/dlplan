#include "policy_factory.h"

#include <stdexcept>

#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>

#include "include/dlplan/policy.h"
#include "include/dlplan/policy/parsers/syntactic/ast.hpp"
#include "include/dlplan/policy/parsers/semantic/context.hpp"
#include "include/dlplan/policy/parsers/semantic/parser.hpp"

#include "parsers/syntactic/parser.hpp"

#include "condition.h"
#include "effect.h"


using namespace dlplan;


namespace dlplan::policy {

PolicyFactoryImpl::PolicyFactoryImpl() : m_element_factory(nullptr) { }

PolicyFactoryImpl::PolicyFactoryImpl(std::shared_ptr<core::SyntacticElementFactory> element_factory)
    : m_element_factory(element_factory) { }

std::shared_ptr<const Policy> PolicyFactoryImpl::parse_policy(
    PolicyFactory& parent,
    const std::string& description,
    const std::string& filename) {
    iterator_type iter(description.begin());
    iterator_type const end(description.end());
    return parse_policy(parent, iter, end, filename);
}

std::shared_ptr<const Policy> PolicyFactoryImpl::parse_policy(
    PolicyFactory& parent,
    iterator_type& iter, iterator_type end,
    const std::string& filename) {
    /* Stage 1 parse */
    // Our parser
    using boost::spirit::x3::with;

    // Our error handler
    error_handler_type error_handler(iter, end, std::cerr, filename);
    auto const parser =
        // we pass our error handler to the parser so we can access
        // it later on in our on_error and on_sucess handlers
        with<error_handler_tag>(std::ref(error_handler)) [
            policy_root()
        ];

    // Our AST
    ast::Policy ast;

    // Go forth and parse!
    using boost::spirit::x3::ascii::space;
    bool success = phrase_parse(iter, end, parser, space, ast);
    if (!success) {
        throw std::runtime_error("Failed parse.");
    }

    /* Stage 2 parse */
    Context context(parent);
    std::shared_ptr<const Policy> policy = parse(ast, error_handler, context);

    return policy;
}

std::shared_ptr<const NamedBoolean> PolicyFactoryImpl::make_boolean(const std::string& key, const std::shared_ptr<const core::Boolean>& boolean) {
    auto it = m_caches.m_boolean_cache->insert(std::make_unique<NamedBoolean>(key, boolean));
    if (!it.second && (it.first->get_boolean() != boolean)) {
        throw std::runtime_error("Failed to make boolean because a different boolean with the same key already exists.");
    }
    return it.first;
}

std::shared_ptr<const NamedNumerical> PolicyFactoryImpl::make_numerical(const std::string& key, const std::shared_ptr<const core::Numerical>& numerical) {
    auto it = m_caches.m_numerical_cache->insert(std::make_unique<NamedNumerical>(key, numerical));
    if (!it.second && (it.first->get_numerical() != numerical)) {
        throw std::runtime_error("Failed to make numerical because a different numerical with the same key already exists.");
    }
    return it.first;
}

std::shared_ptr<const NamedConcept> PolicyFactoryImpl::make_concept(const std::string& key, const std::shared_ptr<const core::Concept>& concept) {
    auto it = m_caches.m_concept_cache->insert(std::make_unique<NamedConcept>(key, concept));
    if (!it.second && (it.first->get_concept() != concept)) {
        throw std::runtime_error("Failed to make concept because a different concept with the same key already exists.");
    }
    return it.first;
}

std::shared_ptr<const NamedRole> PolicyFactoryImpl::make_role(const std::string& key, const std::shared_ptr<const core::Role>& role) {
    auto it = m_caches.m_role_cache->insert(std::make_unique<NamedRole>(key, role));
    if (!it.second && (it.first->get_role() != role)) {
        throw std::runtime_error("Failed to make role because a different role with the same key already exists.");
    }
    return it.first;
}

std::shared_ptr<const BaseCondition> PolicyFactoryImpl::make_pos_condition(const std::shared_ptr<const NamedBoolean>& boolean) {
    return m_caches.m_condition_cache->insert(std::make_unique<PositiveBooleanCondition>(boolean, m_caches.m_condition_cache->size())).first;
}

std::shared_ptr<const BaseCondition> PolicyFactoryImpl::make_neg_condition(const std::shared_ptr<const NamedBoolean>& boolean) {
    return m_caches.m_condition_cache->insert(std::make_unique<NegativeBooleanCondition>(boolean, m_caches.m_condition_cache->size())).first;
}

std::shared_ptr<const BaseCondition> PolicyFactoryImpl::make_gt_condition(const std::shared_ptr<const NamedNumerical>& numerical) {
    return m_caches.m_condition_cache->insert(std::make_unique<GreaterNumericalCondition>(numerical, m_caches.m_condition_cache->size())).first;
}

std::shared_ptr<const BaseCondition> PolicyFactoryImpl::make_eq_condition(const std::shared_ptr<const NamedNumerical>& numerical) {
    return m_caches.m_condition_cache->insert(std::make_unique<EqualNumericalCondition>(numerical, m_caches.m_condition_cache->size())).first;
}

std::shared_ptr<const BaseEffect> PolicyFactoryImpl::make_pos_effect(const std::shared_ptr<const NamedBoolean>& boolean) {
    return m_caches.m_effect_cache->insert(std::make_unique<PositiveBooleanEffect>(boolean, m_caches.m_effect_cache->size())).first;
}

std::shared_ptr<const BaseEffect> PolicyFactoryImpl::make_neg_effect(const std::shared_ptr<const NamedBoolean>& boolean) {
    return m_caches.m_effect_cache->insert(std::make_unique<NegativeBooleanEffect>(boolean, m_caches.m_effect_cache->size())).first;
}

std::shared_ptr<const BaseEffect> PolicyFactoryImpl::make_bot_effect(const std::shared_ptr<const NamedBoolean>& boolean) {
    return m_caches.m_effect_cache->insert(std::make_unique<UnchangedBooleanEffect>(boolean, m_caches.m_effect_cache->size())).first;
}

std::shared_ptr<const BaseEffect> PolicyFactoryImpl::make_inc_effect(const std::shared_ptr<const NamedNumerical>& numerical) {
    return m_caches.m_effect_cache->insert(std::make_unique<IncrementNumericalEffect>(numerical, m_caches.m_effect_cache->size())).first;
}

std::shared_ptr<const BaseEffect> PolicyFactoryImpl::make_dec_effect(const std::shared_ptr<const NamedNumerical>& numerical) {
    return m_caches.m_effect_cache->insert(std::make_unique<DecrementNumericalEffect>(numerical, m_caches.m_effect_cache->size())).first;
}

std::shared_ptr<const BaseEffect> PolicyFactoryImpl::make_bot_effect(const std::shared_ptr<const NamedNumerical>& numerical) {
    return m_caches.m_effect_cache->insert(std::make_unique<UnchangedNumericalEffect>(numerical, m_caches.m_effect_cache->size())).first;
}

std::shared_ptr<const Rule> PolicyFactoryImpl::make_rule(const Conditions& conditions, const Effects& effects) {
    return m_caches.m_rule_cache->insert(std::unique_ptr<Rule>(new Rule(conditions, effects, m_caches.m_rule_cache->size()))).first;
}

std::shared_ptr<const Policy> PolicyFactoryImpl::make_policy(
    const Rules& rules) {
    return m_caches.m_policy_cache->insert(std::unique_ptr<Policy>(new Policy(rules, m_caches.m_policy_cache->size()))).first;
}

std::shared_ptr<core::SyntacticElementFactory> PolicyFactoryImpl::get_element_factory() const {
    return m_element_factory;
}

}


namespace boost::serialization {
template<typename Archive>
void serialize( Archive& ar, dlplan::policy::PolicyFactoryImpl& t, const unsigned int /* version */ )
{
    ar & t.m_caches;
}

template void serialize(boost::archive::text_iarchive& ar,
    dlplan::policy::PolicyFactoryImpl& t, const unsigned int version);
template void serialize(boost::archive::text_oarchive& ar,
    dlplan::policy::PolicyFactoryImpl& t, const unsigned int version);

}