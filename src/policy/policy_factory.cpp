#include "policy_factory.h"

#include "parsers/syntactic/parser.hpp"

#include "../../include/dlplan/policy.h"
#include "../../include/dlplan/policy/parsers/syntactic/ast.hpp"
#include "../../include/dlplan/policy/parsers/semantic/context.hpp"
#include "../../include/dlplan/policy/parsers/semantic/parser.hpp"


#include <stdexcept>


using namespace dlplan;


namespace dlplan::policy {
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
    auto result = m_cache.get_or_create<NamedBoolean>(key, boolean);
    if (!result.created && (result.object->get_element() != boolean)) {
        throw std::runtime_error("Failed to make boolean because a different boolean with the same key already exists.");
    }
    return result.object;
}

std::shared_ptr<const NamedNumerical> PolicyFactoryImpl::make_numerical(const std::string& key, const std::shared_ptr<const core::Numerical>& numerical) {
    auto result = m_cache.get_or_create<NamedNumerical>(key, numerical);
    if (!result.created && (result.object->get_element() != numerical)) {
        throw std::runtime_error("Failed to make numerical because a different numerical with the same key already exists.");
    }
    return result.object;
}

std::shared_ptr<const NamedConcept> PolicyFactoryImpl::make_concept(const std::string& key, const std::shared_ptr<const core::Concept>& concept_) {
    auto result = m_cache.get_or_create<NamedConcept>(key, concept_);
    if (!result.created && (result.object->get_element() != concept_)) {
        throw std::runtime_error("Failed to make concept because a different concept with the same key already exists.");
    }
    return result.object;
}

std::shared_ptr<const NamedRole> PolicyFactoryImpl::make_role(const std::string& key, const std::shared_ptr<const core::Role>& role) {
    auto result = m_cache.get_or_create<NamedRole>(key, role);
    if (!result.created && (result.object->get_element() != role)) {
        throw std::runtime_error("Failed to make role because a different role with the same key already exists.");
    }
    return result.object;
}

std::shared_ptr<const BaseCondition> PolicyFactoryImpl::make_pos_condition(const std::shared_ptr<const NamedBoolean>& boolean) {
    return m_cache.get_or_create<PositiveBooleanCondition>(boolean).object;
}

std::shared_ptr<const BaseCondition> PolicyFactoryImpl::make_neg_condition(const std::shared_ptr<const NamedBoolean>& boolean) {
    return m_cache.get_or_create<NegativeBooleanCondition>(boolean).object;
}

std::shared_ptr<const BaseCondition> PolicyFactoryImpl::make_gt_condition(const std::shared_ptr<const NamedNumerical>& numerical) {
    return m_cache.get_or_create<GreaterNumericalCondition>(numerical).object;
}

std::shared_ptr<const BaseCondition> PolicyFactoryImpl::make_eq_condition(const std::shared_ptr<const NamedNumerical>& numerical) {
    return m_cache.get_or_create<EqualNumericalCondition>(numerical).object;
}

std::shared_ptr<const BaseCondition> PolicyFactoryImpl::make_gt_condition(const std::shared_ptr<const NamedConcept>& concept_) {
    return m_cache.get_or_create<GreaterConceptCondition>(concept_).object;
}

std::shared_ptr<const BaseCondition> PolicyFactoryImpl::make_eq_condition(const std::shared_ptr<const NamedConcept>& concept_) {
    return m_cache.get_or_create<EqualConceptCondition>(concept_).object;
}

std::shared_ptr<const BaseEffect> PolicyFactoryImpl::make_pos_effect(const std::shared_ptr<const NamedBoolean>& boolean) {
    return m_cache.get_or_create<PositiveBooleanEffect>(boolean).object;
}

std::shared_ptr<const BaseEffect> PolicyFactoryImpl::make_neg_effect(const std::shared_ptr<const NamedBoolean>& boolean) {
    return m_cache.get_or_create<NegativeBooleanEffect>(boolean).object;
}

std::shared_ptr<const BaseEffect> PolicyFactoryImpl::make_bot_effect(const std::shared_ptr<const NamedBoolean>& boolean) {
    return m_cache.get_or_create<UnchangedBooleanEffect>(boolean).object;
}

