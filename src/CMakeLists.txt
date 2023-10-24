# Required for Apple Silicon support.
cmake_minimum_required(VERSION 3.19)

project(
        LaunchDarklyHelloCPPClient
        VERSION 0.1
        DESCRIPTION "LaunchDarkly Hello CPP Client"
        LANGUAGES CXX
)

add_compile_options(-Werror)

#add_executable(hello-cpp main.cpp)
add_executable(hello-cpp
    main.cpp
    remote_client.cpp
)
target_link_libraries(hello-cpp PRIVATE launchdarkly::client)

target_include_directories(hello-cpp PRIVATE
    ${CMAKE_SOURCE_DIR}/include
)
