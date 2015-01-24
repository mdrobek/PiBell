cmake_minimum_required(VERSION 2.8.4)
project(PiBell)

SET(ARM "arm")
SET(X86 "x86")
SET(IS_ARM false)
SET(PLATTFORM "")

# call cmake for different plattform build as follows:
#    cmake -DTARGET=arm|x86 CMakeLists.txt
# By default, always the last set entry is chosen
#message(STATUS "This is the target variable: ${TARGET}")
if("${TARGET}" STREQUAL ${ARM})
    SET(IS_ARM true)
    SET(PLATTFORM ${ARM})
elseif("${TARGET}" STREQUAL ${X86})
    SET(IS_ARM false)
    SET(PLATTFORM ${X86})
else()
    MESSAGE(FATAL_ERROR "No plattform has been set.\nCall as: cmake -DTARGET=arm|x86 CMakeLists.txt")
endif()
message(STATUS "This is the target plattform: ${TARGET}")

########
# Plattform independent stuff
########
# this one is important
SET(CMAKE_SYSTEM_NAME Linux)
#this one not so much
SET(CMAKE_SYSTEM_VERSION 1)
set(PiBell_DIR ${PiBell_SOURCE_DIR})
set(PiBell_INCLUDE_DIR ${PiBell_DIR}/inc)
set(PiBell_SOURCE_DIR ${PiBell_DIR}/src)
set(PiBell_BINARY_DIR ${PiBell_DIR}/bin)

set(EXECUTABLE_OUTPUT_PATH ${PiBell_BINARY_DIR})
set(OUTPUT_NAME ${CMAKE_PROJECT_NAME}-${PLATTFORM})
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${PiBell_DIR}/build/cmake)
set(CMAKE_VERBOSE_MAKEFILE true)

message(STATUS "This is the binary file name: ${OUTPUT_NAME}")

# Define compiler flags
SET(GCC_VALGRIND_COMPILE_FLAGS "-g -O0")
add_definitions(${GCC_VALGRIND_COMPILE_FLAGS})
SET(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} ${GCC_VALGRIND_COMPILE_FLAGS}" )


if(${IS_ARM})
    ########
    # ARM specific definitions
    ########
    # specify the cross compiler
    SET(CMAKE_C_COMPILER /opt/crosstool-ng/x-tools/arm-unknown-linux-gnueabi/bin/arm-unknown-linux-gnueabi-gcc)

    # where is the target environment
    SET(PI_ENV_DIR /opt/crosstool-ng/x-tools/arm-unknown-linux-gnueabi)
    SET(CMAKE_FIND_ROOT_PATH ${PI_ENV_DIR})
    set(CMAKE_LIBRARY_PATH ${CMAKE_LIBRARY_PATH} /usr/lib)
    # search for programs in the build host directories
    SET(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
    # for libraries and headers in the target directories
    SET(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
    SET(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)

    SET(PI_LIB_DIR ${PI_ENV_DIR}/lib/arm-linux-gnueabihf/)
    link_directories(${PI_LIB_DIR})
else()
    ########
    # x86 specific definitions
    ########
    set(CMAKE_C_COMPILER "/usr/bin/clang-3.5")
    # set(CMAKE_LIBRARY_PATH ${CMAKE_LIBRARY_PATH} /usr/lib/x86_64-linux-gnu) 
endif()
    
include(FindLibCurl-${PLATTFORM} REQUIRED)
add_definitions( -DCMAKE_PLATTFORM="${PLATTFORM}" )
message(STATUS "%%% environment: ${LIBCURL_LIBRARY} | ${LIBCURL_INCLUDE_DIR}")

include_directories(${LIBCURL_INCLUDE_DIR} ${PiBell_INCLUDE_DIR} )

set(SOURCE_FILES ${PiBell_SOURCE_DIR}/main.c
    ${PiBell_SOURCE_DIR}/ctrler.c
    ${PiBell_SOURCE_DIR}/rest.c
    ${PiBell_SOURCE_DIR}/cJSON.c)
add_executable(${OUTPUT_NAME} ${SOURCE_FILES})
target_link_libraries(${OUTPUT_NAME} ${LIBCURL_LIBRARY})
# Link in math library for cJSON
target_link_libraries(${OUTPUT_NAME} m)
