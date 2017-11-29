#include <iostream>
#include <thread>
#include "agent_manager.hpp"
#include "agent_manager.cpp"

// The main will controll IO and graphics, and start the program.
int main() {
    std::cout << "It's learning!" << std::endl;

    /* Mikael - Code for testing threading and CMake
    printHello();
    std::thread t1(printHello);
    std::cout << "Main thread is executing." << std::endl;
    t1.join();
    std::cout << "Thread joined." << std::endl;
    */
}
