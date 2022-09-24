#include "../../include/dlplan/novelty.h"


namespace dlplan::novelty {

NoveltyTable::NoveltyTable(std::shared_ptr<const NoveltyBase> novelty_base)
    : m_novelty_base(novelty_base) { }

NoveltyTable::NoveltyTable(const NoveltyTable& other) = default;

NoveltyTable& NoveltyTable::operator=(const NoveltyTable& other) = default;

NoveltyTable::NoveltyTable(NoveltyTable&& other) = default;

NoveltyTable& NoveltyTable::operator=(NoveltyTable&& other) = default;

NoveltyTable::~NoveltyTable() = default;

}