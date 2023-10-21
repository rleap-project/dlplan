#include "include/dlplan/core/parsers/elements/stage_2/parser.hpp"

#include <sstream>

using namespace dlplan::common::parsers;


namespace dlplan::core::parsers::elements::stage_2::parser {


static std::string
parse(const stage_1::ast::Name& node, const error_handler_type&, SyntacticElementFactory&) {
    std::stringstream ss;
    ss << node.alphabetical << node.suffix;
    return ss.str();
}

static core::Constant
parse(const stage_1::ast::Constant& node, const error_handler_type& error_handler, SyntacticElementFactory& context) {
    const auto name = parse(node.name, error_handler, context);
    const auto& constants_mapping = context.get_vocabulary_info()->get_constants_mapping();
    auto it = constants_mapping.find(name);
    if (it == constants_mapping.end()) {
        error_handler(node, "undefined constant");
        throw std::runtime_error("Failed parse.");
    }
    return context.get_vocabulary_info()->get_constant(name);
}

static core::Predicate
parse(const stage_1::ast::Predicate& node, const error_handler_type& error_handler, SyntacticElementFactory& context) {
    const auto name = parse(node.name, error_handler, context);
    const auto predicates_mapping = context.get_vocabulary_info()->get_predicates_mapping();
    auto it = predicates_mapping.find(name);
    if (it == predicates_mapping.end()) {
        error_handler(node, "undefined predicate");
        throw std::runtime_error("Failed parse.");
    }
    return context.get_vocabulary_info()->get_predicate(name);
}

static int
parse(const stage_1::ast::Position& node, const error_handler_type&, SyntacticElementFactory&) {
    return node.value;
}

class ConceptVisitor {
private:
    const error_handler_type& error_handler;
    SyntacticElementFactory& context;

public:
    ConceptVisitor(const error_handler_type& error_handler, SyntacticElementFactory& context)
        : error_handler(error_handler), context(context) { }

    std::shared_ptr<const core::Concept> result;

    void operator()(const std::shared_ptr<const core::Concept>& concept) {
        result = concept;
    }

    void operator()(const std::shared_ptr<const core::Role>&) { }
};

class RoleVisitor {
private:
    const error_handler_type& error_handler;
    SyntacticElementFactory& context;

public:
    RoleVisitor(const error_handler_type& error_handler, SyntacticElementFactory& context)
        : error_handler(error_handler), context(context) { }

    std::shared_ptr<const core::Role> result;

    void operator()(const std::shared_ptr<const core::Concept>&) { }

