# Required for Apple Silicon support.
cmake_minimum_required(VERSION 3.19)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

include(CPM)
# Add targets for formating cpp and cmake files. See README.md
CPMAddPackage("gh:TheLartians/Format.cmake@1.7.3")
# Test framework for replacing qTest
CPMAddPackage("gh:catchorg/Catch2@3.4.0")

# It will generate file `compile_commands.json` which can be used for editors for code completion
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
# Don't use e.g. GNU extension (like -std=gnu++11) for portability
set(CMAKE_CXX_EXTENSIONS OFF)

project(JustATest)

add_subdirectory(third_party/cpp-sdks)
add_subdirectory(src)
