#include "bot.h"

#include "../../generator_data.h"

#include "../../../core/elements/concepts/bot.h"


namespace dlplan::generator::rules {
void BotConcept::generate_impl(const core::States& states, int target_complexity, GeneratorData& data, core::DenotationsCaches& caches) {
    assert(target_complexity == 1);
    core::SyntacticElementFactory& factory = data.m_factory;
    auto element = factory.make_bot_concept();
    auto denotations = element.get_element()->evaluate(states, caches);
    if (data.m_concept_hash_table.insert(denotations).second) {
        data.m_reprs.push_back(element.compute_repr());
        data.m_concepts_by_iteration[target_complexity].push_back(std::move(element));
        increment_generated();
    }
}

std::string BotConcept::get_name() const {
    return core::element::BotConcept::get_name();
}

}
