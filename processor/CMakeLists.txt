#
# project abidos
# Manuel Hevia
# make VERBOSE=1
#
# g++ -o abidos src/CMakeFiles/abidos.dir/generator_class_diagram.cpp.o  src/CMakeFiles/abidos.dir/parser_descent_preprocessor.cpp.o src/CMakeFiles/abidos.dir/generator_original.cpp.o       src/CMakeFiles/abidos.dir/parser_descent_rules.cpp.o src/CMakeFiles/abidos.dir/lexer.c.o                      src/CMakeFiles/abidos.dir/semantic.cpp.o src/CMakeFiles/abidos.dir/loader.cpp.o                   src/CMakeFiles/abidos.dir/symbols_table.cpp.o src/CMakeFiles/abidos.dir/main.cpp.o                     src/CMakeFiles/abidos.dir/trace.cpp.o src/CMakeFiles/abidos.dir/options.cpp.o                  src/CMakeFiles/abidos.dir/ts.cpp.o src/CMakeFiles/abidos.dir/parser_descent.cpp.o -rdynamic 
#
#/usr/bin/cmake -E cmake_link_script CMakeFiles/abidos.dir/link.txt --verbose=1
#/usr/bin/c++   -Wall -c -g -I/home/hevia/working/abidos/processor/includes -Wall -c -g -I/home/hevia/working/abidos/processor/includes   CMakeFiles/abidos.dir/options.cpp.o CMakeFiles/abidos.dir/trace.cpp.o CMakeFiles/abidos.dir/lexer.c.o CMakeFiles/abidos.dir/parser_descent.cpp.o CMakeFiles/abidos.dir/parser_descent_rules.cpp.o CMakeFiles/abidos.dir/parser_descent_preprocessor.cpp.o CMakeFiles/abidos.dir/symbols_table.cpp.o CMakeFiles/abidos.dir/semantic.cpp.o CMakeFiles/abidos.dir/generator_class_diagram.cpp.o CMakeFiles/abidos.dir/generator_original.cpp.o CMakeFiles/abidos.dir/ts.cpp.o CMakeFiles/abidos.dir/loader.cpp.o CMakeFiles/abidos.dir/main.cpp.o  -o abidos -rdynamic
#/usr/bin/c++   -Wall -c -g -I/home/hevia/working/abidos/processor/includes   CMakeFiles/abidos.dir/options.cpp.o CMakeFiles/abidos.dir/trace.cpp.o CMakeFiles/abidos.dir/lexer.c.o CMakeFiles/abidos.dir/parser_descent.cpp.o CMakeFiles/abidos.dir/parser_descent_rules.cpp.o CMakeFiles/abidos.dir/parser_descent_preprocessor.cpp.o CMakeFiles/abidos.dir/symbols_table.cpp.o CMakeFiles/abidos.dir/semantic.cpp.o CMakeFiles/abidos.dir/generator_class_diagram.cpp.o CMakeFiles/abidos.dir/generator_original.cpp.o CMakeFiles/abidos.dir/ts.cpp.o CMakeFiles/abidos.dir/loader.cpp.o CMakeFiles/abidos.dir/main.cpp.o  -o abidos -rdynamic

cmake_minimum_required(VERSION 2.8)

set(CMAKE_VERBOSE_MAKEFILE 1 CACHE STRING "1 is necesary to run make &> make_out.txt and use abidos_make_process.pl")
SET(CMAKE_BUILD_TYPE Debug CACHE STRING "Debug or Release avalible")
SET(CMAKE_INSTALL_PREFIX /usr/local/bin/ CACHE STRING "abidos install in /opt/abidos and links in /usr/local/bin/")

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
set( VERSION_PATCH 11 )

# configure a header file to pass some of the CMake settings
# to the source code

set(EXECUTABLE abidos)

project (${EXECUTABLE})

set(CMAKE_SOURCE_DIR "${PROJECT_SOURCE_DIR}/src/")
set(CMAKE_PROJECT_DIR "${PROJECT_SOURCE_DIR}/")
set(PROJECT_OUT_DIR "${PROJECT_SOURCE_DIR}/out/")
set(CMAKE_PROJECT_PREPROCESSOR "${PROJECT_SOURCE_DIR}/../preprocessor/")

set(INCLUDES "${PROJECT_SOURCE_DIR}/includes/")
set(SCRIPTS_DIR "${PROJECT_SOURCE_DIR}/scripts/")
set(PYTHON_THIRD_PARTY "${PROJECT_SOURCE_DIR}/python/")
set(TEST_INCLUDES "${PROJECT_SOURCE_DIR}/test_includes/")

#message("## includes->[${INCLUDES}]")

add_subdirectory(${INCLUDES})
add_subdirectory(${CMAKE_SOURCE_DIR})

ENABLE_TESTING()

