# Check that the build does not happen in the source tree
IF(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
   MESSAGE(FATAL_ERROR "Prevented in-tree built. Try again using CMake in the build folder.")
ENDIF(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})

# Project wide settings
CMAKE_MINIMUM_REQUIRED(VERSION 3.7.2)
PROJECT(q-learning-9)

# Set compiler and linker flags
add_compile_options(-c -g -std=c++0x -Wall -Wextra -pedantic -pthread)

# TODO: Add Doxygen
# Build Doxygen documentation
#ADD_CUSTOM_TARGET(build_docs
#    COMMAND cd .. && doxygen doxygen/doxygen.conf
#    COMMENT "Building Doxygen documentation")

# Create SOURCES variable that contain the used source files
set(SOURCES
    src/interactor.hpp src/interactor.cpp
    src/q-table.hpp src/q-table.cpp
    src/agent_learner.hpp src/agent_learner.cpp
    src/agent_manager.hpp src/agent_manager.cpp
    src/config_reader.hpp src/config_reader.cpp)

# Add main
ADD_EXECUTABLE(main src/main.cpp ${SOURCES})
target_link_libraries(main pthread)

# Add tests that use googletest
enable_testing()
add_subdirectory(lib/gtest-1.7.0)
include_directories(lib/gtest-1.7.0/include)
ADD_EXECUTABLE(qtests
    test/test_interactor.cpp
    test/test_config_reader.cpp
    test/test_agent_manager.cpp
    test/test_qtable.cpp
    test/test_agent_learner.cpp
    ${SOURCES})
target_link_libraries(qtests gtest gtest_main pthread)
add_test( qtests qtests )

# Add Simulation and Box2D
add_subdirectory(src/Box2D_v2.3.0/Box2D)
