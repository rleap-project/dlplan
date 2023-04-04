#include "../../include/dlplan/state_space.h"


using namespace dlplan::core;
using namespace std::string_literals;

namespace dlplan::state_space::reader {

extern StateSpace read(std::shared_ptr<const VocabularyInfo> vocabulary_info=nullptr, int index=-1);

}