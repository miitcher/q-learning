# Check that the build does not happen in the source tree
IF(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
   MESSAGE(FATAL_ERROR "Prevented in-tree built. Try again using CMake in the build folder.")
ENDIF(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})

# Project wide settings
CMAKE_MINIMUM_REQUIRED(VERSION 3.5)
PROJECT(q-learning-9)

# Module path added separately for Cairo, because it is not in cmake.
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

# Set compiler and linker flags
# Flags after pthread are needed for Cairo.
add_compile_options(-c -g -std=c++0x -Wall -Wextra -pedantic -pthread
    -I /usr/include/cairo/ -L /usr/lib -lcairo )

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
    src/config_reader.hpp src/config_reader.cpp
    src/simulation.hpp src/simulation.cpp)

# Add Box2D and Cairo
add_subdirectory(lib/Box2D_v2.3.0)
include_directories(
    ${Box2D_SOURCE_DIR}
    ${Cairo_INCLUDE_DIRS}
)
find_package(Cairo)

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
    cairo
    ${Cairo_LIBRARIES}
)

# Add tests
enable_testing()
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
target_link_libraries(qtests
    gtest gtest_main pthread
    Box2D
)
add_test( qtests qtests )
