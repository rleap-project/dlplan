#include "policy.h"

namespace dlplan::policy {

template<typename PARENT_T, typename SUB_T>
bool PolicyMinimizer::check_subtype_equality(const std::unordered_set<std::shared_ptr<const PARENT_T>>& values) const {
    return std::all_of(values.begin(), values.end(), [](const std::shared_ptr<const PARENT_T>& value){ return std::dynamic_pointer_cast<const SUB_T>(value); });
}

template<typename T>
bool PolicyMinimizer::check_feature_index_equality(const std::unordered_set<std::shared_ptr<const T>>& values) const {
    if (values.empty()) return true;
    return std::all_of(values.begin(), values.end(), [index=(*(values.begin()))->get_base_feature()->get_index()](const std::shared_ptr<const T>& value){ return value->get_base_feature()->get_index() == index; } );
}

template<typename PARENT_T, typename SUB_T1, typename SUB_T2>
std::unordered_set<std::shared_ptr<const PARENT_T>> PolicyMinimizer::compute_mergeable_values(const std::vector<std::vector<std::shared_ptr<const PARENT_T>>>& values_by_rule) const {
    std::unordered_map<std::shared_ptr<const PARENT_T>, int> value_frequencies;
    for (const auto& values : values_by_rule) {
        for (const auto& v : values) {
            if (value_frequencies.count(v)) {
                ++value_frequencies[v];
            } else {
                value_frequencies[v] = 1;
            }
        }
    }
    std::unordered_set<std::shared_ptr<const PARENT_T>> symmetric_diff;
    for (const auto& p : value_frequencies) {
        if (value_frequencies[p.first] == 1) {
            symmetric_diff.insert(p.first);
        }
    }
    if (!check_feature_index_equality(symmetric_diff) ||
        !(check_subtype_equality<PARENT_T, SUB_T1>(symmetric_diff) || check_subtype_equality<PARENT_T, SUB_T2>(symmetric_diff))) {
        return {};
    }
    return symmetric_diff;
}

template<typename T>
std::vector<std::shared_ptr<const T>> PolicyMinimizer::compute_merged_values(const std::vector<std::shared_ptr<const T>>& original_values, const std::unordered_set<std::shared_ptr<const T>>& removed_values, PolicyBuilder& builder) const {
    std::vector<std::shared_ptr<const T>> values;
    for (const auto& value : original_values) {
        if (removed_values.count(value)) {
            continue;
        }
        values.push_back(value->visit(builder));
    }
    return values;
}

}