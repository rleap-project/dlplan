#include "include/dlplan/core/parsers/elements/stage_2/parser.hpp"

#include <sstream>


namespace dlplan::core::parsers::elements::stage_2::parser {

// forward declarations
static boost::variant<std::shared_ptr<const core::Concept>, std::shared_ptr<const core::Role>>
parse(const stage_1::ast::ConceptOrRole& node, const dlplan::parsers::error_handler_type& error_handler, SyntacticElementFactory& element_factory);



static std::string
parse(const stage_1::ast::Name& node, const dlplan::parsers::error_handler_type&, SyntacticElementFactory&) {
    std::stringstream ss;
    ss << node.alphabetical << node.suffix;
    return ss.str();
}

static core::Constant
parse(const stage_1::ast::Constant& node, const dlplan::parsers::error_handler_type& error_handler, SyntacticElementFactory& element_factory) {
    const auto name = parse(node.name, error_handler, element_factory);
    const auto& constants_mapping = element_factory.get_vocabulary_info()->get_constants_mapping();
    auto it = constants_mapping.find(name);
    if (it == constants_mapping.end()) {
        error_handler(node, "undefined constant");
    }
    return element_factory.get_vocabulary_info()->get_constant(name);
}

static core::Predicate
parse(const stage_1::ast::Predicate& node, const dlplan::parsers::error_handler_type& error_handler, SyntacticElementFactory& element_factory) {
    const auto name = parse(node.name, error_handler, element_factory);
    const auto& predicates_mapping = element_factory.get_vocabulary_info()->get_predicates_mapping();
    auto it = predicates_mapping.find(name);
    if (it == predicates_mapping.end()) {
        error_handler(node, "undefined predicate");
    }
    return element_factory.get_vocabulary_info()->get_predicate(name);
}

static int
parse(const stage_1::ast::Position& node, const dlplan::parsers::error_handler_type&, SyntacticElementFactory&) {
    return node.value;
}

class ConceptVisitor {
private:
    const dlplan::parsers::error_handler_type& error_handler;
    SyntacticElementFactory& element_factory;

public:
    ConceptVisitor(const dlplan::parsers::error_handler_type& error_handler, SyntacticElementFactory& element_factory)
        : error_handler(error_handler), element_factory(element_factory) { }

    std::shared_ptr<const core::Concept> result;

    void operator()(const std::shared_ptr<const core::Concept>& concept) {
        result = concept;
    }

    void operator()(const std::shared_ptr<const core::Role>&) { }
};

class RoleVisitor {
private:
    const dlplan::parsers::error_handler_type& error_handler;
    SyntacticElementFactory& element_factory;

public:
    RoleVisitor(const dlplan::parsers::error_handler_type& error_handler, SyntacticElementFactory& element_factory)
        : error_handler(error_handler), element_factory(element_factory) { }

    std::shared_ptr<const core::Role> result;

    void operator()(const std::shared_ptr<const core::Concept>&) { }

