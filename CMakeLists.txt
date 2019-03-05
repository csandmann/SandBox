cmake_minimum_required(VERSION 3.0)
project(SandBox)
set(SandBox_VERSION_MAJOR 0)
set(SandBox_VERSION_MINOR 1)
set(SandBox_VERSION_PATCH 0)

#necessary linking with stdc++fs
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#add files and target sandbox
file(GLOB_RECURSE sources src/*.cpp src/*.h Resources/*.cpp Resources/*.h)
add_executable(SandBox ${sources})

#find dependencies
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")
set(Boost_USE_STATIC_LIBS ON)
find_package(Boost COMPONENTS system REQUIRED)
find_package(cpprest REQUIRED)
find_package(Threads REQUIRED)
find_package(OpenSSL REQUIRED)

#compilation and linking linking
target_compile_options(SandBox PUBLIC -std=c++11 -Wall)
include_directories(${Boost_INCLUDE_DIRS} ${CPPREST_INCLUDE_DIRS})
target_link_libraries(SandBox PUBLIC ${CPPREST_LIBRARIES} ${Boost_SYSTEM_LIBRARY} ${CMAKE_THREAD_LIBS_INIT} ${OPENSSL_LIBRARIES} stdc++fs)


