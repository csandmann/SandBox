cmake_minimum_required(VERSION 3.0)
project(SandBox)
set(SandBox_VERSION_MAJOR 0)
set(SandBox_VERSION_MINOR 0)
set(SandBox_VERSION_PATCH 0)

#cpp standard
set(CMAKE_CXX_STANDARD 11)
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
find_package(Boost COMPONENTS system filesystem REQUIRED)
find_package(Threads REQUIRED)
find_package(OpenSSL REQUIRED)
find_package(ZLIB REQUIRED)

#add cpprest
find_library(CPPREST_LIBRARY NAMES cpprest cpprest_2_10)
find_path(CPPREST_INCLUDE_DIR "cpprest/http_client.h")

#compilation and linking linking
if(MSVC)
  target_compile_options(SandBox PRIVATE /W4)
elseif(UNIX)
  target_compile_options(SandBox PRIVATE -Wall -std=c++11 -O3)
endif(MSVC)
target_include_directories(SandBox PRIVATE ${Boost_INCLUDE_DIRS} ${CPPREST_INCLUDE_DIR})
target_link_libraries(SandBox PRIVATE ${ZLIB_LIBRARIES} ${CPPREST_LIBRARY} ${Boost_SYSTEM_LIBRARY} ${Boost_FILESYSTEM_LIBRARY} ${CMAKE_THREAD_LIBS_INIT} ${OPENSSL_LIBRARIES} Boost::boost)