    void operator()(const std::shared_ptr<const core::Role>& role) {
        result = role;
    }
};

static std::shared_ptr<const core::Boolean>
parse(const stage_1::ast::EmptyBoolean& node, const dlplan::parsers::error_handler_type& error_handler, SyntacticElementFactory& element_factory) {
    auto concept_or_role = parse(node.element, error_handler, element_factory);
    ConceptVisitor concept_visitor(error_handler, element_factory);
    boost::apply_visitor(concept_visitor, concept_or_role);
    if (concept_visitor.result != nullptr) return element_factory.make_empty_boolean(concept_visitor.result);
    RoleVisitor role_visitor(error_handler, element_factory);
    boost::apply_visitor(role_visitor, concept_or_role);
    if (role_visitor.result != nullptr) return element_factory.make_empty_boolean(role_visitor.result);
    error_handler(node, "expected two concepts or two roles");
    throw std::runtime_error("Failed parse");
}

static std::shared_ptr<const core::Boolean>
parse(const stage_1::ast::InclusionBoolean& node, const dlplan::parsers::error_handler_type& error_handler, SyntacticElementFactory& element_factory) {
    auto concept_or_role_left = parse(node.element_left, error_handler, element_factory);
    auto concept_or_role_right = parse(node.element_right, error_handler, element_factory);
    ConceptVisitor concept_visitor_left(error_handler, element_factory);
    ConceptVisitor concept_visitor_right(error_handler, element_factory);
    boost::apply_visitor(concept_visitor_left, concept_or_role_left);
    boost::apply_visitor(concept_visitor_right, concept_or_role_right);
    if (concept_visitor_left.result != nullptr && concept_visitor_right.result != nullptr) {
        return element_factory.make_inclusion_boolean(concept_visitor_left.result, concept_visitor_right.result);
    }
    RoleVisitor role_visitor_left(error_handler, element_factory);
    RoleVisitor role_visitor_right(error_handler, element_factory);
    boost::apply_visitor(role_visitor_left, concept_or_role_left);
    boost::apply_visitor(role_visitor_right, concept_or_role_right);
    if (role_visitor_left.result != nullptr && role_visitor_right.result != nullptr) {
        return element_factory.make_inclusion_boolean(role_visitor_left.result, role_visitor_right.result);
    }
    error_handler(node, "expected two concepts or two roles");
    throw std::runtime_error("Failed parse");
}

static std::shared_ptr<const core::Boolean>
parse(const stage_1::ast::NullaryBoolean& node, const dlplan::parsers::error_handler_type& error_handler, SyntacticElementFactory& element_factory) {
    return element_factory.make_nullary_boolean(parse(node.predicate, error_handler, element_factory));
}

static std::shared_ptr<const core::Concept>
parse(const stage_1::ast::AllConcept& node, const dlplan::parsers::error_handler_type& error_handler, SyntacticElementFactory& element_factory) {
    return element_factory.make_all_concept(
        parse(node.role, error_handler, element_factory),
        parse(node.concept_, error_handler, element_factory));
}

static std::shared_ptr<const core::Concept>
parse(const stage_1::ast::AndConcept& node, const dlplan::parsers::error_handler_type& error_handler, SyntacticElementFactory& element_factory) {
    return element_factory.make_and_concept(
        parse(node.concept_left, error_handler, element_factory),
        parse(node.concept_right, error_handler, element_factory));
}

static std::shared_ptr<const core::Concept>
parse(const stage_1::ast::BotConcept&, const dlplan::parsers::error_handler_type&, SyntacticElementFactory& element_factory) {
    return element_factory.make_bot_concept();
}

static std::shared_ptr<const core::Concept>
parse(const stage_1::ast::DiffConcept& node, const dlplan::parsers::error_handler_type& error_handler, SyntacticElementFactory& element_factory) {
    return element_factory.make_diff_concept(
        parse(node.concept_left, error_handler, element_factory),
        parse(node.concept_right, error_handler, element_factory));
}

static std::shared_ptr<const core::Concept>
parse(const stage_1::ast::EqualConcept& node, const dlplan::parsers::error_handler_type& error_handler, SyntacticElementFactory& element_factory) {
    return element_factory.make_equal_concept(
        parse(node.role_left, error_handler, element_factory),
        parse(node.role_right, error_handler, element_factory));
}

static std::shared_ptr<const core::Concept>
parse(const stage_1::ast::NotConcept& node, const dlplan::parsers::error_handler_type& error_handler, SyntacticElementFactory& element_factory) {
    return element_factory.make_not_concept(
        parse(node.concept_, error_handler, element_factory));
}

static std::shared_ptr<const core::Concept>
parse(const stage_1::ast::OneOfConcept& node, const dlplan::parsers::error_handler_type& error_handler, SyntacticElementFactory& element_factory) {
    return element_factory.make_one_of_concept(
        parse(node.constant, error_handler, element_factory));
}

static std::shared_ptr<const core::Concept>
parse(const stage_1::ast::OrConcept& node, const dlplan::parsers::error_handler_type& error_handler, SyntacticElementFactory& element_factory) {
    return element_factory.make_or_concept(
        parse(node.concept_left, error_handler, element_factory),
        parse(node.concept_right, error_handler, element_factory));
}

static std::shared_ptr<const core::Concept>
parse(const stage_1::ast::PrimitiveConcept& node, const dlplan::parsers::error_handler_type& error_handler, SyntacticElementFactory& element_factory) {
    return element_factory.make_primitive_concept(
        parse(node.predicate, error_handler, element_factory),
        parse(node.pos, error_handler, element_factory));
}

static std::shared_ptr<const core::Concept>
parse(const stage_1::ast::ProjectionConcept& node, const dlplan::parsers::error_handler_type& error_handler, SyntacticElementFactory& element_factory) {
    return element_factory.make_projection_concept(
        parse(node.role, error_handler, element_factory),
        parse(node.pos, error_handler, element_factory));
}

static std::shared_ptr<const core::Concept>
parse(const stage_1::ast::SomeConcept& node, const dlplan::parsers::error_handler_type& error_handler, SyntacticElementFactory& element_factory) {
    return element_factory.make_some_concept(
        parse(node.role, error_handler, element_factory),
        parse(node.concept_, error_handler, element_factory));
}

static std::shared_ptr<const core::Concept>
parse(const stage_1::ast::SubsetConcept& node, const dlplan::parsers::error_handler_type& error_handler, SyntacticElementFactory& element_factory) {
    return element_factory.make_subset_concept(
        parse(node.role_left, error_handler, element_factory),
        parse(node.role_right, error_handler, element_factory));
}

static std::shared_ptr<const core::Concept>
parse(const stage_1::ast::TopConcept&, const dlplan::parsers::error_handler_type&, SyntacticElementFactory& element_factory) {
    return element_factory.make_top_concept();
}

static std::shared_ptr<const core::Numerical>
parse(const stage_1::ast::ConceptDistanceNumerical& node, const dlplan::parsers::error_handler_type& error_handler, SyntacticElementFactory& element_factory) {
    return element_factory.make_concept_distance_numerical(
        parse(node.concept_left, error_handler, element_factory),
        parse(node.role, error_handler, element_factory),
        parse(node.concept_right, error_handler, element_factory));
}

static std::shared_ptr<const core::Numerical>
parse(const stage_1::ast::CountNumerical& node, const dlplan::parsers::error_handler_type& error_handler, SyntacticElementFactory& element_factory) {
    auto concept_or_role = parse(node.element, error_handler, element_factory);
    ConceptVisitor concept_visitor(error_handler, element_factory);
    boost::apply_visitor(concept_visitor, concept_or_role);
    if (concept_visitor.result != nullptr) return element_factory.make_count_numerical(concept_visitor.result);
    RoleVisitor role_visitor(error_handler, element_factory);
    boost::apply_visitor(role_visitor, concept_or_role);
    if (role_visitor.result != nullptr) return element_factory.make_count_numerical(role_visitor.result);
    error_handler(node, "expected two concepts or two roles");
    throw std::runtime_error("Failed parse");
}

static std::shared_ptr<const core::Numerical>
parse(const stage_1::ast::RoleDistanceNumerical& node, const dlplan::parsers::error_handler_type& error_handler, SyntacticElementFactory& element_factory) {
    return element_factory.make_role_distance_numerical(
        parse(node.role_left, error_handler, element_factory),
        parse(node.role, error_handler, element_factory),
        parse(node.role_right, error_handler, element_factory));
}

static std::shared_ptr<const core::Numerical>
parse(const stage_1::ast::SumConceptDistanceNumerical& node, const dlplan::parsers::error_handler_type& error_handler, SyntacticElementFactory& element_factory) {
    return element_factory.make_sum_concept_distance_numerical(
        parse(node.concept_left, error_handler, element_factory),
        parse(node.role, error_handler, element_factory),
        parse(node.concept_right, error_handler, element_factory));
}

static std::shared_ptr<const core::Numerical>
parse(const stage_1::ast::SumRoleDistanceNumerical& node, const dlplan::parsers::error_handler_type& error_handler, SyntacticElementFactory& element_factory) {
    return element_factory.make_sum_role_distance_numerical(
        parse(node.role_left, error_handler, element_factory),
        parse(node.role, error_handler, element_factory),
        parse(node.role_right, error_handler, element_factory));
}

static std::shared_ptr<const core::Role>
parse(const stage_1::ast::AndRole& node, const dlplan::parsers::error_handler_type& error_handler, SyntacticElementFactory& element_factory) {
    return element_factory.make_and_role(
        parse(node.role_left, error_handler, element_factory),
        parse(node.role_right, error_handler, element_factory));
}

static std::shared_ptr<const core::Role>
parse(const stage_1::ast::ComposeRole& node, const dlplan::parsers::error_handler_type& error_handler, SyntacticElementFactory& element_factory) {
    return element_factory.make_compose_role(
        parse(node.role_left, error_handler, element_factory),
        parse(node.role_right, error_handler, element_factory));
}

static std::shared_ptr<const core::Role>
parse(const stage_1::ast::DiffRole& node, const dlplan::parsers::error_handler_type& error_handler, SyntacticElementFactory& element_factory) {
    return element_factory.make_diff_role(
        parse(node.role_left, error_handler, element_factory),
        parse(node.role_right, error_handler, element_factory));
}

static std::shared_ptr<const core::Role>
parse(const stage_1::ast::IdentityRole& node, const dlplan::parsers::error_handler_type& error_handler, SyntacticElementFactory& element_factory) {
    return element_factory.make_identity_role(
        parse(node.concept_, error_handler, element_factory));
}

static std::shared_ptr<const core::Role>
parse(const stage_1::ast::InverseRole& node, const dlplan::parsers::error_handler_type& error_handler, SyntacticElementFactory& element_factory) {
    return element_factory.make_inverse_role(
        parse(node.role, error_handler, element_factory));
}

static std::shared_ptr<const core::Role>
parse(const stage_1::ast::NotRole& node, const dlplan::parsers::error_handler_type& error_handler, SyntacticElementFactory& element_factory) {
    return element_factory.make_not_role(
        parse(node.role, error_handler, element_factory));
}

static std::shared_ptr<const core::Role>
parse(const stage_1::ast::OrRole& node, const dlplan::parsers::error_handler_type& error_handler, SyntacticElementFactory& element_factory) {
    return element_factory.make_or_role(
        parse(node.role_left, error_handler, element_factory),
        parse(node.role_right, error_handler, element_factory));
}

static std::shared_ptr<const core::Role>
parse(const stage_1::ast::PrimitiveRole& node, const dlplan::parsers::error_handler_type& error_handler, SyntacticElementFactory& element_factory) {
    return element_factory.make_primitive_role(
        parse(node.predicate, error_handler, element_factory),
        parse(node.pos_1, error_handler, element_factory),
        parse(node.pos_2, error_handler, element_factory));
}

static std::shared_ptr<const core::Role>
parse(const stage_1::ast::RestrictRole& node, const dlplan::parsers::error_handler_type& error_handler, SyntacticElementFactory& element_factory) {
    return element_factory.make_restrict_role(
        parse(node.role, error_handler, element_factory),
        parse(node.concept_, error_handler, element_factory));
}

static std::shared_ptr<const core::Role>
parse(const stage_1::ast::TopRole&, const dlplan::parsers::error_handler_type&, SyntacticElementFactory& element_factory) {
    return element_factory.make_top_role();
}

static std::shared_ptr<const core::Role>
parse(const stage_1::ast::TransitiveClosureRole& node, const dlplan::parsers::error_handler_type& error_handler, SyntacticElementFactory& element_factory) {
    return element_factory.make_transitive_closure(
        parse(node.role, error_handler, element_factory));
}

static std::shared_ptr<const core::Role>
parse(const stage_1::ast::TransitiveReflexiveClosureRole& node, const dlplan::parsers::error_handler_type& error_handler, SyntacticElementFactory& element_factory) {
    return element_factory.make_transitive_reflexive_closure(
        parse(node.role, error_handler, element_factory));
};

class ConceptInnerVisitor {
private:
    const dlplan::parsers::error_handler_type& error_handler;
    SyntacticElementFactory& element_factory;

public:
    ConceptInnerVisitor(
        const dlplan::parsers::error_handler_type& error_handler, SyntacticElementFactory& element_factory)
        : error_handler(error_handler), element_factory(element_factory) { }

