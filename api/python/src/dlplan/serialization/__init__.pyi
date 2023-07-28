from enum import Enum

from typing import List

from ..state_space import StateSpace

class Data:
    state_spaces: List[StateSpace]
    def __init__(self) -> None: ...

def serialize(data: Data) -> str: ...

def deserialize(text: str) -> Data: ...
