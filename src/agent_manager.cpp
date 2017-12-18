#include "agent_manager.hpp"
#include "agent_learner.hpp"
#include "interactor.hpp"
#include "simulation.hpp"
#include <vector>
#include <thread>
#include <chrono>
#include <iostream>
#include <algorithm> // for_each
#include <mutex>
#include <utility>
#include <math.h> // pow
#include <atomic>
#include <system_error>
#include <fstream>
#include <random>
#include <sstream>

// The filename for the fittest saved Q-table, that is used for the evolution
// of the Agents, when multiple Agents are learning.
std::string evolutionFittestQtableFilename
    = "generations_fittest_qtable_in_evolution.bin";

// Controlls the use of the "evolutionFittestQtableFilename" file.
std::mutex evolutionFittestFile_mutex;

// Controlls the use of cout between threads.
std::mutex cout_mutex;

// String for keeping track of what the saved filename is in a thread.
// Protected by mutex.
std::string savedQtableFilename("");

// Mutex protecting savedQtableFilename.
std::mutex savedQtableFilename_mutex;

// Counter that will keep track of how many Agents are waiting on to
// update their Q-table with the fittest Q-table.
// 0 means that no agents are waiting.
std::atomic_uint agentsWaitingToEvolve_counter(0);

/**
Enable log and debug print with threads.
*/
std::atomic_bool useLogging(false);
std::atomic_bool useDebugging(false);

/**
Atomics are thread safe, and they are used here to communicate
between threads.
*/
std::atomic_bool endThreads(false);
std::atomic_bool pauseThreads(false);
/**
When saveQtableInThread is true, then one thread will save its Qtable to file.
A threads Qtable can be saved if canSaveQtable is true.
*/
std::atomic_bool saveQtableInThread(false);
// If only on Agent is used, then no goal is used, but otherwise it is used.
std::atomic_bool useEvolution(true);
std::atomic_bool anAgentHasReachedTheGoal(false);

// log and debug are helper functions that manage thread safe writing to cout.
void log(std::string str0, std::string str1, std::string str2, std::string str3) {
    if (useLogging) {
        cout_mutex.lock();
        std::cout << str0 << str1 << str2 << str3 << std::endl;
        cout_mutex.unlock();
    }
}

void log(unsigned integer0, std::string str1, unsigned integer2, std::string str3) {
    log(std::to_string(integer0), str1, std::to_string(integer2), str3);
}

void log(std::string str0) {
    log(str0, "", "", "");
}

void debug(std::string str0, std::string str1, std::string str2) {
    if (useDebugging) {
        cout_mutex.lock();
        std::cout << str0 << str1 << str2 << std::endl;
        cout_mutex.unlock();
    }
}

void debug(std::string str0, int integer1) {
    debug(str0, std::to_string(integer1), "");
}

void debug(std::string str0, std::string str1) {
    debug(str0, str1, "");
}

void debug(std::string str0) {
    debug(str0, "", "");
}

void debug(int integer0, std::string str1) {
    debug(std::to_string(integer0), str1, "");
}

void moveAgentToBeginning(Simulation &simulation, AgentLearner &agentLearner,
    double &lastAgentXLocation)
{
    // Have the Simulation:s and AgentLearner:s state at the beginning.
    simulation.moveAgentToStartPosition();
    agentLearner.receiveStartingState(simulation.getState());
    lastAgentXLocation = 0.0;
}

void introduceVariationInQVars(double &discountFactor,
    double &learningRate, double &explorationFactor)
{
    // Construct a trivial random generator engine from a time-based seed:
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::default_random_engine generator(seed);
    std::uniform_int_distribution<int> distribution(0, 100);

    // Semi random floats in range [-0.1; 0.1]
    float random_float0 = ( float(distribution(generator)) - 50.0 ) / 500.0;
    float random_float1 = ( float(distribution(generator)) - 50.0 ) / 500.0;
    float random_float2 = ( float(distribution(generator)) - 50.0 ) / 500.0;

    std::stringstream ss;
    ss << "random_floats: " << random_float0 << "; "
        << random_float1 << "; " << random_float2;
    debug(ss.str());

    // Change the variables, but ceep them in the range [0,1].
    discountFactor += random_float0;
    if (discountFactor < 0) {
        discountFactor = 0.0;
    } else if (discountFactor > 1) {
        discountFactor = 1.0;
    }

    learningRate += random_float1;
    if (learningRate < 0) {
        learningRate = 0.0;
    } else if (learningRate > 1) {
        learningRate = 1.0;
    }

    explorationFactor += random_float2;
    if (explorationFactor < 0) {
        explorationFactor = 0.0;
    } else if (explorationFactor > 1) {
        explorationFactor = 1.0;
    }
}

