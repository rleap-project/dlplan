#include <gtest/gtest.h>

#include "../../include/dlplan/state_space.h"
#include "../../include/dlplan/serialization.h"

#include <sstream>


namespace dlplan::serialization::tests {

TEST(DLPTests, SerializationTestEmpty) {
    std::stringstream buffer;
    dlplan::serialization::Data out_data{};
    dlplan::serialization::serialize(out_data, buffer);
    dlplan::serialization::Data in_data = dlplan::serialization::deserialize(buffer);
    EXPECT_EQ(in_data.state_spaces.size(), 0);
}

}
