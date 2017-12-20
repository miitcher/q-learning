# Check that the build does not happen in the source tree
IF(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
   MESSAGE(FATAL_ERROR "Prevented in-tree built. Try again using CMake in the build folder.")
ENDIF(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})

# Project wide settings
CMAKE_MINIMUM_REQUIRED(VERSION 3.5)
PROJECT(q-learning-9)

# Set compiler and linker flags
add_compile_options(-c -g -std=c++0x -Wall -Wextra -pedantic -pthread)

# Create SOURCES variable that contain the used source files
set(
    SOURCES
    src/interactor.hpp src/interactor.cpp
    src/q-table.hpp src/q-table.cpp
    src/agent_learner.hpp src/agent_learner.cpp
    src/agent_manager.hpp src/agent_manager.cpp
    src/config_reader.hpp src/config_reader.cpp
    src/simulation.hpp src/simulation.cpp
)

# Add Box2D
add_subdirectory(lib/Box2D_v2.3.0)
include_directories(${Box2D_SOURCE_DIR})

# Add main
ADD_EXECUTABLE(
    main
    src/main.cpp
    ${SOURCES}
)
target_link_libraries(
    main
    pthread
    Box2D
)

# Add tests
enable_testing()
# Included as source code, because google test library must be built with
# the same flags as the C++ code under test
add_subdirectory(lib/gtest-1.7.0)
include_directories(lib/gtest-1.7.0/include)
ADD_EXECUTABLE(
    qtests
    test/test_interactor.cpp
    test/test_config_reader.cpp
    test/test_agent_manager.cpp
    test/test_qtable.cpp
    test/test_agent_learner.cpp
    test/test_simulation.cpp
    ${SOURCES}
)
target_link_libraries(
    qtests
    pthread
    Box2D
    gtest
    gtest_main
)
add_test( qtests qtests )
