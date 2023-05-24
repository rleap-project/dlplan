from ..core import VocabularyInfo


class StateSpace:
    pass


class GeneratorExitCode:
    pass


class GeneratorResultWrapper:
    pass


def generate_state_space(domain_file: str, instance_file: str, vocabulary_info: VocabularyInfo = None, index: int = -1, max_time: int = 1000000) -> GeneratorResultWrapper: ...
