from enum import Enum

from typing import Dict

from ..novelty import TupleGraph
from ..state_space import StateSpace


class Data:
    state_spaces: Dict[str, StateSpace]
    tuple_graphs: Dict[str, TupleGraph]
    def __init__(self) -> None: ...

def serialize(data: Data) -> str: ...

def deserialize(text: str) -> Data: ...