void agentTask(unsigned agentID,
    std::vector<Actor> actors, std::vector<Sensor> sensors,
    AgentShape agentShape, std::string qtableFilename,
    bool drawGraphics, unsigned maxLoopCount, bool canSaveQtable)
{
    // This is in the Box2D units in the Simulation.
    // Used for the goal in evolution.
    SensorInput agentXAxisLocationOfGoal = 30000;
    // Used for the last position an Agent can be.
    // The worlds floor is 40 000 units long in both directions: [-40k; 40k].
    SensorInput agentXAxisLastPosition = 35000;

    // Create AgentLearner and Simulation that makes the Agent.
    AgentLearner agentLearner(actors, sensors, qtableFilename);
    Simulation simulation(agentID, actors, sensors, agentShape, drawGraphics);

    // Set Q-Learning function variables in AgentLearner when there are
    // more than one Agent. Changes all Agents but the first.
    double discountFactor = agentLearner.getDiscountFactor();
    double learningRate = agentLearner.getLearningRate();
    double explorationFactor = agentLearner.getExplorationFactor();
    if (agentID != 0) {
        introduceVariationInQVars(discountFactor, learningRate,
            explorationFactor);
        agentLearner.setDiscountFactor(discountFactor);
        agentLearner.setLearningRate(learningRate);
        agentLearner.setExplorationFactor(explorationFactor);
    }
    std::stringstream ss;
    ss << "Agent_" << agentID << " QVars: d=" << discountFactor << "; l="
        << learningRate << "; e=" << explorationFactor;
    log(ss.str());

    // Have the Simulation:s and AgentLearner:s state at the beginning be
    // the same.
    State state = simulation.getState();
    agentLearner.receiveStartingState(state);

    // Initiate variables.
    Action action;
    unsigned count = 0;
    double currentXLocation;
    float currentVelosity;
    // Used for showing the Agents learning amount. If the speed is constant
    // and large, then the Agent has learnt an optimal "strategy".
    double lastAgentXLocation = 0.0;
    double timeDeltaBetweenSpeadReading = 5; // thousand counts
    int loop_count_between_output = pow(10, 3) * timeDeltaBetweenSpeadReading;

    while (true) {
        // The learning and simulation parts communicate.
        try {
            action = agentLearner.doAction();
            state = simulation.simulateAction(action);
            agentLearner.receiveSimulationResponse(state);

        } catch (std::exception& e) {
            endThreads = true;
            std::cerr << "exeption when learning functions called from agentTask: "
                        << e.what() << '\n';
        }

        currentXLocation = agentLearner.getXAxisLocation();

        try {

            // EVOLUTION RELATED CODE START!

            /*
            Evolution of the Agents, when multiple Agents are learning:

            The fittest AgentLearner saves its Qtable to the filename:
                evolutionFittestQtableFilename
            This filename is reserved for the evulution.
            An error is thrown if the mentioned file already exists.

            The fittest Qtable file is then read by all the other AgentLearners,
            thus updating all the AgentLearners Q-tables to the fittest Q-table.
            The reading of the file is controlled with the mutex:
                evolutionFittestFile_mutex
            When the Q-tables have been set, the Agents bodyes will also move to the
            starting position.

            Evolution of fittest Agent.
            If an Agent has reached the goal its the fittest of the Agents
            that are running, and will "teach" the other Agents. The "teaching"
            is done by having the other Agents copy the fittest Agents Qtable into
            their Qtable. The modified Agents form the next generation.
            */
            if (// There is more than one Agent learning.
                useEvolution
                // No other Agent has reached the goal.
                && !anAgentHasReachedTheGoal
                // This Agent has reached the goal.
                && currentXLocation > agentXAxisLocationOfGoal)
            {
                log(count, " Agent_", agentID,
                    " has reached the goal, the agents will now evolve");

                // This thread contains the fittest Agent this generation.
                anAgentHasReachedTheGoal = true;

                // Save the fittest Q-table to file, so the other Agents can copy it.
                evolutionFittestFile_mutex.lock();
                // Remove the reserved file if it exist. It should not.
                if (std::ifstream(evolutionFittestQtableFilename)) {
                    debug(agentID, " Evolution fittest: Remove existing qtable");
                    std::remove(evolutionFittestQtableFilename.c_str());
                    if (std::ifstream(evolutionFittestQtableFilename)) {
                        // End execution and throw error, when the file is not deleted.
                        endThreads = true;
                        throw std::runtime_error("File '"
                            + evolutionFittestQtableFilename + "' not removed.");
                    }
                }
                debug(agentID, " Evolution fittest: Save qtable");
                agentLearner.saveQtable(evolutionFittestQtableFilename);
                evolutionFittestFile_mutex.unlock();

                // Wait untill the other Agents have copied the fittest Q-table.
                while (agentsWaitingToEvolve_counter != 0) {
                    std::this_thread::sleep_for (std::chrono::milliseconds(10));
                }

                // Remove the fittest Q-table file.
                debug(agentID, " Evolution fittest: Remove qtable");
                evolutionFittestFile_mutex.lock();
                std::remove(evolutionFittestQtableFilename.c_str());
                if (std::ifstream(evolutionFittestQtableFilename)) {
                    // End execution and throw error, when the file is not deleted.
                    endThreads = true;
                    throw std::runtime_error("File '"
                        + evolutionFittestQtableFilename + "' not removed.");
                }
                evolutionFittestFile_mutex.unlock();

                moveAgentToBeginning(simulation, agentLearner, lastAgentXLocation);

                // Resume the normal running of the learning.
                anAgentHasReachedTheGoal = false;
                pauseThreads = false;
            }

            /*
            Evolution of Agents other than the fittest Agent.
            When the fittest Agent has reached the goal, the other agent will
            wait until they can read the saved Q-table.
            */
            if (anAgentHasReachedTheGoal)
            {
                // Add this Agent to the Agents that are waiting on their turn to
                // update its Q-table, by incrementing this counter:
                agentsWaitingToEvolve_counter++;

                // Let the fittest Agent take controll of evolutionFittestFile_mutex.
                // TODO: MABY NOT NEEDED...
                std::this_thread::sleep_for (std::chrono::milliseconds(100));

                // Set the Q-table for all the Agents to the fittest agents Q-table.
                debug(agentID, " Evolution other: Load qtable");
                evolutionFittestFile_mutex.lock();
                agentLearner.loadQtable(evolutionFittestQtableFilename);
                evolutionFittestFile_mutex.unlock();

                moveAgentToBeginning(simulation, agentLearner, lastAgentXLocation);

                // This thread, and the threads not containing the fittest Agent,
                // will now pause until all the Agents are done updating
                // their Q-tables. The paused thread is executing in the loop
                // here under, in the while(pauseThreads)-loop.
                pauseThreads = true;

                // Remove this Agent from the Agents that are waiting on their turn
                // to update their Q-table, by decreasing this counter:
                agentsWaitingToEvolve_counter--;
            }

            // EVOLUTION RELATED CODE END!

        } catch (std::exception& e) {
            endThreads = true;
            std::cerr << "exeption in Agent evolution: " << e.what() << '\n';

        }

        //Move Agent to beginning if the Agent has reached the end of the
        //worlds floor [-40k; 40k].
        if (agentXAxisLastPosition < currentXLocation
            || currentXLocation < -agentXAxisLastPosition)
        {
            debug("Move Agent_", std::to_string(agentID)," to begginning!");
            moveAgentToBeginning(simulation, agentLearner, lastAgentXLocation);
        }

        count++;

        // Used in testing where the learning is ending fast.
        if (maxLoopCount != 0 && count > maxLoopCount)
            break;

        // Output info about Agent to commandline.
        if (count % loop_count_between_output == 0) {
            // v = d / t; time is X * 1000 count-units
            currentVelosity = ( currentXLocation - lastAgentXLocation )
                / timeDeltaBetweenSpeadReading;
            lastAgentXLocation = currentXLocation;
            // Print the current currentVelosity and location for the Agent.
            // Used instead of graphics.
            cout_mutex.lock();
            std::cout << int(count / 1000) << "k: Agent_" << agentID
                << " V=" << int(currentVelosity)
                << " X=" << currentXLocation
                << std::endl;
            cout_mutex.unlock();
        }

        /*
        Listen to commands from the main thread.
        Actions that can be requested:
            pause self; resume self; end self; save Qtable
        */
        while (pauseThreads) {
            std::this_thread::sleep_for (std::chrono::milliseconds(500));
            if (canSaveQtable && saveQtableInThread) {
                savedQtableFilename_mutex.lock();
                savedQtableFilename = agentLearner.saveQtable();
                savedQtableFilename_mutex.unlock();
                saveQtableInThread = false;
            }
        }
        if (endThreads)
            break;
    }
}