std::shared_ptr<const BaseEffect> PolicyFactoryImpl::make_inc_effect(const std::shared_ptr<const NamedNumerical>& numerical) {
    return m_cache.get_or_create<IncrementNumericalEffect>(numerical).object;
}

std::shared_ptr<const BaseEffect> PolicyFactoryImpl::make_inc_bot_effect(const std::shared_ptr<const NamedNumerical>& numerical) {
    return m_cache.get_or_create<IncrementOrUnchangedNumericalEffect>(numerical).object;
}

std::shared_ptr<const BaseEffect> PolicyFactoryImpl::make_dec_effect(const std::shared_ptr<const NamedNumerical>& numerical) {
    return m_cache.get_or_create<DecrementNumericalEffect>(numerical).object;
}

std::shared_ptr<const BaseEffect> PolicyFactoryImpl::make_dec_bot_effect(const std::shared_ptr<const NamedNumerical>& numerical) {
    return m_cache.get_or_create<DecrementOrUnchangedNumericalEffect>(numerical).object;
}

std::shared_ptr<const BaseEffect> PolicyFactoryImpl::make_bot_effect(const std::shared_ptr<const NamedNumerical>& numerical) {
    return m_cache.get_or_create<UnchangedNumericalEffect>(numerical).object;
}

std::shared_ptr<const BaseEffect> PolicyFactoryImpl::make_gt_effect(const std::shared_ptr<const NamedNumerical>& numerical) {
    return m_cache.get_or_create<GreaterNumericalEffect>(numerical).object;
}

std::shared_ptr<const BaseEffect> PolicyFactoryImpl::make_eq_effect(const std::shared_ptr<const NamedNumerical>& numerical) {
    return m_cache.get_or_create<EqualNumericalEffect>(numerical).object;
}

std::shared_ptr<const BaseEffect> PolicyFactoryImpl::make_inc_effect(const std::shared_ptr<const NamedConcept>& concept_) {
    return m_cache.get_or_create<IncrementConceptEffect>(concept_).object;
}

std::shared_ptr<const BaseEffect> PolicyFactoryImpl::make_dec_effect(const std::shared_ptr<const NamedConcept>& concept_) {
    return m_cache.get_or_create<DecrementConceptEffect>(concept_).object;
}

std::shared_ptr<const BaseEffect> PolicyFactoryImpl::make_bot_effect(const std::shared_ptr<const NamedConcept>& concept_) {
    return m_cache.get_or_create<UnchangedConceptEffect>(concept_).object;
}

std::shared_ptr<const BaseEffect> PolicyFactoryImpl::make_gt_effect(const std::shared_ptr<const NamedConcept>& concept_) {
    return m_cache.get_or_create<GreaterConceptEffect>(concept_).object;
}

std::shared_ptr<const BaseEffect> PolicyFactoryImpl::make_eq_effect(const std::shared_ptr<const NamedConcept>& concept_) {
    return m_cache.get_or_create<EqualConceptEffect>(concept_).object;
}

std::shared_ptr<const Rule> PolicyFactoryImpl::make_rule(const Conditions& conditions, const Effects& effects) {
    return m_cache.get_or_create<Rule>(conditions, effects).object;
}

std::shared_ptr<const Policy> PolicyFactoryImpl::make_policy(const Rules& rules) {
    return m_cache.get_or_create<Policy>(rules).object;
}

std::shared_ptr<core::SyntacticElementFactory> PolicyFactoryImpl::get_element_factory() const {
    return m_element_factory;
}

// Explicit template instantiations
}

namespace dlplan {
template class ReferenceCountedObjectFactory<policy::NamedBoolean
        , policy::NamedNumerical
        , policy::NamedConcept
        , policy::NamedRole
        , policy::PositiveBooleanCondition
        , policy::NegativeBooleanCondition
        , policy::GreaterNumericalCondition
        , policy::EqualNumericalCondition
        , policy::PositiveBooleanEffect
        , policy::NegativeBooleanEffect
        , policy::UnchangedBooleanEffect
        , policy::IncrementNumericalEffect
        , policy::DecrementNumericalEffect
        , policy::UnchangedNumericalEffect
        , policy::Rule
        , policy::Policy>;
}