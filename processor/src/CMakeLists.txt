#
# project abidos
# Manuel Hevia
# make VERBOSE=1
#

message("# --cmake procesing src--")
message("# includes->[${INCLUDES}]")

#CMAKE_CXX_COMPILER

message("## CMAKE_EXE_LINK_FLAGS [${CMAKE_EXE_LINK_FLAGS}]")

# Set a default build type for single-configuration
# CMake generators if no build type is set.
IF(NOT CMAKE_CONFIGURATION_TYPES AND NOT CMAKE_BUILD_TYPE)
   SET(CMAKE_BUILD_TYPE Debug)
ENDIF(NOT CMAKE_CONFIGURATION_TYPES AND NOT CMAKE_BUILD_TYPE)

set(DEBUG "-g")
set(CMAKE_CXX_FLAGS "-Wall -c ${DEBUG} -I${INCLUDES}")
set(CMAKE_CXX_FLAGS_DEBUG "-Wall -c ${DEBUG} -I${INCLUDES}")
set(CMAKE_CXX_LINK_EXECUTABLE "${CMAKE_CXX_COMPILER} -o ${EXECUTABLE} <OBJECTS>")

#link_directories (${HELLO_BINARY_DIR}) 

set (CMAKE_C_FLAGS ${CMAKE_CXX_FLAGS})
set (CMAKE_C_FLAGS_DEBUG ${CMAKE_CXX_FLAGS_DEBUG})

find_package(BISON)
find_package(FLEX)

set(LEX  ${FLEX_EXECUTABLE})
set(YACC ${BISON_EXECUTABLE})

# lex
add_custom_command(
   OUTPUT lexer.c
   COMMAND ${SCRIPTS_DIR}flex_execute.pl
   DEPENDS parser.c ${INCLUDES}parser.h
)   

add_custom_command(
   OUTPUT ts.cpp
   COMMAND echo "coping ts.cpp from preprocessor"
   COMMAND cp ../../preprocessor/ts.cpp .
   COMMAND cp ../../preprocessor/ts.h .   
)

#yacc
add_custom_command(
   OUTPUT  ${INCLUDES}parser.h
   COMMAND ${SCRIPTS_DIR}bison_execute.pl
   DEPENDS parser.y
)

add_custom_command(
  OUTPUT  ${INCLUDES}tokens.h
	COMMAND ${SCRIPTS_DIR}extract_symbols.pl
	COMMAND mv -f tokens.h ${INCLUDES}
	DEPENDS ${INCLUDES}parser.h
)

add_custom_target(
  parser_descent_cpp
	DEPENDS ${INCLUDES}tokens.h
)

#generator_class_diagram.cpp: generate_begin_graph.cpp

#generate_begin_graph.cpp: ../python/out_begin_2.gv

add_custom_target(
  generator_class_diagram_cpp
  COMMAND ${SCRIPTS_DIR}generate_begin_graph.pl
  DEPENDS ${PYTHON_THIRD_PARTY}out_begin_2.gv
)

add_executable(${EXECUTABLE} 
  options
	trace
	lexer
	parser_descent
	parser_descent_rules
	parser_descent_preprocessor
	symbols_table
	semantic
	generator_class_diagram
	generator_original
	ts
	loader
	main	
)

add_dependencies(${EXECUTABLE}  parser_descent_cpp generator_class_diagram_cpp config_h)

