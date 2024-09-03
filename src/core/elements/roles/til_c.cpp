#include "../../../../include/dlplan/core/elements/roles/til_c.h"

namespace dlplan::core
{

    void TilCRole::compute_result(const RoleDenotation &role_denot, const ConceptDenotation &concept_denot, RoleDenotation &result) const
    {
        auto concept_object_indicies = concept_denot.to_vector();

        std::unordered_set<ObjectIndex> current(concept_object_indicies.begin(), concept_object_indicies.end()), next;
        std::unordered_set<ObjectIndex> visited(concept_object_indicies.begin(), concept_object_indicies.end());

        std::unordered_map<ObjectIndex, std::unordered_set<ObjectIndex>> inv_edges;

        for(auto& pair : role_denot.to_vector()) {
            auto& [from, to] = pair;
            inv_edges[to].insert(from);
        }

        while(current.size() > 0) {
            for(auto& to : current) {
                for(auto& from : inv_edges[to]) {
                    if(! visited.contains(from)) {
                        result.insert(std::make_pair(from, to));
                        next.insert(from);
                    }
                }
            }
            visited.insert(next.begin(), next.end());
            current = next;
            next.clear();
        }
    }

    RoleDenotation TilCRole::evaluate_impl(const State &state, DenotationsCaches &caches) const
    {
        RoleDenotation denotation(state.get_instance_info()->get_objects().size());
        compute_result(
            *m_role->evaluate(state, caches),
            *m_concept->evaluate(state, caches),
            denotation);
        return denotation;
    }

    RoleDenotations TilCRole::evaluate_impl(const States &states, DenotationsCaches &caches) const
    {
        RoleDenotations denotations;
        denotations.reserve(states.size());
        auto role_denotations = m_role->evaluate(states, caches);
        auto concept_denotations = m_concept->evaluate(states, caches);
        for (size_t i = 0; i < states.size(); ++i)
        {
            RoleDenotation denotation(states[i].get_instance_info()->get_objects().size());
            compute_result(
                *(*role_denotations)[i],
                *(*concept_denotations)[i],
                denotation);
            denotations.push_back(caches.data.insert_unique(std::move(denotation)));
        }
        return denotations;
    }

    TilCRole::TilCRole(ElementIndex index, std::shared_ptr<VocabularyInfo> vocabulary_info, std::shared_ptr<const Role> role, std::shared_ptr<const Concept> concept_)
        : Role(index, vocabulary_info, role->is_static() && concept_->is_static()),
          m_role(role), m_concept(concept_) {}

    bool TilCRole::are_equal_impl(const Role &other) const
    {
        if (typeid(*this) == typeid(other))
        {
            const auto &other_derived = static_cast<const TilCRole &>(other);
            return m_is_static == other_derived.m_is_static && m_role == other_derived.m_role && m_concept == other_derived.m_concept;
        }
        return false;
    }

    size_t TilCRole::hash_impl() const
    {
        return hash_combine(m_is_static, m_role, m_concept);
    }

    RoleDenotation TilCRole::evaluate(const State &state) const
    {
        RoleDenotation denotation(state.get_instance_info()->get_objects().size());
        compute_result(
            m_role->evaluate(state),
            m_concept->evaluate(state),
            denotation);
        return denotation;
    }

    int TilCRole::compute_complexity_impl() const
    {
        return m_role->compute_complexity() + m_concept->compute_complexity() + 1;
    }

    void TilCRole::str_impl(std::stringstream &out) const
    {
        out << "r_til_c" << "(";
        m_role->str(out);
        out << ",";
        m_concept->str(out);
        out << ")";
    }

    int TilCRole::compute_evaluate_time_score_impl() const
    {
        return m_role->compute_evaluate_time_score() + m_concept->compute_evaluate_time_score() + SCORE_QUADRATIC;
    }

}

namespace std
{
    bool less<std::shared_ptr<const dlplan::core::TilCRole>>::operator()(
        const std::shared_ptr<const dlplan::core::TilCRole> &left_role,
        const std::shared_ptr<const dlplan::core::TilCRole> &right_role) const
    {
        return *left_role < *right_role;
    }

    std::size_t hash<dlplan::core::TilCRole>::operator()(const dlplan::core::TilCRole &role) const
    {
        return role.hash();
    }
}