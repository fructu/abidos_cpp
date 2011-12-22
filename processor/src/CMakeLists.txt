#
# project abidos
# Manuel Hevia
# make VERBOSE=1
#

set(VERBOSE 1)
#set(CMAKE_BUILD_TYPE ${GLOBAL_CMAKE_BUILD_TYPE})
# Set a default build type for single-configuration
# CMake generators if no build type is set.

find_package(BISON)
find_package(FLEX)

IF(NOT CMAKE_CONFIGURATION_TYPES AND NOT CMAKE_BUILD_TYPE)
   SET(CMAKE_BUILD_TYPE Debug)
ENDIF(NOT CMAKE_CONFIGURATION_TYPES AND NOT CMAKE_BUILD_TYPE)

set(CMAKE_CXX_FLAGS_RELEASE "-Wall -c -o3 -I${INCLUDES}")
set(CMAKE_CXX_FLAGS_DEBUG   "-Wall -c -g -I${INCLUDES}")
SET(CMAKE_CXX_FLAGS_DEFAULT ${CMAKE_CXX_FLAGS_DEBUG})
SET(CMAKE_CXX_FLAGS         ${CMAKE_CXX_FLAGS_DEBUG})

set(CMAKE_CXX_LINK_EXECUTABLE "${CMAKE_CXX_COMPILER} -o ${EXECUTABLE} <OBJECTS>")

#set (CMAKE_C_FLAGS ${CMAKE_CXX_FLAGS})
#if not lexx fail..
set (CMAKE_C_FLAGS ${CMAKE_CXX_FLAGS_DEBUG})
set (CMAKE_C_FLAGS_DEBUG ${CMAKE_CXX_FLAGS_DEBUG})

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

add_custom_target(
  test_run
	COMMAND ${SCRIPTS_DIR}tests_run.pl
	DEPENDS ${EXECUTABLE}
)

set(TEST_TO_RUN "t028.cpp")

add_custom_target(
  execute_one_test
	COMMAND ${SCRIPTS_DIR}execute_one_test.pl ${TEST_TO_RUN}
	DEPENDS ${EXECUTABLE}
)

# install
set(INSTALL_DIR "/opt/abidos")
set(INSTALL_INCLUDES_DIR "/opt/abidos/includes")

INSTALL(FILES ${EXECUTABLE} DESTINATION ${INSTALL_DIR}
  PERMISSIONS OWNER_READ OWNER_WRITE WORLD_READ WORLD_EXECUTE
)
INSTALL(FILES ${SCRIPTS_DIR}abidos_make_process.pl DESTINATION ${INSTALL_DIR}
  PERMISSIONS OWNER_READ OWNER_WRITE WORLD_READ WORLD_EXECUTE
)
INSTALL(FILES ${PYTHON_THIRD_PARTY}out_begin.py DESTINATION ${INSTALL_DIR})
INSTALL(FILES ${PYTHON_THIRD_PARTY}out_end.py   DESTINATION ${INSTALL_DIR})
INSTALL(FILES ${PYTHON_THIRD_PARTY}xdot2.py     DESTINATION ${INSTALL_DIR})
INSTALL(FILES ${TEST_INCLUDES}stdio.h           DESTINATION ${INSTALL_INCLUDES_DIR})
INSTALL(FILES ${TEST_INCLUDES}std.h             DESTINATION ${INSTALL_INCLUDES_DIR})
INSTALL(FILES ${TEST_INCLUDES}inttypes.h        DESTINATION ${INSTALL_INCLUDES_DIR})

# uninstall
add_custom_target(
  uninstall
	COMMAND rm -rf /opt/abidos
)

