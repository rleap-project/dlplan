#include "utils.h"

std::shared_ptr<const VocabularyInfo> construct_vocabulary_info() {
    std::shared_ptr<VocabularyInfo> vocabulary_info = std::make_shared<VocabularyInfo>();
    vocabulary_info->add_predicate("role", 2);
    vocabulary_info->add_predicate("concept", 1);
    return vocabulary_info;
}

SyntacticElementFactory construct_syntactic_element_factory(std::shared_ptr<const VocabularyInfo> vocabulary_info) {
    return SyntacticElementFactory(vocabulary_info);
}