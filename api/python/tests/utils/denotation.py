from dlplan.core import InstanceInfo, ConceptDenotation, RoleDenotation

from typing import List, Tuple


def create_concept_denotation(instance: InstanceInfo, object_names: List[str]):
    num_objects = len(instance.get_objects())
    denotation = ConceptDenotation(num_objects)
    for object_name in object_names:
        object = instance.get_object(object_name)
        denotation.insert(object.get_index())
    return denotation


def create_role_denotation(instance: InstanceInfo, pairs_of_object_names: List[Tuple[str, str]]):
    num_objects = len(instance.get_objects())
    denotation = RoleDenotation(num_objects)
    for object_name_1, object_name_2 in pairs_of_object_names:
        object_first = instance.get_object(object_name_1)
        object_second = instance.get_object(object_name_2)
        denotation.insert((object_first.get_index(), object_second.get_index()))
    return denotation
