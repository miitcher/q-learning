#include <gtest/gtest.h>
#include "../src/interactor.hpp"
#include <iostream>

TEST(test_interactor, test_Interactor) {
    Interactor a(42, "generic interactor", 1256, -3.14, 1.1);

    EXPECT_EQ(a.getID(), 42);
    EXPECT_EQ(a.getDescription(), "generic interactor");
    EXPECT_EQ(a.getQuantizationSteps(), 1256);
    EXPECT_EQ(a.getMinAngle(), float(-3.14));
    EXPECT_EQ(a.getMaxAngle(), float(1.1));
}

TEST(test_interactor, test_Actor) {
    Actor a(23, "generic actor", 126, 1, 2, {Still, Clockwise});

    EXPECT_EQ(a.getID(), 23);
    EXPECT_EQ(a.getDescription(), "generic actor");
    EXPECT_EQ(a.getQuantizationSteps(), 126);
    EXPECT_EQ(a.getMinAngle(), float(1));
    EXPECT_EQ(a.getMaxAngle(), float(2));
    EXPECT_EQ(a.getNumberOfActions(), 2);

    auto actions = a.getActions();
    //for (auto i : actions) { std::cout << "Action: " << i << std::endl; }
    EXPECT_EQ(actions[0], Still);
    EXPECT_EQ(actions[1], Clockwise);
}

TEST(test_interactor, test_Sensor) {
    Sensor a(231, "generic sensor", 126, 10, 2);

    EXPECT_EQ(a.getID(), 231);
    EXPECT_EQ(a.getDescription(), "generic sensor");
    EXPECT_EQ(a.getQuantizationSteps(), 126);
    EXPECT_EQ(a.getMinAngle(), float(10));
    EXPECT_EQ(a.getMaxAngle(), float(2));
}

TEST(test_interactor, test_JointActor) {
    JointActor a(2, 456, 0.0, 0);

    EXPECT_EQ(a.getID(), 2);
    EXPECT_EQ(a.getDescription(), "Actor of rotating 2D joint.");
    EXPECT_EQ(a.getQuantizationSteps(), 456);
    EXPECT_EQ(a.getMinAngle(), float(0));
    EXPECT_EQ(a.getMaxAngle(), float(0));
    EXPECT_EQ(a.getNumberOfActions(), 3);
}

TEST(test_interactor, test_JointSensor) {
    JointSensor a(1, 46, 2, 1);

    EXPECT_EQ(a.getID(), 1);
    EXPECT_EQ(a.getDescription(), "Sensor of 2D joints angle.");
    EXPECT_EQ(a.getQuantizationSteps(), 46);
    EXPECT_EQ(a.getMinAngle(), float(2));
    EXPECT_EQ(a.getMaxAngle(), float(1));
}

TEST(test_interactor, test_XAxisSensor) {
    XAxisSensor a(6);

    EXPECT_EQ(a.getID(), 6);
    EXPECT_EQ(a.getDescription(), "Sensor of Agents position on the x-axis.");
    EXPECT_EQ(a.getQuantizationSteps(), 0);
    EXPECT_EQ(a.getMinAngle(), float(0));
    EXPECT_EQ(a.getMaxAngle(), float(0));
}
