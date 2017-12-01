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

# Add 3rd party library include directory
#INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/include/3rdpartylibs/libname/include)
#include_directories(lib/gtest-1.7.0/include)

# Add subdirectories (in order of dependencies)
# As told before, these directories should have their own CMakeLists.txt files!
#ADD_SUBDIRECTORY(src)
#ADD_SUBDIRECTORY(test)
#ADD_SUBDIRECTORY(lib/gtest-1.7.0)

# Locate GTest
#find_package(GTest REQUIRED)
#include_directories(${GTEST_INCLUDE_DIRS})

# Link runTests with what we want to test and the GTest and pthread library
ADD_EXECUTABLE(main src/main.cpp)
#target_link_libraries(main ${GTEST_LIBRARIES} pthread)


add_subdirectory(lib/gtest-1.7.0)
include_directories(lib/gtest-1.7.0/include)

ADD_EXECUTABLE(tests test/test_source.cpp)
target_link_libraries(tests gtest pthread)




# KAKI
# Enable threads
#find_package (Threads REQUIRED)

#add_executable (main src/main.cpp)

#target_link_libraries (main ${CMAKE_THREAD_LIBS_INIT})

# Add google test
#add_subdirectory(lib/gtest-1.7.0)
#include_directories(lib/gtest-1.7.0/include)

# Add tests
#add_custom_target(tests)
#add_executable(test_executable test/test_source.cpp)
#add_dependencies(tests test_executable)
