
class Object:
    """ An object occuring in an instance. """
    def __init__(self, index, name):
        self.index = index
        self.name = name

class Predicate:
    """ A predicate occuring in a domain. """
    def __init__(self, index, name, arity):
        self.index = index
        self.name = name
        self.arity = arity

class Atom:
    """ An atom occuring in an instance. """
    def __init__(self, index, name, predicate, objects, is_static):
        self.index = index
        self.name = name
        self.predicate = predicate
        self.objects = objects
        self.is_static = is_static

class State:
    """ A state over ground atoms of an instance. """
    def __init__(self, index, atoms):
        self.index = index
        self.atoms = atoms

class TransitionSystem:
    """ The transition system of an instance. """
    def __init__(self, states, forward_transitions, backward_transitions, goals, deadends, dynamic_atoms, static_atoms, predicates, objects, initial):
        self.states = states
        self.forward_transitions = forward_transitions
        self.backward_transitions = backward_transitions
        self.goals = goals
        self.deadends = deadends
        self.dynamic_atoms = dynamic_atoms
        self.static_atoms = static_atoms
        self.predicates = predicates
        self.objects = objects
        self.initial = initial
