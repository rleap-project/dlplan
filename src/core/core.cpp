#include "../../include/dlp/core.h"

#include "elements/concept.h"
#include "elements/role.h"
#include "elements/numerical.h"
#include "elements/boolean.h"

#include "element_factory.h"
#include "instance_info.h"
#include "atom.h"
#include "state.h"


namespace dlp {
namespace core {

InstanceInfo::InstanceInfo() { }

InstanceInfo::InstanceInfo(const InstanceInfo& other) : m_pImpl(*(other.m_pImpl)) { }

InstanceInfo::~InstanceInfo() { }

State InstanceInfo::convert_state(const Index_Vec& atom_idxs) { }


Atom::Atom(std::shared_ptr<InstanceInfo> parent, const AtomImpl& impl) : m_parent(parent), m_pImpl(impl) { }

Atom::Atom(const Atom& other) : m_pImpl(*(other.m_pImpl)) { }

Atom::~Atom() { }


State::State(std::shared_ptr<InstanceInfo> parent, StateImpl impl) : m_parent(parent), m_pImpl(impl) { }

State::State(const State& other) : m_pImpl(*(other.m_pImpl)) {}

State::~State() { }

std::string State::str() const { /* tba */ }

const std::shared_ptr<InstanceInfo>& State::parent() const { return m_parent; }



ConceptElement::ConceptElement(std::shared_ptr<InstanceInfo> parent, element::ConceptElement_Ptr pImpl)
    : Element<Concepts>(parent), m_pImpl(pImpl) { }

ConceptElement::ConceptElement(const ConceptElement& other)
    : Element<Concepts>(other.m_parent), m_pImpl(*(other.m_pImpl)) { }

Concepts ConceptElement::evaluate(const State& state) const {
    if (m_parent != state.parent()) {
        throw std::invalid_argument("ConceptElement::evaluate - instance information between state and element do not match.");
    }
    (*m_pImpl)->evaluate(state);
}

unsigned ConceptElement::complexity() const {
    return (*m_pImpl)->complexity();
}

RoleElement::RoleElement(std::shared_ptr<InstanceInfo> parent, element::RoleElement_Ptr pImpl)
    : Element<Roles>(parent), m_pImpl(pImpl) { }

RoleElement::RoleElement(const RoleElement& other)
    : Element<Roles>(other.m_parent), m_pImpl(*(other.m_pImpl)) { }

Roles RoleElement::evaluate(const State& state) const {
    if (m_parent != state.parent()) {
        throw std::invalid_argument("RoleElement::evaluate - instance information between state and element do not match.");
    }
    (*m_pImpl)->evaluate(state);
}

unsigned RoleElement::complexity() const {
    return (*m_pImpl)->complexity();
}

NumericalElement::NumericalElement(std::shared_ptr<InstanceInfo> parent, element::NumericalElement_Ptr pImpl)
    : Element<int>(parent), m_pImpl(pImpl) { }

NumericalElement::NumericalElement(const NumericalElement& other)
    : Element<int>(other.m_parent), m_pImpl(*(other.m_pImpl)) { }

int NumericalElement::evaluate(const State& state) const {
    if (m_parent != state.parent()) {
        throw std::invalid_argument("NumericalElement::evaluate - instance information between state and element do not match.");
    }
    (*m_pImpl)->evaluate(state);
}

unsigned NumericalElement::complexity() const {
    return (*m_pImpl)->complexity();
}

BooleanElement::BooleanElement(std::shared_ptr<InstanceInfo> parent, element::BooleanElement_Ptr pImpl)
    : Element<bool>(parent), m_pImpl(pImpl) { }

BooleanElement::BooleanElement(const BooleanElement& other)
    : Element<bool>(other.m_parent), m_pImpl(*(other.m_pImpl)) { }

bool BooleanElement::evaluate(const State& state) const {
    if (m_parent != state.parent()) {
        throw std::invalid_argument("BooleanElement::evaluate - instance information between state and element do not match.");
    }
    (*m_pImpl)->evaluate(state);
}

unsigned BooleanElement::complexity() const {
    return (*m_pImpl)->complexity();
}


ElementFactory::ElementFactory() { }

ElementFactory::~ElementFactory() { }

Atom InstanceInfo::add_atom(const std::string &predicate_name, const Name_Vec &object_names, bool is_static, bool add_goal_version) {
    return Atom(std::make_shared<InstanceInfo>(*this), m_pImpl->add_atom(predicate_name, object_names, is_static, add_goal_version));
}

ConceptElement ElementFactory::parse_concept_element(const InstanceInfo& info, const std::string &description) {
    return ConceptElement(std::make_shared<InstanceInfo>(info), m_pImpl->parse_concept_element(*info.m_pImpl, description));
}

RoleElement ElementFactory::parse_role_element(const InstanceInfo& info, const std::string &description) {
    return RoleElement(std::make_shared<InstanceInfo>(info), m_pImpl->parse_role_element(*info.m_pImpl, description));
}

NumericalElement ElementFactory::parse_numerical_element(const InstanceInfo& info, const std::string &description) {
    return NumericalElement(std::make_shared<InstanceInfo>(info), m_pImpl->parse_numerical_element(*info.m_pImpl, description));
}

BooleanElement ElementFactory::parse_boolean_element(const InstanceInfo& info, const std::string &description) {
    return BooleanElement(std::make_shared<InstanceInfo>(info), m_pImpl->parse_boolean_element(*info.m_pImpl, description));
}

}
}