    std::shared_ptr<const core::Concept> result;

    template<typename Node>
    void operator()(const Node& node) {
        result = parse(node, error_handler, element_factory);
    }
};

static std::shared_ptr<const core::Concept>
parse(const stage_1::ast::ConceptInner& node, const dlplan::parsers::error_handler_type& error_handler, SyntacticElementFactory& element_factory) {
    ConceptInnerVisitor visitor(error_handler, element_factory);
    boost::apply_visitor(visitor, node);
    return visitor.result;
}

class RoleInnerVisitor {
private:
    const dlplan::parsers::error_handler_type& error_handler;
    SyntacticElementFactory& element_factory;

public:
    RoleInnerVisitor(
        const dlplan::parsers::error_handler_type& error_handler, SyntacticElementFactory& element_factory)
        : error_handler(error_handler), element_factory(element_factory) { }

    std::shared_ptr<const core::Role> result;

    template<typename Node>
    void operator()(const Node& node) {
        result = parse(node, error_handler, element_factory);
    }
};

static std::shared_ptr<const core::Role>
parse(const stage_1::ast::RoleInner& node, const dlplan::parsers::error_handler_type& error_handler, SyntacticElementFactory& element_factory) {
    RoleInnerVisitor visitor(error_handler, element_factory);
    boost::apply_visitor(visitor, node);
    return visitor.result;
}

class BooleanInnerVisitor {
private:
    const dlplan::parsers::error_handler_type& error_handler;
    SyntacticElementFactory& element_factory;

public:
    BooleanInnerVisitor(
        const dlplan::parsers::error_handler_type& error_handler, SyntacticElementFactory& element_factory)
        : error_handler(error_handler), element_factory(element_factory) { }

