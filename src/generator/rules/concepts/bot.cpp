#include "bot.h"

#include "../../generator_data.h"


namespace dlplan::generator::rules {
void BotConcept::generate_impl(const core::States& states, int target_complexity, GeneratorData& data, core::DenotationsCaches& caches) {
    assert(target_complexity == 1);
    core::SyntacticElementFactory& factory = data.m_factory;
    auto element = factory.make_bot_concept();
    auto denotations = element->evaluate(states, caches);
    if (data.m_concept_hash_table.insert(denotations).second) {
        std::get<2>(data.m_generated_features).push_back(element);
        data.m_concepts_by_iteration[target_complexity].push_back(std::move(element));
        increment_generated();
    }
}

std::string BotConcept::get_name() const {
    return "c_bot";
}

}
