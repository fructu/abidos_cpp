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

set(CMAKE_VERBOSE_MAKEFILE 1)

# The version number.
set( VERSION_MAJOR 0 )
set( VERSION_MINOR 0 )
set( VERSION_PATCH 1 )

# configure a header file to pass some of the CMake settings
# to the source code

set(EXECUTABLE abidos)

project (${EXECUTABLE})

set(CMAKE_SOURCE_DIR "${PROJECT_SOURCE_DIR}/src/")

set(INCLUDES "${PROJECT_SOURCE_DIR}/includes/")
set(SCRIPTS_DIR "${PROJECT_SOURCE_DIR}/scripts/")
set(PYTHON_THIRD_PARTY "${PROJECT_SOURCE_DIR}/python/")

message("## source_dir->[${PROJECT_SOURCE_DIR}]")
message("## includes->[${INCLUDES}]")

add_subdirectory(${CMAKE_SOURCE_DIR})
add_subdirectory(${INCLUDES})


