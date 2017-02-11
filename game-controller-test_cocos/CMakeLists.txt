
cmake_policy(SET CMP0017 NEW)
cmake_minimum_required(VERSION 2.8)

set(APP_NAME controller-test)
project (${APP_NAME})
set(COCOS2D_ROOT $ENV{CMAKE_INCLUDE_PATH}/cocos2d)


# set module path
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${COCOS2D_ROOT}/cmake/Modules/")
include(CocosBuildHelpers)

# Some macro definitions
set(PLATFORM_FOLDER linux)



# Compiler options
set(CMAKE_C_FLAGS_DEBUG "-g -Wall -DCOCOS2D_DEBUG=1")
set(CMAKE_CXX_FLAGS_DEBUG ${CMAKE_C_FLAGS_DEBUG})
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wno-deprecated-declarations -Wno-reorder")

set(PLATFORM_SPECIFIC_SRC)
set(PLATFORM_SPECIFIC_HEADERS)

# Use same method as in cocos library
set(PLATFORM_SRC proj.linux/main.cpp)
set(RES_PREFIX "/Resources")

set(TESTS_SRC
  Classes/AppDelegate.cpp
  Classes/GameControllerTest.cpp
  ${PLATFORM_SRC}
)



# add the executable
add_executable(${APP_NAME}
  ${TESTS_SRC}
)


#Set Include directories
include_directories(
  ${COCOS2D_ROOT}
  ${COCOS2D_ROOT}/cocos
  ${COCOS2D_ROOT}/cocos/platform
  ${COCOS2D_ROOT}/cocos/audio/include/
  ${COCOS2D_ROOT}/cocos/editor-support/
  ${COCOS2D_ROOT}/external/
  ${COCOS2D_ROOT}/cocos/platform/linux
  ${COCOS2D_ROOT}/extensions
  ${COCOS2D_ROOT}/cocos/network
  Classes
)


  find_package(PkgConfig)
  pkg_check_modules(GLES2 REQUIRED glesv2)
  include_directories(${GLES2_INCLUDE_DIRS})

  pkg_check_modules(CURL REQUIRED libcurl)
  include_directories(${CURL_INCLUDE_DIRS})

  pkg_check_modules(FONTCONF REQUIRED fontconfig)
  include_directories(${FONTCONF_INCLUDE_DIRS})

  pkg_check_modules(FREETYPE REQUIRED freetype2)
  include_directories(${FREETYPE_INCLUDE_DIRS})

  pkg_check_modules(KORE REQUIRED kore)
  include_directories(${KORE_INCLUDE_DIRS})


  target_link_libraries(${APP_NAME}
    libcocos2d.a
    ${GLES2_LDFLAGS}
    -lSDL2 -lSDL2_mixer -lboost_regex
    -lpng16 -ljpeg
    -lm -lpthread
    -lfontconfig -ltiff
    -lfreetype
    -lz
    libtinyxml2.a
    libxxhash.a
    libwebp.a
    libchipmunk.a
    libbullet.a
    librecast.a
    libunzip.a
    ${CURL_LDFLAGS}
    ${FONTCONF_LDFLAGS}
    ${FREETYPE_LDFLAGS}
    ${KORE_LDFLAGS}
  )




set(APP_BIN_DIR "${CMAKE_BINARY_DIR}/bin")

set_target_properties(${APP_NAME} PROPERTIES
     RUNTIME_OUTPUT_DIRECTORY  "${APP_BIN_DIR}")

pre_build(${APP_NAME}
  COMMAND ${CMAKE_COMMAND} -E remove_directory ${APP_BIN_DIR}/Resources
  COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/Resources ${APP_BIN_DIR}/Resources

  COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/appinfo.json ${APP_BIN_DIR}/
  COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/icon.png ${APP_BIN_DIR}/
  COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/largeIcon.png ${APP_BIN_DIR}/

  COMMAND ${CMAKE_COMMAND} -E make_directory ${APP_BIN_DIR}/lib/
  COMMAND ${CMAKE_COMMAND} -E copy ${KORE_LIBRARY_DIRS}/libkore.so.2.0.0 ${APP_BIN_DIR}/lib/
  COMMAND ${CMAKE_COMMAND} -E chdir ${APP_BIN_DIR}/lib ${CMAKE_COMMAND} -E create_symlink libkore.so.2.0.0 libkore.so.2
  COMMAND ${CMAKE_COMMAND} -E chdir ${APP_BIN_DIR}/lib ${CMAKE_COMMAND} -E create_symlink libkore.so.2.0.0 libkore.so

  COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/../websocket_kore/pkg_arm/cert ${APP_BIN_DIR}/cert
  COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/../websocket_kore/pkg_arm/conf ${APP_BIN_DIR}/conf
  COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/../websocket_kore/pkg_arm/dh2048.pem ${APP_BIN_DIR}/
  COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/../websocket_kore/pkg_arm/libkore_websocket_sample.so ${APP_BIN_DIR}/
)

set(CMAKE_BUILD_WITH_INSTALL_RPATH TRUE)
set(CMAKE_INSTALL_RPATH "./lib")