    void operator()(const std::shared_ptr<const core::Role>& role) {
        result = role;
    }
};

static std::shared_ptr<const core::Boolean>
parse(const stage_1::ast::EmptyBoolean& node, const error_handler_type& error_handler, SyntacticElementFactory& context) {
    auto concept_or_role = parse(node.element, error_handler, context);
    ConceptVisitor concept_visitor(error_handler, context);
    boost::apply_visitor(concept_visitor, concept_or_role);
    if (concept_visitor.result != nullptr) return context.make_empty_boolean(concept_visitor.result);
    RoleVisitor role_visitor(error_handler, context);
    boost::apply_visitor(role_visitor, concept_or_role);
    if (role_visitor.result != nullptr) return context.make_empty_boolean(role_visitor.result);
    error_handler(node, "expected two concepts or two roles");
    throw std::runtime_error("Failed parse.");
}

static std::shared_ptr<const core::Boolean>
parse(const stage_1::ast::InclusionBoolean& node, const error_handler_type& error_handler, SyntacticElementFactory& context) {
    auto concept_or_role_left = parse(node.element_left, error_handler, context);
    auto concept_or_role_right = parse(node.element_right, error_handler, context);
    ConceptVisitor concept_visitor_left(error_handler, context);
    ConceptVisitor concept_visitor_right(error_handler, context);
    boost::apply_visitor(concept_visitor_left, concept_or_role_left);
    boost::apply_visitor(concept_visitor_right, concept_or_role_right);
    if (concept_visitor_left.result != nullptr && concept_visitor_right.result != nullptr) {
        return context.make_inclusion_boolean(concept_visitor_left.result, concept_visitor_right.result);
    }
    RoleVisitor role_visitor_left(error_handler, context);
    RoleVisitor role_visitor_right(error_handler, context);
    boost::apply_visitor(role_visitor_left, concept_or_role_left);
    boost::apply_visitor(role_visitor_right, concept_or_role_right);
    if (role_visitor_left.result != nullptr && role_visitor_right.result != nullptr) {
        return context.make_inclusion_boolean(role_visitor_left.result, role_visitor_right.result);
    }
    error_handler(node, "expected two concepts or two roles");
    throw std::runtime_error("Failed parse.");
}

static std::shared_ptr<const core::Boolean>
parse(const stage_1::ast::NullaryBoolean& node, const error_handler_type& error_handler, SyntacticElementFactory& context) {
    return context.make_nullary_boolean(parse(node.predicate, error_handler, context));
}

static std::shared_ptr<const core::Concept>
parse(const stage_1::ast::AllConcept& node, const error_handler_type& error_handler, SyntacticElementFactory& context) {
    return context.make_all_concept(
        parse(node.role, error_handler, context),
        parse(node.concept_, error_handler, context));
}

static std::shared_ptr<const core::Concept>
parse(const stage_1::ast::AndConcept& node, const error_handler_type& error_handler, SyntacticElementFactory& context) {
    return context.make_and_concept(
        parse(node.concept_left, error_handler, context),
        parse(node.concept_right, error_handler, context));
}

static std::shared_ptr<const core::Concept>
parse(const stage_1::ast::BotConcept&, const error_handler_type&, SyntacticElementFactory& context) {
    return context.make_bot_concept();
}

static std::shared_ptr<const core::Concept>
parse(const stage_1::ast::DiffConcept& node, const error_handler_type& error_handler, SyntacticElementFactory& context) {
    return context.make_diff_concept(
        parse(node.concept_left, error_handler, context),
        parse(node.concept_right, error_handler, context));
}

static std::shared_ptr<const core::Concept>
parse(const stage_1::ast::EqualConcept& node, const error_handler_type& error_handler, SyntacticElementFactory& context) {
    return context.make_equal_concept(
        parse(node.role_left, error_handler, context),
        parse(node.role_right, error_handler, context));
}

static std::shared_ptr<const core::Concept>
parse(const stage_1::ast::NotConcept& node, const error_handler_type& error_handler, SyntacticElementFactory& context) {
    return context.make_not_concept(
        parse(node.concept_, error_handler, context));
}

static std::shared_ptr<const core::Concept>
parse(const stage_1::ast::OneOfConcept& node, const error_handler_type& error_handler, SyntacticElementFactory& context) {
    return context.make_one_of_concept(
        parse(node.constant, error_handler, context));
}

static std::shared_ptr<const core::Concept>
parse(const stage_1::ast::OrConcept& node, const error_handler_type& error_handler, SyntacticElementFactory& context) {
    return context.make_or_concept(
        parse(node.concept_left, error_handler, context),
        parse(node.concept_right, error_handler, context));
}

static std::shared_ptr<const core::Concept>
parse(const stage_1::ast::PrimitiveConcept& node, const error_handler_type& error_handler, SyntacticElementFactory& context) {
    return context.make_primitive_concept(
        parse(node.predicate, error_handler, context),
        parse(node.pos, error_handler, context));
}

static std::shared_ptr<const core::Concept>
parse(const stage_1::ast::ProjectionConcept& node, const error_handler_type& error_handler, SyntacticElementFactory& context) {
    return context.make_projection_concept(
        parse(node.role, error_handler, context),
        parse(node.pos, error_handler, context));
}

static std::shared_ptr<const core::Concept>
parse(const stage_1::ast::SomeConcept& node, const error_handler_type& error_handler, SyntacticElementFactory& context) {
    return context.make_some_concept(
        parse(node.role, error_handler, context),
        parse(node.concept_, error_handler, context));
}

static std::shared_ptr<const core::Concept>
parse(const stage_1::ast::SubsetConcept& node, const error_handler_type& error_handler, SyntacticElementFactory& context) {
    return context.make_subset_concept(
        parse(node.role_left, error_handler, context),
        parse(node.role_right, error_handler, context));
}

static std::shared_ptr<const core::Concept>
parse(const stage_1::ast::TopConcept&, const error_handler_type&, SyntacticElementFactory& context) {
    return context.make_top_concept();
}

static std::shared_ptr<const core::Numerical>
parse(const stage_1::ast::ConceptDistanceNumerical& node, const error_handler_type& error_handler, SyntacticElementFactory& context) {
    return context.make_concept_distance_numerical(
        parse(node.concept_left, error_handler, context),
        parse(node.role, error_handler, context),
        parse(node.concept_right, error_handler, context));
}

static std::shared_ptr<const core::Numerical>
parse(const stage_1::ast::CountNumerical& node, const error_handler_type& error_handler, SyntacticElementFactory& context) {
    auto concept_or_role = parse(node.element, error_handler, context);
    ConceptVisitor concept_visitor(error_handler, context);
    boost::apply_visitor(concept_visitor, concept_or_role);
    if (concept_visitor.result != nullptr) return context.make_count_numerical(concept_visitor.result);
    RoleVisitor role_visitor(error_handler, context);
    boost::apply_visitor(role_visitor, concept_or_role);
    if (role_visitor.result != nullptr) return context.make_count_numerical(role_visitor.result);
    error_handler(node, "expected two concepts or two roles");
    throw std::runtime_error("Failed parse.");
}

static std::shared_ptr<const core::Numerical>
parse(const stage_1::ast::RoleDistanceNumerical& node, const error_handler_type& error_handler, SyntacticElementFactory& context) {
    return context.make_role_distance_numerical(
        parse(node.role_left, error_handler, context),
        parse(node.role, error_handler, context),
        parse(node.role_right, error_handler, context));
}

static std::shared_ptr<const core::Numerical>
parse(const stage_1::ast::SumConceptDistanceNumerical& node, const error_handler_type& error_handler, SyntacticElementFactory& context) {
    return context.make_sum_concept_distance_numerical(
        parse(node.concept_left, error_handler, context),
        parse(node.role, error_handler, context),
        parse(node.concept_right, error_handler, context));
}

static std::shared_ptr<const core::Numerical>
parse(const stage_1::ast::SumRoleDistanceNumerical& node, const error_handler_type& error_handler, SyntacticElementFactory& context) {
    return context.make_sum_role_distance_numerical(
        parse(node.role_left, error_handler, context),
        parse(node.role, error_handler, context),
        parse(node.role_right, error_handler, context));
}

static std::shared_ptr<const core::Role>
parse(const stage_1::ast::AndRole& node, const error_handler_type& error_handler, SyntacticElementFactory& context) {
    return context.make_and_role(
        parse(node.role_left, error_handler, context),
        parse(node.role_right, error_handler, context));
}

static std::shared_ptr<const core::Role>
parse(const stage_1::ast::ComposeRole& node, const error_handler_type& error_handler, SyntacticElementFactory& context) {
    return context.make_compose_role(
        parse(node.role_left, error_handler, context),
        parse(node.role_right, error_handler, context));
}

static std::shared_ptr<const core::Role>
parse(const stage_1::ast::DiffRole& node, const error_handler_type& error_handler, SyntacticElementFactory& context) {
    return context.make_diff_role(
        parse(node.role_left, error_handler, context),
        parse(node.role_right, error_handler, context));
}

static std::shared_ptr<const core::Role>
parse(const stage_1::ast::IdentityRole& node, const error_handler_type& error_handler, SyntacticElementFactory& context) {
    return context.make_identity_role(
        parse(node.concept_, error_handler, context));
}

static std::shared_ptr<const core::Role>
parse(const stage_1::ast::InverseRole& node, const error_handler_type& error_handler, SyntacticElementFactory& context) {
    return context.make_inverse_role(
        parse(node.role, error_handler, context));
}

static std::shared_ptr<const core::Role>
parse(const stage_1::ast::NotRole& node, const error_handler_type& error_handler, SyntacticElementFactory& context) {
    return context.make_not_role(
        parse(node.role, error_handler, context));
}

static std::shared_ptr<const core::Role>
parse(const stage_1::ast::OrRole& node, const error_handler_type& error_handler, SyntacticElementFactory& context) {
    return context.make_or_role(
        parse(node.role_left, error_handler, context),
        parse(node.role_right, error_handler, context));
}

static std::shared_ptr<const core::Role>
parse(const stage_1::ast::PrimitiveRole& node, const error_handler_type& error_handler, SyntacticElementFactory& context) {
    return context.make_primitive_role(
        parse(node.predicate, error_handler, context),
        parse(node.pos_1, error_handler, context),
        parse(node.pos_2, error_handler, context));
}

static std::shared_ptr<const core::Role>
parse(const stage_1::ast::RestrictRole& node, const error_handler_type& error_handler, SyntacticElementFactory& context) {
    return context.make_restrict_role(
        parse(node.role, error_handler, context),
        parse(node.concept_, error_handler, context));
}

static std::shared_ptr<const core::Role>
parse(const stage_1::ast::TopRole&, const error_handler_type&, SyntacticElementFactory& context) {
    return context.make_top_role();
}

static std::shared_ptr<const core::Role>
parse(const stage_1::ast::TransitiveClosureRole& node, const error_handler_type& error_handler, SyntacticElementFactory& context) {
    return context.make_transitive_closure(
        parse(node.role, error_handler, context));
}

static std::shared_ptr<const core::Role>
parse(const stage_1::ast::TransitiveReflexiveClosureRole& node, const error_handler_type& error_handler, SyntacticElementFactory& context) {
    return context.make_transitive_reflexive_closure(
        parse(node.role, error_handler, context));
}

class ConceptInnerVisitor {
private:
    const error_handler_type& error_handler;
    SyntacticElementFactory& context;

public:
    ConceptInnerVisitor(
        const error_handler_type& error_handler, SyntacticElementFactory& context)
        : error_handler(error_handler), context(context) { }

