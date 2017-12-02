# Check that the build does not happen in the source tree
IF(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
   MESSAGE(FATAL_ERROR "Prevented in-tree built. This is bad practice.")
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
set(SOURCES src/interactor.hpp src/interactor.cpp src/q-table.hpp src/q-table.cpp src/agent.hpp src/agent.cpp src/agent_manager.hpp src/agent_manager.cpp)

# Add main
ADD_EXECUTABLE(main src/main.cpp ${SOURCES})
target_link_libraries(main pthread)

# Add tests that use googletest
enable_testing()
add_subdirectory(lib/gtest-1.7.0)
include_directories(lib/gtest-1.7.0/include)
ADD_EXECUTABLE(qtests test/test_source.cpp ${SOURCES})
target_link_libraries(qtests gtest gtest_main pthread)
add_test( qtests qtests )