    std::shared_ptr<const core::Boolean> result;

    template<typename Node>
    void operator()(const Node& node) {
        result = parse(node, error_handler, element_factory);
    }
};

static std::shared_ptr<const core::Boolean>
parse(const stage_1::ast::BooleanInner& node, const dlplan::parsers::error_handler_type& error_handler, SyntacticElementFactory& element_factory) {
    BooleanInnerVisitor visitor(error_handler, element_factory);
    boost::apply_visitor(visitor, node);
    return visitor.result;
}

class NumericalInnerVisitor {
private:
    const dlplan::parsers::error_handler_type& error_handler;
    SyntacticElementFactory& element_factory;

public:
    NumericalInnerVisitor(
        const dlplan::parsers::error_handler_type& error_handler, SyntacticElementFactory& element_factory)
        : error_handler(error_handler), element_factory(element_factory) { }

    std::shared_ptr<const core::Numerical> result;

    template<typename Node>
    void operator()(const Node& node) {
        result = parse(node, error_handler, element_factory);
    }
};

static std::shared_ptr<const core::Numerical>
parse(const stage_1::ast::NumericalInner& node, const dlplan::parsers::error_handler_type& error_handler, SyntacticElementFactory& element_factory) {
    NumericalInnerVisitor visitor(error_handler, element_factory);
    boost::apply_visitor(visitor, node);
    return visitor.result;
}

class ConceptOrRoleInnerVisitor {
private:
    const dlplan::parsers::error_handler_type& error_handler;
    SyntacticElementFactory& element_factory;

public:
    ConceptOrRoleInnerVisitor(const dlplan::parsers::error_handler_type& error_handler, SyntacticElementFactory& element_factory)
        : error_handler(error_handler), element_factory(element_factory) { }

