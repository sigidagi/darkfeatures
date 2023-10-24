# This project aims to follow modern cmake guidelines, e.g.
# https://cliutils.gitlab.io/modern-cmake

# Required for Apple Silicon support.
cmake_minimum_required(VERSION 3.19)
include(CMakeDependentOption)

project(
        LaunchDarklyCPPSDKs
        VERSION 0.1
        DESCRIPTION "LaunchDarkly C++ SDK Monorepo (Server/Client)"
        LANGUAGES CXX C
)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

#include(GNUInstallDirs)

option(BUILD_TESTING "Top-level switch for testing. Turn off to disable unit and contract tests." OFF)
option(LD_BUILD_SHARED_LIBS "Build the SDKs as shared libraries" OFF)

add_compile_options(
    -Wall
    -Wextra
    -Wpedantic
    -Wno-unused-parameter
    -Wno-unused-variable
    -Wno-invalid-noreturn
    -Wno-mismatched-tags
    -Wno-reorder-ctor
    -Wno-sign-compare
    -Wno-unused-private-field
    -Wno-logical-op-parentheses
    -Wno-delete-non-abstract-non-virtual-dtor
    -Wno-missing-braces
    -Wno-unused-function
    -Wno-pessimizing-move
)

# All projects in this repo should share the same version of 3rd party depends.
# It's the only way to remain sane.
set(CMAKE_FILES "${CMAKE_SOURCE_DIR}/cmake")
set(CMAKE_CXX_STANDARD 17)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)

message(STATUS "LaunchDarkly: searching for static OpenSSL library")
set(OPENSSL_USE_STATIC_LIBS ON)

find_package(OpenSSL REQUIRED)
message(STATUS "LaunchDarkly: using OpenSSL v${OPENSSL_VERSION}")

# Even though the main SDK might be a static or shared lib, boost should always statically
# linked into the binary.
set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)

find_package(Boost 1.81 REQUIRED COMPONENTS json url coroutine)
message(STATUS "LaunchDarkly: using Boost v${Boost_VERSION}")

include(${CMAKE_FILES}/certify.cmake)

add_subdirectory(vendor/foxy)
## Common, internal, and server-sent-events are built as "object" libraries.
add_subdirectory(libs/common)
add_subdirectory(libs/internal)
add_subdirectory(libs/server-sent-events)
## Built as static or shared depending on LD_BUILD_STATIC_LIBS variable.
## This target "links" in common, internal, and sse as object libraries.
add_subdirectory(libs/client-sdk)
