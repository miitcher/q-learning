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
    src/config_reader.hpp src/config_reader.cpp
    src/simulation.hpp src/simulation.cpp)

# Define the Simulation source files.
set(Testbed_SOURCES
    src/Testbed/Framework/TestbedMain.cpp
    src/Testbed/Framework/Render.cpp
    src/Testbed/Framework/Render.h
    src/Testbed/Framework/Test.cpp
    src/Testbed/Framework/Test.h
    src/Testbed/Tests/TestEntries.cpp
)

# The Box2D library.
add_subdirectory(lib/Box2D_v2.3.0)

include_directories(
    ${OPENGL_INCLUDE_DIR}
    ${Box2D_SOURCE_DIR}
)

# Add dependencies (for graphics)
find_package(OpenGL REQUIRED)
add_subdirectory(lib/Box2D_v2.3.0/freeglut)
add_subdirectory(lib/Box2D_v2.3.0/glui)

# Some flags for Freeglut and GLUI.
add_definitions( -DFREEGLUT_EXPORTS -DFREEGLUT_STATIC -D_CRT_SECURE_NO_WARNINGS )

# Add main
include_directories(src/Testbed)
ADD_EXECUTABLE(
    main src/main.cpp
    ${SOURCES}
    ${Testbed_SOURCES})
target_link_libraries(
    main
    pthread
    Box2D
    freeglut_static
    glui
    ${ADDITIONAL_LIBRARIES}
    ${OPENGL_LIBRARIES}
    X11)

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
    ${SOURCES}
    ${Testbed_SOURCES})
target_link_libraries(qtests
    gtest gtest_main pthread
    Box2D
    freeglut_static
    glui
    ${ADDITIONAL_LIBRARIES}
    ${OPENGL_LIBRARIES}
    X11)
add_test( qtests qtests )


# Testbed is used in the main.cpp for graphics.
# make will not compile both main and Testbed separately in their current forms.
if(FALSE) # fake a block comment

## Add Testbed
add_executable(Testbed
    ${Testbed_SOURCES}
)

target_link_libraries (
    Testbed
    Box2D
    freeglut_static
    glui
    ${ADDITIONAL_LIBRARIES}
    ${OPENGL_LIBRARIES}
    X11
)

endif() # fake a block comment