AgentManager::AgentManager(std::vector<Actor>& actors,
    std::vector<Sensor>& sensors, AgentShape& agentShape,
    unsigned int agentCount, std::string const& qtableFilename,
    bool drawGraphics)
    : actors(actors), sensors(sensors), agentShape(agentShape),
    agentCount(agentCount), qtableFilename(qtableFilename),
    drawGraphics(drawGraphics) {}

void AgentManager::pause_threads() {
    pauseThreads = true;
    log("Paused");
}

void AgentManager::resume_threads() {
    pauseThreads = false;
    log("Resumed");
}

// Helper function for AgentManager::join_threads().
void do_join(std::thread& t) { t.join(); }

void AgentManager::join_threads() {
    try {
        std::for_each(agentThreads.begin(), agentThreads.end(), do_join);

        // Remove all the joined threads.
        agentThreads.clear();
    } catch (std::system_error& e) {
        std::cout << "Error during joining of threads: " << e.what()
            << std::endl;
    }
}

void AgentManager::stop_threads() {
    pauseThreads = false;
    endThreads = true;
    join_threads();
    log("Stopped");
}

void AgentManager::createAndStartThreads() {
    unsigned maxLoopCount = 0;
    bool canSaveQtable;

    // Create threads in vector agentThreads.
    for ( unsigned agentID = 0; agentID != agentCount; agentID++) {
        // Only one thread can have its Qtable saved; the first one.
        if (agentID == 0) {
            canSaveQtable = true;
        } else {
            canSaveQtable = false;
        }
        agentThreads.emplace_back(agentTask, agentID, actors, sensors,
            agentShape, qtableFilename, drawGraphics,
            maxLoopCount, canSaveQtable);
    }

    // Evolution is used with generating new generations of Agents
    // if there is more than one Agent thread.
    if (agentCount == 1)
        useEvolution = false;
}