    boost::variant<std::shared_ptr<const core::Concept>, std::shared_ptr<const core::Role>> result;

    template<typename Node>
    void operator()(const Node& node) {
        result = parse(node, error_handler, element_factory);
    }
};

static boost::variant<std::shared_ptr<const core::Concept>, std::shared_ptr<const core::Role>>
parse(const stage_1::ast::ConceptOrRole& node, const dlplan::parsers::error_handler_type& error_handler, SyntacticElementFactory& element_factory) {
    ConceptOrRoleInnerVisitor visitor(error_handler, element_factory);
    boost::apply_visitor(visitor, node.inner);
    return visitor.result;
}

std::shared_ptr<const core::Concept>
parse(const stage_1::ast::Concept& node, const dlplan::parsers::error_handler_type& error_handler, SyntacticElementFactory& element_factory) {
    return parse(node.concept, error_handler, element_factory);
}

std::shared_ptr<const core::Role>
parse(const stage_1::ast::Role& node, const dlplan::parsers::error_handler_type& error_handler, SyntacticElementFactory& element_factory) {
    return parse(node.role, error_handler, element_factory);
}

std::shared_ptr<const Boolean>
parse(const stage_1::ast::Boolean& node, const dlplan::parsers::error_handler_type& error_handler, SyntacticElementFactory& element_factory) {
    return parse(node.boolean, error_handler, element_factory);
}

std::shared_ptr<const Numerical>
parse(const stage_1::ast::Numerical& node, const dlplan::parsers::error_handler_type& error_handler, SyntacticElementFactory& element_factory) {
    return parse(node.numerical, error_handler, element_factory);
}

}