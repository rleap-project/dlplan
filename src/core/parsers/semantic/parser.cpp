#include "../../../../include/dlplan/core/parsers/semantic/parser.hpp"

#include <sstream>

using namespace dlplan;


namespace dlplan::core {
template<typename T>
class ParseVisitor : public boost::static_visitor<T> {
private:
    const error_handler_type& error_handler;
    SyntacticElementFactory& context;

public:
    ParseVisitor(const error_handler_type& error_handler, SyntacticElementFactory& context)
        : error_handler(error_handler), context(context) { }

    template<typename Node>
    T operator()(const Node& node) const {
        return parse(node, error_handler, context);
    }
};


template<typename T>
class PointerTypeVisitor : public boost::static_visitor<T> {
public:
    template<typename Node>
    T operator()(const Node&) const {
        return nullptr;
    }

    T operator()(const T& node) const {
        return node;
    }
};


std::string
parse(const ast::Name& node, const error_handler_type&, SyntacticElementFactory&) {
    std::stringstream ss;
    ss << node.alphabetical << node.suffix;
    return ss.str();
}

core::Constant
parse(const ast::Constant& node, const error_handler_type& error_handler, SyntacticElementFactory& context) {
    const auto name = parse(node.name, error_handler, context);
    const auto& constants_mapping = context.get_vocabulary_info()->get_constants_mapping();
    auto it = constants_mapping.find(name);
    if (it == constants_mapping.end()) {
        error_handler(node, "undefined constant");
        throw std::runtime_error("Failed parse.");
    }
    return context.get_vocabulary_info()->get_constant(name);
}

core::Predicate
parse(const ast::Predicate& node, const error_handler_type& error_handler, SyntacticElementFactory& context) {
    const auto name = parse(node.name, error_handler, context);
    const auto predicates_mapping = context.get_vocabulary_info()->get_predicates_mapping();
    auto it = predicates_mapping.find(name);
    if (it == predicates_mapping.end()) {
        error_handler(node, "undefined predicate");
        throw std::runtime_error("Failed parse.");
    }
    return context.get_vocabulary_info()->get_predicate(name);
}

int
parse(const ast::Integer& node, const error_handler_type&, SyntacticElementFactory&) {
    return node.value;
}

int
parse(const ast::Position& node, const error_handler_type& error_handler, SyntacticElementFactory& context) {
    return parse(node.integer, error_handler, context);
}


std::shared_ptr<const core::Boolean>
parse(const ast::EmptyBoolean& node, const error_handler_type& error_handler, SyntacticElementFactory& context) {
    auto concept_or_role = parse(node.element, error_handler, context);
    PointerTypeVisitor<std::shared_ptr<const core::Concept>> concept_visitor;
    auto concept = boost::apply_visitor(concept_visitor, concept_or_role);
    if (concept != nullptr) return context.make_empty_boolean(concept);
    PointerTypeVisitor<std::shared_ptr<const core::Role>> role_visitor;
    auto role = boost::apply_visitor(role_visitor, concept_or_role);
    if (role != nullptr) return context.make_empty_boolean(role);
    error_handler(node, "expected two concepts or two roles");
    throw std::runtime_error("Failed parse.");
}

std::shared_ptr<const core::Boolean>
parse(const ast::InclusionBoolean& node, const error_handler_type& error_handler, SyntacticElementFactory& context) {
    auto concept_or_role_left = parse(node.element_left, error_handler, context);
    auto concept_or_role_right = parse(node.element_right, error_handler, context);
    PointerTypeVisitor<std::shared_ptr<const core::Concept>> concept_visitor_left;
    PointerTypeVisitor<std::shared_ptr<const core::Concept>> concept_visitor_right;
    auto concept_left = boost::apply_visitor(concept_visitor_left, concept_or_role_left);
    auto concept_right = boost::apply_visitor(concept_visitor_right, concept_or_role_right);
    if (concept_left != nullptr && concept_right != nullptr) {
        return context.make_inclusion_boolean(concept_left, concept_right);
    }
    PointerTypeVisitor<std::shared_ptr<const core::Role>> role_visitor_left;
    PointerTypeVisitor<std::shared_ptr<const core::Role>> role_visitor_right;
    auto role_left = boost::apply_visitor(role_visitor_left, concept_or_role_left);
    auto role_right = boost::apply_visitor(role_visitor_right, concept_or_role_right);
    if (role_left != nullptr && role_right != nullptr) {
        return context.make_inclusion_boolean(role_left, role_right);
    }
    error_handler(node, "expected two concepts or two roles");
    throw std::runtime_error("Failed parse.");
}

std::shared_ptr<const core::Boolean>
parse(const ast::NullaryBoolean& node, const error_handler_type& error_handler, SyntacticElementFactory& context) {
    return context.make_nullary_boolean(parse(node.predicate, error_handler, context));
}

std::shared_ptr<const core::Concept>
parse(const ast::AllConcept& node, const error_handler_type& error_handler, SyntacticElementFactory& context) {
    return context.make_all_concept(
        parse(node.role, error_handler, context),
        parse(node.concept_, error_handler, context));
}

std::shared_ptr<const core::Concept>
parse(const ast::AndConcept& node, const error_handler_type& error_handler, SyntacticElementFactory& context) {
    return context.make_and_concept(
        parse(node.concept_left, error_handler, context),
        parse(node.concept_right, error_handler, context));
}

std::shared_ptr<const core::Concept>
parse(const ast::BotConcept&, const error_handler_type&, SyntacticElementFactory& context) {
    return context.make_bot_concept();
}

std::shared_ptr<const core::Concept>
parse(const ast::DiffConcept& node, const error_handler_type& error_handler, SyntacticElementFactory& context) {
    return context.make_diff_concept(
        parse(node.concept_left, error_handler, context),
        parse(node.concept_right, error_handler, context));
}

std::shared_ptr<const core::Concept>
parse(const ast::EqualConcept& node, const error_handler_type& error_handler, SyntacticElementFactory& context) {
    return context.make_equal_concept(
        parse(node.role_left, error_handler, context),
        parse(node.role_right, error_handler, context));
}

std::shared_ptr<const core::Concept>
parse(const ast::NotConcept& node, const error_handler_type& error_handler, SyntacticElementFactory& context) {
    return context.make_not_concept(
        parse(node.concept_, error_handler, context));
}

std::shared_ptr<const core::Concept>
parse(const ast::OneOfConcept& node, const error_handler_type& error_handler, SyntacticElementFactory& context) {
    return context.make_one_of_concept(
        parse(node.constant, error_handler, context));
}

std::shared_ptr<const core::Concept>
parse(const ast::OrConcept& node, const error_handler_type& error_handler, SyntacticElementFactory& context) {
    return context.make_or_concept(
        parse(node.concept_left, error_handler, context),
        parse(node.concept_right, error_handler, context));
}

std::shared_ptr<const core::Concept>
parse(const ast::PrimitiveConcept& node, const error_handler_type& error_handler, SyntacticElementFactory& context) {
    return context.make_primitive_concept(
        parse(node.predicate, error_handler, context),
        parse(node.pos, error_handler, context));
}

std::shared_ptr<const core::Concept>
parse(const ast::ProjectionConcept& node, const error_handler_type& error_handler, SyntacticElementFactory& context) {
    return context.make_projection_concept(
        parse(node.role, error_handler, context),
        parse(node.pos, error_handler, context));
}

std::shared_ptr<const core::Concept>
parse(const ast::SomeConcept& node, const error_handler_type& error_handler, SyntacticElementFactory& context) {
    return context.make_some_concept(
        parse(node.role, error_handler, context),
        parse(node.concept_, error_handler, context));
}

std::shared_ptr<const core::Concept>
parse(const ast::SubsetConcept& node, const error_handler_type& error_handler, SyntacticElementFactory& context) {
    return context.make_subset_concept(
        parse(node.role_left, error_handler, context),
        parse(node.role_right, error_handler, context));
}

std::shared_ptr<const core::Concept>
parse(const ast::TopConcept&, const error_handler_type&, SyntacticElementFactory& context) {
    return context.make_top_concept();
}

std::shared_ptr<const core::Numerical>
parse(const ast::ConceptDistanceNumerical& node, const error_handler_type& error_handler, SyntacticElementFactory& context) {
    return context.make_concept_distance_numerical(
        parse(node.concept_left, error_handler, context),
        parse(node.role, error_handler, context),
        parse(node.concept_right, error_handler, context));
}

std::shared_ptr<const core::Numerical>
parse(const ast::CountNumerical& node, const error_handler_type& error_handler, SyntacticElementFactory& context) {
    auto concept_or_role = parse(node.element, error_handler, context);
    PointerTypeVisitor<std::shared_ptr<const core::Concept>> concept_visitor;
    auto concept = boost::apply_visitor(concept_visitor, concept_or_role);
    if (concept != nullptr) return context.make_count_numerical(concept);
    PointerTypeVisitor<std::shared_ptr<const core::Role>> role_visitor;
    auto role = boost::apply_visitor(role_visitor, concept_or_role);
    if (role != nullptr) return context.make_count_numerical(role);
    error_handler(node, "expected two concepts or two roles");
    throw std::runtime_error("Failed parse.");
}

std::shared_ptr<const core::Numerical>
parse(const ast::RoleDistanceNumerical& node, const error_handler_type& error_handler, SyntacticElementFactory& context) {
    return context.make_role_distance_numerical(
        parse(node.role_left, error_handler, context),
        parse(node.role, error_handler, context),
        parse(node.role_right, error_handler, context));
}

std::shared_ptr<const core::Numerical>
parse(const ast::SumConceptDistanceNumerical& node, const error_handler_type& error_handler, SyntacticElementFactory& context) {
    return context.make_sum_concept_distance_numerical(
        parse(node.concept_left, error_handler, context),
        parse(node.role, error_handler, context),
        parse(node.concept_right, error_handler, context));
}

std::shared_ptr<const core::Numerical>
parse(const ast::SumRoleDistanceNumerical& node, const error_handler_type& error_handler, SyntacticElementFactory& context) {
    return context.make_sum_role_distance_numerical(
        parse(node.role_left, error_handler, context),
        parse(node.role, error_handler, context),
        parse(node.role_right, error_handler, context));
}

std::shared_ptr<const core::Role>
parse(const ast::AndRole& node, const error_handler_type& error_handler, SyntacticElementFactory& context) {
    return context.make_and_role(
        parse(node.role_left, error_handler, context),
        parse(node.role_right, error_handler, context));
}

std::shared_ptr<const core::Role>
parse(const ast::ComposeRole& node, const error_handler_type& error_handler, SyntacticElementFactory& context) {
    return context.make_compose_role(
        parse(node.role_left, error_handler, context),
        parse(node.role_right, error_handler, context));
}

std::shared_ptr<const core::Role>
parse(const ast::DiffRole& node, const error_handler_type& error_handler, SyntacticElementFactory& context) {
    return context.make_diff_role(
        parse(node.role_left, error_handler, context),
        parse(node.role_right, error_handler, context));
}

std::shared_ptr<const core::Role>
parse(const ast::IdentityRole& node, const error_handler_type& error_handler, SyntacticElementFactory& context) {
    return context.make_identity_role(
        parse(node.concept_, error_handler, context));
}

std::shared_ptr<const core::Role>
parse(const ast::InverseRole& node, const error_handler_type& error_handler, SyntacticElementFactory& context) {
    return context.make_inverse_role(
        parse(node.role, error_handler, context));
}

std::shared_ptr<const core::Role>
parse(const ast::NotRole& node, const error_handler_type& error_handler, SyntacticElementFactory& context) {
    return context.make_not_role(
        parse(node.role, error_handler, context));
}

std::shared_ptr<const core::Role>
parse(const ast::OrRole& node, const error_handler_type& error_handler, SyntacticElementFactory& context) {
    return context.make_or_role(
        parse(node.role_left, error_handler, context),
        parse(node.role_right, error_handler, context));
}

std::shared_ptr<const core::Role>
parse(const ast::PrimitiveRole& node, const error_handler_type& error_handler, SyntacticElementFactory& context) {
    return context.make_primitive_role(
        parse(node.predicate, error_handler, context),
        parse(node.pos_1, error_handler, context),
        parse(node.pos_2, error_handler, context));
}

std::shared_ptr<const core::Role>
parse(const ast::RestrictRole& node, const error_handler_type& error_handler, SyntacticElementFactory& context) {
    return context.make_restrict_role(
        parse(node.role, error_handler, context),
        parse(node.concept_, error_handler, context));
}

std::shared_ptr<const core::Role>
parse(const ast::TopRole&, const error_handler_type&, SyntacticElementFactory& context) {
    return context.make_top_role();
}

std::shared_ptr<const core::Role>
parse(const ast::TransitiveClosureRole& node, const error_handler_type& error_handler, SyntacticElementFactory& context) {
    return context.make_transitive_closure(
        parse(node.role, error_handler, context));
}

std::shared_ptr<const core::Role>
parse(const ast::TransitiveReflexiveClosureRole& node, const error_handler_type& error_handler, SyntacticElementFactory& context) {
    return context.make_transitive_reflexive_closure(
        parse(node.role, error_handler, context));
}

boost::variant<std::shared_ptr<const core::Concept>, std::shared_ptr<const core::Role>>
parse(const ast::ConceptOrRole& node, const error_handler_type& error_handler, SyntacticElementFactory& context) {
    ParseVisitor<boost::variant<std::shared_ptr<const core::Concept>, std::shared_ptr<const core::Role>>> visitor(error_handler, context);
    return boost::apply_visitor(visitor, node);
}

std::shared_ptr<const core::Concept>
parse(const ast::Concept& node, const error_handler_type& error_handler, SyntacticElementFactory& context) {
    ParseVisitor<std::shared_ptr<const core::Concept>> visitor(error_handler, context);
    return boost::apply_visitor(visitor, node);
}


std::shared_ptr<const core::Role>
parse(const ast::Role& node, const error_handler_type& error_handler, SyntacticElementFactory& context) {
    ParseVisitor<std::shared_ptr<const core::Role>> visitor(error_handler, context);
    return boost::apply_visitor(visitor, node);
}


std::shared_ptr<const core::Boolean>
parse(const ast::Boolean& node, const error_handler_type& error_handler, SyntacticElementFactory& context) {
    ParseVisitor<std::shared_ptr<const core::Boolean>> visitor(error_handler, context);
    return boost::apply_visitor(visitor, node);
}

std::shared_ptr<const core::Numerical>
parse(const ast::Numerical& node, const error_handler_type& error_handler, SyntacticElementFactory& context) {
    ParseVisitor<std::shared_ptr<const core::Numerical>> visitor(error_handler, context);
    return boost::apply_visitor(visitor, node);
}


}