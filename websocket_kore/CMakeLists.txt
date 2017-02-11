cmake_minimum_required(VERSION 2.8.7)

set(PROJECT_NAME "kore_websocket")

project(${PROJECT_NAME} C CXX)

option(DEBUG_MODE "Debug or release?" ON)

add_definitions( -DCONFIG_PATH="./conf/conf.conf" )

if(DEBUG_MODE)
  set(CMAKE_BUILD_TYPE DEBUG)
else(DEBUG_MODE)
  set(CMAKE_BUILD_TYPE RELEASE)
endif(DEBUG_MODE)

# ---
# add include files
include_directories(${CMAKE_SOURCE_DIR})
include_directories(${CMAKE_SOURCE_DIR}/src)
include_directories(${CMAKE_SOURCE_DIR}/include)

# ---
# find required packages
include(FindPkgConfig)

pkg_check_modules(CRYPTO REQUIRED libcrypto)
include_directories(${OPENSSL_INCLUDE_DIRS})

pkg_check_modules(OPENSSL REQUIRED openssl)
include_directories(${OPENSSL_INCLUDE_DIRS})

pkg_check_modules(KORE REQUIRED kore)
include_directories(${KORE_INCLUDE_DIRS})


MESSAGE( "CMAKE_SOURCE_DIR: " ${CMAKE_SOURCE_DIR})

# create lib
set(SO_NAME ${PROJECT_NAME}_sample)

file(GLOB SOURCES_WEB ${CMAKE_SOURCE_DIR}/.objs/*.c)
file(GLOB SOURCES_LIB ${CMAKE_SOURCE_DIR}/src/*.c)
set(SRC_LIST_LIB
    ${SOURCES_WEB}
    ${SOURCES_LIB}
)

SET (LIBRARY_OUTPUT_PATH "${CMAKE_SOURCE_DIR}/pkg_$ENV{ARCH}/")

ADD_LIBRARY(${SO_NAME} SHARED ${SRC_LIST_LIB})

# ---
# create executable file
set(BIN_NAME ${PROJECT_NAME})

file(GLOB SOURCES_MAIN ${CMAKE_SOURCE_DIR}/src/main.c)
set(SRC_LIST
    ${SOURCES_MAIN}
)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/pkg_$ENV{ARCH}/")
add_executable(${BIN_NAME} ${SRC_LIST})
set_target_properties(${BIN_NAME} PROPERTIES LINKER_LANGUAGE C)

target_link_libraries (${BIN_NAME}
        ${CRYPTO_LDFLAGS}
        ${OPENSSL_LDFLAGS}
        ${KORE_LDFLAGS}
)

# copy resource files to output folder
if(EXISTS "${CMAKE_SOURCE_DIR}/res")
    file(COPY "${CMAKE_SOURCE_DIR}/res" DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
    MESSAGE( "files in 'res' have been copied to output " ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
endif()

if(EXISTS "${CMAKE_SOURCE_DIR}/cert")
    file(COPY "${CMAKE_SOURCE_DIR}/cert" DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
    MESSAGE( "files in 'cert' have been copied to output " ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
endif()

if(EXISTS "${CMAKE_SOURCE_DIR}/conf")
    file(COPY "${CMAKE_SOURCE_DIR}/conf" DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
    MESSAGE( "files in 'conf' have been copied to output " ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
endif()

if(EXISTS "$ENV{CMAKE_PREFIX_PATH}/lib")
    file(MAKE_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/lib)
    file(COPY "$ENV{CMAKE_PREFIX_PATH}/lib/libkore.so.2.0.0" DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/lib)
    file(COPY "$ENV{CMAKE_PREFIX_PATH}/lib/libkore.so.2" DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/lib)
    file(COPY "$ENV{CMAKE_PREFIX_PATH}/lib/libkore.so" DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/lib)
    MESSAGE( $ENV{CMAKE_PREFIX_PATH}/lib "/libkore* have been copied to output " ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
endif()

# copy appinfo.json file to output folder
if(EXISTS "${CMAKE_SOURCE_DIR}/appinfo.json")
    file(COPY "${CMAKE_SOURCE_DIR}/appinfo.json" DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
else()
    MESSAGE( "'appinfo.json' file was not found !!")
endif()

# copy icon.png file to output folder
if(EXISTS "${CMAKE_SOURCE_DIR}/icon.png")
    file(COPY "${CMAKE_SOURCE_DIR}/icon.png" DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
else()
    MESSAGE( "'icon.png' file was not found !!")
endif()

# copy largeIcon.png file to output folder
if(EXISTS "${CMAKE_SOURCE_DIR}/largeIcon.png")
    file(COPY "${CMAKE_SOURCE_DIR}/largeIcon.png" DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
else()
    MESSAGE( "'largeIcon.png' file was not found !!")
endif()

if(EXISTS "${CMAKE_SOURCE_DIR}/dh2048.pem")
    file(COPY "${CMAKE_SOURCE_DIR}/dh2048.pem" DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
else()
    MESSAGE( "'dh2048.pem' file was not found !!")
endif()
