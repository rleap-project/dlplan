from typing import List, Tuple


class ConceptDenotation:
    def to_sorted_vector(self) -> List[int]: ...
    def __str__(self) -> str: ...
    def __repr__(self) -> str: ...


class RoleDenotation:
    def to_sorted_vector(self) -> List[Tuple[int, int]]: ...
    def __str__(self) -> str: ...
    def __repr__(self) -> str: ...


class DenotationsCaches:
    pass


class Constant:
    pass


class Predicate:
    pass


class VocabularyInfo:
    def __copy__(self) -> "VocabularyInfo": ...
    def __deepcopy__(self, other: "VocabularyInfo") -> "VocabularyInfo": ...
    def add_predicate(self, name: str, arity: int, is_static: bool) -> Predicate: ...
    def add_constant(self, name: str) -> Constant: ...
    def get_predicates(self) -> List[Predicate]: ...
    def get_constants(self) -> List[Constant]: ...
    def get_predicate(self, name: str) -> Predicate: ...
    def get_constant(self, name: str) -> Constant: ...


class Object:
    pass


class Atom:
    pass


class InstanceInfo:
    pass


class State:
    pass


class BaseElement:
    pass


class Concept:
    pass


class Role:
    pass


class Boolean:
    pass


class Numerical:
    pass


class SyntacticElementFactory:
    pass