void AgentManager::initRun() {
    // Print more information to cout.
    useLogging = true;
    //useDebugging = true;

    createAndStartThreads();

    // Give instructions how the program is used from the command line
    cout_mutex.lock();
    std::cout   << " " << agentCount << " Agents learning\n"
                << "--------------------------------------\n"
                << "Do things by typing a command and pressing enter.\n"
                << "Unrecognized commands will be ignored.\n"
                << "p: pause; s: save Qtable; r: resume; q: stop\n"
                << "---------------------------------------------"
                << std::endl;
    cout_mutex.unlock();

    // Take user input from commandline.
    std::string userInput;
    char command;
    while (command != 'q') {
        // If an exeption is thrown in an Agent thread, then all threads
        // has to be terminated, when the program exits cleanly.
        if (endThreads) {
            stop_threads();
            break;
        }

        // The first character is only considered as the command from the
        // users input.
        std::cin >> userInput;
        command = userInput[0];
        switch (command)
        {
            case 'p':
                pause_threads();
                break;
            case 's':
                saveQtable();
                break;
            case 'r':
                resume_threads();
                break;
            case 'q':
                stop_threads();
                break;
        }
    }
}

std::string AgentManager::saveQtable() {
    // Saving is done when all the threads are paused.
    pause_threads();

    // Check that threre is some Agent threads.
    if (agentThreads.empty())
        throw std::runtime_error("There exists no Agent threads.");

    // Save the first agents Qtable.
    saveQtableInThread = true;

    // Wait untill the Qtable has been saved.
    while (saveQtableInThread) {
        std::this_thread::sleep_for (std::chrono::milliseconds(100));
    }

    log("Paused");

    savedQtableFilename_mutex.lock();
    std::string savedFilename = savedQtableFilename;
    savedQtableFilename_mutex.unlock();
    return savedFilename;
}