    std::shared_ptr<const core::Concept> result;

    template<typename Node>
    void operator()(const Node& node) {
        result = parse(node, error_handler, context);
    }
};

static std::shared_ptr<const core::Concept>
parse(const stage_1::ast::ConceptInner& node, const error_handler_type& error_handler, SyntacticElementFactory& context) {
    ConceptInnerVisitor visitor(error_handler, context);
    boost::apply_visitor(visitor, node);
    return visitor.result;
}

class RoleInnerVisitor {
private:
    const error_handler_type& error_handler;
    SyntacticElementFactory& context;

public:
    RoleInnerVisitor(
        const error_handler_type& error_handler, SyntacticElementFactory& context)
        : error_handler(error_handler), context(context) { }

    std::shared_ptr<const core::Role> result;

    template<typename Node>
    void operator()(const Node& node) {
        result = parse(node, error_handler, context);
    }
};

static std::shared_ptr<const core::Role>
parse(const stage_1::ast::RoleInner& node, const error_handler_type& error_handler, SyntacticElementFactory& context) {
    RoleInnerVisitor visitor(error_handler, context);
    boost::apply_visitor(visitor, node);
    return visitor.result;
}

class BooleanInnerVisitor {
private:
    const error_handler_type& error_handler;
    SyntacticElementFactory& context;

public:
    BooleanInnerVisitor(
        const error_handler_type& error_handler, SyntacticElementFactory& context)
        : error_handler(error_handler), context(context) { }

