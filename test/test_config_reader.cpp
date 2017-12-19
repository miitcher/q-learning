#include <gtest/gtest.h>
#include "../src/config_reader.hpp"
#include <stdexcept>

TEST(test_config_reader, test_readAgentConfigFile) {
    std::string agentConfigFilename = "../test/files/configAgent_test.config";

    std::vector<Actor> actors;
    std::vector<Sensor> sensors;
    AgentShape agentShape;
    unsigned int agentCount;
    std::string qtableFilename;
    bool drawGraphics;
    bool doNotTrain;

    readAgentLearnerConfigFile(agentConfigFilename, actors, sensors,
        agentShape, agentCount, qtableFilename, drawGraphics, doNotTrain);

    // Actor 0
    EXPECT_EQ(actors[0].getID(), 1);
    EXPECT_EQ(actors[0].getQuantizationSteps(), 256);
    EXPECT_EQ(actors[0].getMinAngle(), -1);

    // Sensor 1
    EXPECT_EQ(sensors[1].getID(), 2);
    EXPECT_EQ(sensors[1].getQuantizationSteps(), 512);
    EXPECT_EQ(sensors[1].getMaxAngle(), float(2.5));

    // The other variables.
    EXPECT_EQ(agentShape, 2); // TODO: Define agentShape for Box2D.
    EXPECT_EQ(agentCount, unsigned(3));
    EXPECT_EQ(qtableFilename, "qtabletestfile4.bin");
    EXPECT_EQ(drawGraphics, false);
    EXPECT_EQ(doNotTrain, false);

    /* Debug code for printing the variables.
    std::cout << std::endl << "Retvals:" << std::endl;
    for (auto actor : actors) {
        std::cout << actor << std::endl;
    }
    for (auto sensor : sensors) {
        std::cout << sensor << std::endl;
    }
    std::cout << "agentShape: " << agentShape << std::endl
        << "agentCount: " << agentCount << std::endl
        << "qtableFilename: " << qtableFilename << std::endl
        << "drawGraphics: " << drawGraphics << std::endl;
    */
}

TEST(test_config_reader, test_readAgentConfigFile_wrong_filename) {
    std::string agentConfigFilename = "../test/files/configAgent_test.config2";

    std::vector<Actor> actors;
    std::vector<Sensor> sensors;
    AgentShape agentShape;
    unsigned int agentCount;
    std::string qtableFilename;
    bool drawGraphics;
    bool doNotTrain;

    try {
        readAgentLearnerConfigFile(agentConfigFilename, actors, sensors,
            agentShape, agentCount, qtableFilename, drawGraphics, doNotTrain);
    } catch (std::runtime_error& e) {
        EXPECT_STREQ(e.what(),
            "File '../test/files/configAgent_test.config2' does not exist.");
    }
}

TEST(test_config_reader, test_readAgentConfigFile_faulty_file) {
    std::string agentConfigFilename = "../test/files/config_faulty_test.config";

    std::vector<Actor> actors;
    std::vector<Sensor> sensors;
    AgentShape agentShape;
    unsigned int agentCount;
    std::string qtableFilename;
    bool drawGraphics;
    bool doNotTrain;

    try {
        readAgentLearnerConfigFile(agentConfigFilename, actors, sensors,
            agentShape, agentCount, qtableFilename, drawGraphics, doNotTrain);
    } catch (std::runtime_error& e) {
        EXPECT_STREQ(e.what(),
            "FAULTY LINE:11: JointSensor, 1, 512, 0.0");
    }
}
