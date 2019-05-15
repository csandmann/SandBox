cmake_minimum_required(VERSION 3.0)
project(SandBox)
set(SandBox_VERSION_MAJOR 0)
set(SandBox_VERSION_MINOR 0)
set(SandBox_VERSION_PATCH 0)

#cpp standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#add files and target sandbox
if (DRY_RUN)
	file(GLOB_RECURSE sources src/*.cpp src/*.h Resources/*.cpp Resources/*.h)
	list(REMOVE_ITEM sources "${CMAKE_SOURCE_DIR}/src/ReaderPlugins/ReaderMFRC522.cpp")
else (DRY_RUN)
	file(GLOB_RECURSE sources src/*.cpp src/*.h Resources/*.cpp Resources/*.h 3rdParty/*.cpp 3rdParty/*.h)
	list(REMOVE_ITEM sources "${CMAKE_SOURCE_DIR}/3rdParty/RPi-RFID/Read.cpp")
endif(DRY_RUN)
add_executable(SandBox ${sources})

#check if dependencies are available
set(Boost_USE_STATIC_LIBS ON)
find_package(Boost COMPONENTS system filesystem REQUIRED)
find_package(Threads REQUIRED)
find_package(ZLIB REQUIRED)

#add cpprest
find_path(CPPREST_INCLUDE_DIR "cpprest/http_client.h")

#compilation and linking linking
if(MSVC)
  target_compile_options(SandBox PRIVATE /W4)
elseif(UNIX)
  target_compile_options(SandBox PRIVATE -Wall -std=c++11 -O3)
endif(MSVC)
target_include_directories(SandBox PRIVATE ${Boost_INCLUDE_DIRS} ${CPPREST_INCLUDE_DIR})

if(DRY_RUN)
	message("BUILDING WITH DRY_RUN")
	add_definitions(-DDRY_RUN)
	find_package(OpenSSL REQUIRED)
	target_link_libraries(SandBox PRIVATE ${ZLIB_LIBRARIES} cpprest ${Boost_SYSTEM_LIBRARY} ${Boost_FILESYSTEM_LIBRARY} ${CMAKE_THREAD_LIBS_INIT} ${OPENSSL_LIBRARIES} Boost::boost)
else(DRY_RUN)
	message("BUILDING WITH DEFAULT READER")
	find_package(OpenSSL1.0 REQUIRED)
	target_link_libraries(SandBox PRIVATE ${ZLIB_LIBRARIES} cpprest ${Boost_SYSTEM_LIBRARY} ${Boost_FILESYSTEM_LIBRARY} ${CMAKE_THREAD_LIBS_INIT} ${OPENSSL_LIBRARIES} Boost::boost bcm2835)
endif(DRY_RUN)
