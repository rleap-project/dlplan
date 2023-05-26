#include "hash_spezializations.h"

#include "hashing.h"

#include "../../include/dlplan/core.h"


namespace std {
    size_t hash<dlplan::core::State>::operator()(const dlplan::core::State& state) const noexcept {
        return state.hash();
    }
    size_t hash<unique_ptr<dlplan::core::ConceptDenotation>>::operator()(const unique_ptr<dlplan::core::ConceptDenotation>& denotation) const noexcept {
        return denotation->hash();
    }
    size_t hash<unique_ptr<dlplan::core::RoleDenotation>>::operator()(const unique_ptr<dlplan::core::RoleDenotation>& denotation) const noexcept {
        return denotation->hash();
    }
    size_t hash<unique_ptr<dlplan::core::ConceptDenotations>>::operator()(const unique_ptr<dlplan::core::ConceptDenotations>& denotations) const noexcept {
        size_t seed = 0;
        for (const auto denot_ptr : *denotations) {
            dlplan::utils::hash_combine(seed, denot_ptr);
        }
        return seed;
    }
    size_t hash<unique_ptr<dlplan::core::RoleDenotations>>::operator()(const unique_ptr<dlplan::core::RoleDenotations>& denotations) const noexcept {
        size_t seed = 0;
        for (const auto denot_ptr : *denotations) {
            dlplan::utils::hash_combine(seed, denot_ptr);
        }
        return seed;
    }
    size_t hash<unique_ptr<dlplan::core::BooleanDenotations>>::operator()(const unique_ptr<dlplan::core::BooleanDenotations>& denotations) const noexcept {
        return hash<dlplan::core::BooleanDenotations>()(*denotations);
    }
    size_t hash<unique_ptr<dlplan::core::NumericalDenotations>>::operator()(const unique_ptr<dlplan::core::NumericalDenotations>& denotations) const noexcept {
        return hash<dlplan::core::NumericalDenotations>()(*denotations);
    }
    size_t hash<vector<unsigned>>::operator()(const vector<unsigned>& data) const noexcept {
        size_t seed = data.size();
        for (unsigned value : data) {
            dlplan::utils::hash_combine(seed, value);
        }
        return seed;
    }
    size_t hash<vector<int>>::operator()(const vector<int>& data) const noexcept {
        size_t seed = data.size();
        for (int value : data) {
            dlplan::utils::hash_combine(seed, value);
        }
        return seed;
    }
    size_t hash<std::array<int, 2>>::operator()(const std::array<int, 2>& data) const noexcept {
        size_t seed = data.size();
        for (int value : data) {
            dlplan::utils::hash_combine(seed, value);
        }
        return seed;
    }
    size_t hash<std::array<int, 3>>::operator()(const std::array<int, 3>& data) const noexcept {
        size_t seed = data.size();
        for (int value : data) {
            dlplan::utils::hash_combine(seed, value);
        }
        return seed;
    }
}

