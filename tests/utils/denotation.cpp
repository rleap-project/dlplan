#include "denotation.h"


namespace dlplan::tests {

ConceptDenotation create_concept_denotation(const InstanceInfo& instance, const std::vector<std::string>& object_names) {
    int num_objects = instance.get_objects().size();
    ConceptDenotation denotation(num_objects);
    for (const auto& object_name : object_names) {
        const auto& object = instance.get_object(object_name);
        denotation.insert(object.get_index());
    }
    return denotation;
}

RoleDenotation create_role_denotation(const InstanceInfo& instance, const std::vector<std::pair<std::string, std::string>>& object_pair_names) {
    int num_objects = instance.get_objects().size();
    RoleDenotation denotation(num_objects);
    for (const auto& pair : object_pair_names) {
        const auto& object_first = instance.get_object(pair.first);
        const auto& object_second = instance.get_object(pair.second);
        denotation.insert(std::make_pair(object_first.get_index(), object_second.get_index()));
    }
    return denotation;
}

}