    std::shared_ptr<const core::Boolean> result;

    template<typename Node>
    void operator()(const Node& node) {
        result = parse(node, error_handler, context);
    }
};

static std::shared_ptr<const core::Boolean>
parse(const stage_1::ast::BooleanInner& node, const error_handler_type& error_handler, SyntacticElementFactory& context) {
    BooleanInnerVisitor visitor(error_handler, context);
    boost::apply_visitor(visitor, node);
    return visitor.result;
}

class NumericalInnerVisitor {
private:
    const error_handler_type& error_handler;
    SyntacticElementFactory& context;

public:
    NumericalInnerVisitor(
        const error_handler_type& error_handler, SyntacticElementFactory& context)
        : error_handler(error_handler), context(context) { }

    std::shared_ptr<const core::Numerical> result;

    template<typename Node>
    void operator()(const Node& node) {
        result = parse(node, error_handler, context);
    }
};

static std::shared_ptr<const core::Numerical>
parse(const stage_1::ast::NumericalInner& node, const error_handler_type& error_handler, SyntacticElementFactory& context) {
    NumericalInnerVisitor visitor(error_handler, context);
    boost::apply_visitor(visitor, node);
    return visitor.result;
}

class ConceptOrRoleInnerVisitor {
private:
    const error_handler_type& error_handler;
    SyntacticElementFactory& context;

public:
    ConceptOrRoleInnerVisitor(const error_handler_type& error_handler, SyntacticElementFactory& context)
        : error_handler(error_handler), context(context) { }

    boost::variant<std::shared_ptr<const core::Concept>, std::shared_ptr<const core::Role>> result;

    template<typename Node>
    void operator()(const Node& node) {
        result = parse(node, error_handler, context);
    }
};

boost::variant<std::shared_ptr<const core::Concept>, std::shared_ptr<const core::Role>>
parse(const stage_1::ast::ConceptOrRole& node, const error_handler_type& error_handler, SyntacticElementFactory& context) {
    ConceptOrRoleInnerVisitor visitor(error_handler, context);
    boost::apply_visitor(visitor, node.inner);
    return visitor.result;
}

std::shared_ptr<const core::Concept>
parse(const stage_1::ast::Concept& node, const error_handler_type& error_handler, SyntacticElementFactory& context) {
    return parse(node.concept, error_handler, context);
}

std::shared_ptr<const core::Role>
parse(const stage_1::ast::Role& node, const error_handler_type& error_handler, SyntacticElementFactory& context) {
    return parse(node.role, error_handler, context);
}

std::shared_ptr<const Boolean>
parse(const stage_1::ast::Boolean& node, const error_handler_type& error_handler, SyntacticElementFactory& context) {
    return parse(node.boolean, error_handler, context);
}

std::shared_ptr<const Numerical>
parse(const stage_1::ast::Numerical& node, const error_handler_type& error_handler, SyntacticElementFactory& context) {
    return parse(node.numerical, error_handler, context);
}

}