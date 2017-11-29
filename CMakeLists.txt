cmake_minimum_required (VERSION 3.7)
project (q-learning-9)
add_compile_options(-c -g -std=c++0x -Wall -Wextra -pedantic -pthread)
find_package (Threads REQUIRED)
add_executable (main src/main.cpp)
target_link_libraries (main ${CMAKE_THREAD_LIBS_INIT})




# Unittests with google test (not working atm)

#enable_testing()

#include_directories("$ENV{HOME}/gtest-1.7.0")
#find_package(GTest REQUIRED)
#add_executable(foo test/test_source.cpp)
#target_link_libraries(foo GTest::GTest GTest::Main)
#add_test(AllTestsInFoo foo)


#add_custom_target(tests)
#add_executable(test_executable test/test_source.cpp)
#add_dependencies(tests test_executable)
