#include "interactor.hpp"
#include "agent_learner.hpp"
#include "agent_manager.hpp"
#include "config_reader.hpp"
#include <iostream>
#include <string>
#include <vector>
#include <stdexcept>
#include <cairo.h>

// The main will controll IO and graphics, and start the program.
int main() {


    // trying the cairo library
    // this prints hello.png in the build folder
    cairo_surface_t *surface =
            cairo_image_surface_create (CAIRO_FORMAT_ARGB32, 240, 80);
    cairo_t *cr =
            cairo_create (surface);

    cairo_select_font_face (cr, "serif", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_BOLD);
    cairo_set_font_size (cr, 32.0);
    cairo_set_source_rgb (cr, 0.0, 0.0, 1.0);
    cairo_move_to (cr, 10.0, 50.0);
    cairo_show_text (cr, "Hello, world");

    cairo_destroy (cr);
    cairo_surface_write_to_png (surface, "hello.png");
    cairo_surface_destroy (surface);


    std::cout << "--- q-learning-9 ---" << std::endl;

    // File that dictates the learning AgentLearner.
    std::string agentConfigFilename = "../src/configAgent.config";

    // Initialize variables
    std::vector<Actor> actors;
    std::vector<Sensor> sensors;
    AgentShape agentShape;
    unsigned int agentCount;
    std::string qtableFilename;
    bool drawGraphics;
    // Set variables. This is error prone, because the user can modify the
    // configuration file.
    try {
        readAgentLearnerConfigFile(agentConfigFilename, actors, sensors,
            agentShape, agentCount, qtableFilename, drawGraphics);
    } catch (std::runtime_error& e) {
        std::cout << "An error occured, while reading the config file: "
            << e.what() << std::endl;
        return -1;
    }

    // Set AgentManager
    AgentManager agentManager(actors, sensors, agentShape, agentCount,
        qtableFilename, drawGraphics);

    // Start learning
    agentManager.initRun();

    return 0;
}
