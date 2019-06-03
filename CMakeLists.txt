cmake_minimum_required(VERSION 3.0)
project(SandBox)
set(SandBox_VERSION_MAJOR 1)
set(SandBox_VERSION_MINOR 0)
set(SandBox_VERSION_PATCH 0)

#cpp standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#add files and target sandbox
if (DEVELOPMENT)
	file(GLOB_RECURSE sources src/*.cpp src/*.h Resources/*.cpp Resources/*.h)
	list(REMOVE_ITEM sources "${CMAKE_SOURCE_DIR}/src/ReaderPlugins/ReaderMFRC522.cpp")
else (DEVELOPMENT)
	file(GLOB_RECURSE sources src/*.cpp src/*.h Resources/*.cpp Resources/*.h 3rdParty/*.cpp 3rdParty/*.h)
	list(REMOVE_ITEM sources "${CMAKE_SOURCE_DIR}/3rdParty/RPi-RFID/Read.cpp")
endif(DEVELOPMENT)
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

if(DEVELOPMENT)
	message("BUILDING DEVELOPMENT")
	add_definitions(-DDEVELOPMENT)
	find_library(OPENSSL_LIBRARY libssl.so.1.0.2)
	find_library(CRYPTO_LIBRARY libcrypto.so.1.0.2)
	target_link_libraries(SandBox PRIVATE ${ZLIB_LIBRARIES} cpprest ${Boost_SYSTEM_LIBRARY} ${Boost_FILESYSTEM_LIBRARY} ${CMAKE_THREAD_LIBS_INIT} ${OPENSSL_LIBRARY} ${CRYPTO_LIBRARY} Boost::boost)
else(DEVELOPMENT)
	message("BUILDING DEPLOYMENT")
	find_library(OPENSSL_LIBRARY libssl.so.1.0.2)
	find_library(CRYPTO_LIBRARY libcrypto.so.1.0.2)
	find_library(BCM2835_LIBRARY libbcm2835.a)
	target_link_libraries(SandBox PRIVATE ${ZLIB_LIBRARIES} cpprest ${Boost_SYSTEM_LIBRARY} ${Boost_FILESYSTEM_LIBRARY} ${CMAKE_THREAD_LIBS_INIT} ${OPENSSL_LIBRARY} ${CRYPTO_LIBRARY} Boost::boost ${BCM2835_LIBRARY})
endif(DEVELOPMENT)


#CPACK
install(DIRECTORY Resources DESTINATION /opt/sandbox)
install(FILES Resources/config.ini DESTINATION /opt/sandbox)
install(FILES Resources/librespot.service DESTINATION /etc/systemd/system)
install(TARGETS SandBox DESTINATION /opt/sandbox)

IF(EXISTS "${CMAKE_ROOT}/Modules/CPack.cmake")
INCLUDE(InstallRequiredSystemLibraries)

SET(CPACK_SET_DESTDIR "on")
SET(CPACK_GENERATOR "DEB")

SET(CPACK_PACKAGE_DESCRIPTION "RFID Audio Player for the RaspberryPi")
SET(CPACK_PACKAGE_DESCRIPTION_SUMMARY "This RFID-Controlled Audio Player can do more than you think")
SET(CPACK_PACKAGE_CONTACT "sandbox@cksandmann.de")
SET(CPACK_PACKAGE_HOMEPAGE "https://github.com/check0104/SandBox")
SET(CPACK_PACKAGE_VERSION_MAJOR "${SandBox_VERSION_MAJOR}")
SET(CPACK_PACKAGE_VERSION_MINOR "${SandBox_VERSION_MINOR}")
SET(CPACK_PACKAGE_VERSION_PATCH "${SandBox_VERSION_PATCH}")
SET(CPACK_PACKAGE_FILE_NAME "${CMAKE_PROJECT_NAME}_${SandBox_VERSION_MAJOR}.${SandBox_VERSION_MINOR}.${SandBox_VERSION_PATCH}")
SET(CPACK_SOURCE_PACKAGE_FILE_NAME "${CMAKE_PROJECT_NAME}_${SandBox_VERSION_MAJOR}.${SandBox_VERSION_MINOR}.${SandBox_VERSION_PATCH}")
SET(CPACK_DEBIAN_PACKAGE_DEPENDS "libssl1.1")

SET(CPACK_DEBIAN_PACKAGE_PRIORITY "optional")
SET(CPACK_DEBIAN_PACKAGE_SECTION "Sound")
SET(CPACK_DEBIAN_ARCHITECTURE ${CMAKE_SYSTEM_PROCESSOR})
INCLUDE(CPack)

ENDIF(EXISTS "${CMAKE_ROOT}/Modules/CPack.cmake")
