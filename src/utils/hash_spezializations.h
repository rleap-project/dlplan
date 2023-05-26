#ifndef DLPLAN_SRC_UTILS_HASH_SPEZIALIZATIONS_H_
#define DLPLAN_SRC_UTILS_HASH_SPEZIALIZATIONS_H_

#include "../../include/dlplan/core.h"

#include <memory>
#include <vector>


/// Template specializations of std::hash
namespace std {
    template<> struct hash<dlplan::core::State> {
        size_t operator()(const dlplan::core::State& state) const noexcept;
    };
    template<>
    struct hash<unique_ptr<dlplan::core::ConceptDenotation>> {
        size_t operator()(const unique_ptr<dlplan::core::ConceptDenotation>& denotation) const noexcept;
    };
    template<>
    struct hash<unique_ptr<dlplan::core::RoleDenotation>> {
        size_t operator()(const unique_ptr<dlplan::core::RoleDenotation>& denotation) const noexcept;
    };
    template<>
    struct hash<unique_ptr<dlplan::core::ConceptDenotations>> {
        size_t operator()(const unique_ptr<dlplan::core::ConceptDenotations>& denotations) const noexcept;
    };
    template<>
    struct hash<unique_ptr<dlplan::core::RoleDenotations>> {
        size_t operator()(const unique_ptr<dlplan::core::RoleDenotations>& denotations) const noexcept;
    };
    template<>
    struct hash<unique_ptr<dlplan::core::BooleanDenotations>> {
        size_t operator()(const unique_ptr<dlplan::core::BooleanDenotations>& denotations) const noexcept;
    };
    template<>
    struct hash<unique_ptr<dlplan::core::NumericalDenotations>> {
        size_t operator()(const unique_ptr<dlplan::core::NumericalDenotations>& denotations) const noexcept;
    };
    template<> struct hash<vector<unsigned>> {
        size_t operator()(const vector<unsigned>& data) const noexcept;
    };
    template<> struct hash<vector<int>> {
        size_t operator()(const vector<int>& data) const noexcept;
    };
    template<> struct hash<std::array<int, 2>> {
        size_t operator()(const std::array<int, 2>& data) const noexcept;
    };
    template<> struct hash<std::array<int, 3>> {
        size_t operator()(const std::array<int, 3>& data) const noexcept;
    };
}

#endif
