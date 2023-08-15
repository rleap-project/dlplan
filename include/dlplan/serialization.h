#include "novelty.h"
#include "state_space.h"

#include <istream>
#include <unordered_map>

using namespace dlplan;


namespace dlplan::serialization {

/// @brief Encapsulates a forest of library objects for serialization and deserialization.
struct Data {
    // The key is a string to allow the user the encode more complex nestings.
    // The value is a shared_ptr<T> to the allow sharing between other objects.
    std::unordered_map<std::string, std::shared_ptr<state_space::StateSpace>> state_spaces;
    std::unordered_map<std::string, std::shared_ptr<novelty::TupleGraph>> tuple_graphs;
};

/// @brief Serializes the data and places it into the out_buffer.
/// @param data
/// @param out_buffer
void serialize(const Data& data, std::ostream& out_buffer);

/// @brief Deserializes the data from the buffer.
/// @param buffer
/// @return
Data deserialize(std::istream& buffer);

}
