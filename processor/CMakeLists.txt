#
# project abidos_cpp
# Manuel Hevia
# make VERBOSE=1
#

cmake_minimum_required(VERSION 2.8)

set(CMAKE_VERBOSE_MAKEFILE 1 CACHE STRING "1 is necesary to run make &> make_out.txt and use abidos_make_process.pl")
SET(CMAKE_BUILD_TYPE Debug CACHE STRING "Debug or Release avalible")
SET(CMAKE_INSTALL_PREFIX /usr/local/bin/ CACHE STRING "abidos_cpp install in /opt/abidos_cpp and links in /usr/local/bin/")
SET(CMAKE_MAN_INSTALL /usr/local/man/man1/ CACHE STRING "man abidos_cpp install in /usr/local/man/man1/")

message("-- CMAKE_VERBOSE_MAKEFILE ${CMAKE_VERBOSE_MAKEFILE}")
message("-- CMAKE_BUILD_TYPE ${CMAKE_BUILD_TYPE}")
message("-- CMAKE_INSTALL_PREFIX ${CMAKE_INSTALL_PREFIX}")


INCLUDE(FindPerl)
include(FindPythonInterp)
#INCLUDE(FindPython)

#the next will bee reemplace with the script/put_ifndef_date.pl
set(PROJECT_COMPILATION_TIME "COMPILATION_TIME")

# The version number.
set( VERSION_MAJOR 1 )
set( VERSION_MINOR 0 )
set( VERSION_PATCH 16 )

# configure a header file to pass some of the CMake settings
# to the source code

set(EXECUTABLE abidos_cpp)

project (${EXECUTABLE})

set(CMAKE_SOURCE_DIR "${PROJECT_SOURCE_DIR}/src/")
set(CMAKE_PROJECT_DIR "${PROJECT_SOURCE_DIR}/")
set(PROJECT_OUT_DIR "${PROJECT_SOURCE_DIR}/out/")
set(CMAKE_PROJECT_PREPROCESSOR "${PROJECT_SOURCE_DIR}/../preprocessor/")

set(INCLUDES "${PROJECT_SOURCE_DIR}/includes/")
set(SCRIPTS_DIR "${PROJECT_SOURCE_DIR}/scripts/")
set(PYTHON_THIRD_PARTY "${PROJECT_SOURCE_DIR}/python/")
set(TEST_INCLUDES "${PROJECT_SOURCE_DIR}/test_includes/")

set(CMAKE_TEST_DIR "${PROJECT_SOURCE_DIR}/test/")
set(CMAKE_TEST_OUT_DIR "${PROJECT_SOURCE_DIR}/test_out/")
set(CMAKE_EXECUTABLE_WITH_DIR "${PROJECT_SOURCE_DIR}/src/${EXECUTABLE}")
#message("## includes->[${INCLUDES}]")
set(CMAKE_ABIDOS_WORKING_DIR "${PROJECT_SOURCE_DIR}/.abidos_cpp/")
set(CMAKE_MAN "${PROJECT_SOURCE_DIR}/doc/man/")

add_subdirectory(${INCLUDES})
add_subdirectory(${CMAKE_SOURCE_DIR})

ENABLE_TESTING()

