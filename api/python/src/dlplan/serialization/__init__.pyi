from enum import Enum

from typing import Dict

from ..core import DenotationsCaches, VocabularyInfo, InstanceInfo, SyntacticElementFactory
from ..novelty import TupleGraph
from ..policy import Policy, PolicyBuilder
from ..state_space import StateSpace


class Data:
    vocabulary_infos: Dict[str, VocabularyInfo]
    instance_infos: Dict[str, InstanceInfo]
    syntactic_element_factories: Dict[str, SyntacticElementFactory]
    denotations_caches: Dict[str, DenotationsCaches]
    state_spaces: Dict[str, StateSpace]
    tuple_graphs: Dict[str, TupleGraph]
    policies: Dict[str, Policy]
    policy_builders: Dict[str, PolicyBuilder]
    def __init__(self) -> None: ...

def serialize(data: Data) -> str: ...

def deserialize(text: str) -> Data: ...
