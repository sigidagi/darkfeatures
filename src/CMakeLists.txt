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

# ----- Using build LD from source

target_link_libraries(hello-cpp PRIVATE launchdarkly::client)
target_include_directories(hello-cpp PRIVATE
    ${CMAKE_SOURCE_DIR}/include
)

# ---- OR using pre-compiled static library

#message(STATUS "LaunchDarkly: searching for static OpenSSL library")
#set(OPENSSL_USE_STATIC_LIBS ON)

#find_package(OpenSSL REQUIRED)
#message(STATUS "LaunchDarkly: using OpenSSL v${OPENSSL_VERSION}")

## Even though the main SDK might be a static or shared lib, boost should always statically
## linked into the binary.
#set(Boost_USE_STATIC_LIBS ON)
#set(Boost_USE_MULTITHREADED ON)
#set(Boost_USE_STATIC_RUNTIME OFF)

#find_package(Boost 1.81 REQUIRED COMPONENTS json url coroutine)
#message(STATUS "LaunchDarkly: using Boost v${Boost_VERSION}")

#target_link_directories(hello-cpp PRIVATE ${CMAKE_SOURCE_DIR}/third_party/ld/lib)

#target_link_libraries(hello-cpp PRIVATE 
    #launchdarkly-cpp-client
    #Boost::headers
    #Boost::url
    #Boost::json
    #OpenSSL::SSL
    #Boost::disable_autolinking
#)

#target_include_directories(hello-cpp PRIVATE
    #${CMAKE_SOURCE_DIR}/include
    #${CMAKE_SOURCE_DIR}/third_party/ld/include
#)
