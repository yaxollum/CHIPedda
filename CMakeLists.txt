# Set the minimum version of CMake that can be used
# To find the cmake version run
# $ cmake --version
cmake_minimum_required(VERSION 3.5)

set(CMAKE_CXX_FLAGS "-Wall -Werror -g")

# Set the project name
project (chipedda)

# Create a sources variable with a link to all cpp files to compile
#set(SOURCES
#    src/Hello.cpp
#    src/main.cpp
#)

file(GLOB_RECURSE SOURCES src/*.cc)

# Add an executable with the above sources
add_executable(chipedda ${SOURCES})

# Set the directories that should be included in the build command for this target
# when running g++ these will be included as -I/directory/path/

target_include_directories(chipedda PRIVATE src)

install(TARGETS chipedda DESTINATION bin)
