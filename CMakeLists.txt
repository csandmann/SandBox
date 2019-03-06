cmake_minimum_required(VERSION 3.0)
project(SandBox)
set(SandBox_VERSION_MAJOR 0)
set(SandBox_VERSION_MINOR 0)
set(SandBox_VERSION_PATCH 0)

#necessary since <experimental/filesystem> is used
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#add files and target sandbox
file(GLOB_RECURSE sources src/*.cpp src/*.h Resources/*.cpp Resources/*.h)
add_executable(SandBox ${sources})

#Ubuntu 18.04 has a strange place for cmake files for cpprest
if (UNIX)
	find_program(LSB_RELEASE_EXEC lsb_release)
	execute_process(COMMAND ${LSB_RELEASE_EXEC} -ds OUTPUT_VARIABLE DISTR_NAME OUTPUT_STRIP_TRAILING_WHITESPACE)
	if (DISTR_NAME MATCHES "^Ubuntu 18.04")
		set(cpprestsdk_DIR /usr/lib/${CMAKE_LIBRARY_ARCHITECTURE}/cmake)
	endif()
endif(UNIX)

#check if dependencies are available
set(Boost_USE_STATIC_LIBS ON)
find_package(cpprestsdk REQUIRED)
find_package(Boost COMPONENTS system REQUIRED)
find_package(Threads REQUIRED)
find_package(OpenSSL REQUIRED)
find_package(ZLIB REQUIRED)

#add cpprest
find_library(CPPREST_LIBRARY NAMES cpprest)
find_path(CPPREST_INCLUDE_DIR "cpprest/http_client.h")

#compilation and linking linking
target_compile_options(SandBox PUBLIC -std=c++14 -Wall -O3)
include_directories(${Boost_INCLUDE_DIRS} ${CPPREST_INCLUDE_DIR})
if (UNIX)
	target_link_libraries(SandBox PUBLIC ${ZLIB_LIBRARIES} ${CPPREST_LIBRARY} ${Boost_SYSTEM_LIBRARY} ${CMAKE_THREAD_LIBS_INIT} ${OPENSSL_LIBRARIES} stdc++fs)
else()
	#not unix? no libstdc++fs necessary
	target_link_libraries(SandBox PUBLIC ${ZLIB_LIBRARIES} ${CPPREST_LIBRARY} ${Boost_SYSTEM_LIBRARY} ${CMAKE_THREAD_LIBS_INIT} ${OPENSSL_LIBRARIES})
endif()

