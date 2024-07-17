#include <gtest/gtest.h>

#include "../utils/denotation.h"

#include "../../include/dlplan/core.h"

#include <sstream>

using namespace dlplan::core;


namespace dlplan::tests::core {

TEST(DLPTests, RoleTilC) {
    auto vocabulary = std::make_shared<VocabularyInfo>();
    auto predicate_0 = vocabulary->add_predicate("role", 2);
    auto predicate_1 = vocabulary->add_predicate("concept", 1);
    auto instance = std::make_shared<InstanceInfo>(0, vocabulary);
    auto atom_0 = instance->add_atom("role", {"A", "B"});
    auto atom_1 = instance->add_atom("role", {"B", "C"});
    auto atom_2 = instance->add_atom("role", {"C", "D"});
    auto atom_3 = instance->add_atom("concept", {"D"});

    State state_0(0, instance, {atom_0, atom_1, atom_2, atom_3});

    SyntacticElementFactory factory(vocabulary);

    auto role = factory.parse_role("r_til_c(r_primitive(role,0,1),c_primitive(concept,0))");
    std::cout << role->evaluate(state_0);
    EXPECT_EQ(role->evaluate(state_0), create_role_denotation(*instance, {{"A", "B"},{"B", "C"},{"C", "D"}}));
}

TEST(DLPTests, RoleTilCWithGrid) {
    auto vocabulary = std::make_shared<VocabularyInfo>();
    auto predicate_0 = vocabulary->add_predicate("role", 2);
    auto predicate_1 = vocabulary->add_predicate("concept", 1);
    auto instance = std::make_shared<InstanceInfo>(0, vocabulary);

    std::vector<Atom> atoms;

    auto cell_str = [](int x, int y) {
        std::stringstream ss;
        ss << "C_" << x << "_" << y;
        return ss.str();
    };

    auto add = [&](int from_x, int from_y, int to_x, int to_y) {
        auto atom = instance->add_atom("role", {cell_str(from_x, from_y), cell_str(to_x, to_y)});
        atoms.push_back(atom);
    };

    int size_x = 3, size_y = 3;
    for(int y = 0; y < size_y; y++) {
        for(int x = 0; x < size_x; x++) {
            if(x > 0) add(x, y, x - 1, y);
            if(x < size_x - 1) add(x, y, x + 1, y);
            if(y > 0) add(x, y, x, y - 1);
            if(y < size_y - 1) add(x, y, x, y + 1);
        }
    }

    atoms.push_back(instance->add_atom("concept", {cell_str(0, 0)}));
    atoms.push_back(instance->add_atom("concept", {cell_str(size_x - 1, 0)}));

    State state_0(0, instance, atoms);

    SyntacticElementFactory factory(vocabulary);

    auto role = factory.parse_role("r_til_c(r_primitive(role,0,1),c_primitive(concept,0))");
    EXPECT_EQ(role->evaluate(state_0), create_role_denotation(*instance, {
        {cell_str(0, 1), cell_str(0, 0)},
        {cell_str(0, 2), cell_str(0, 1)},

        {cell_str(1, 0), cell_str(0, 0)},
        {cell_str(1, 0), cell_str(2, 0)},

        {cell_str(1, 1), cell_str(0, 1)},
        {cell_str(1, 1), cell_str(1, 0)},
        {cell_str(1, 1), cell_str(2, 1)},

        {cell_str(1, 2), cell_str(1, 1)},
        {cell_str(1, 2), cell_str(0, 2)},
        {cell_str(1, 2), cell_str(2, 2)},

        {cell_str(2, 1), cell_str(2, 0)},
        {cell_str(2, 2), cell_str(2, 1)},
    }));
}

}
