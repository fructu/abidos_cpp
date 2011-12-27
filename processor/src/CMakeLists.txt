#
# project abidos
# Manuel Hevia
# make VERBOSE=1
#

#set(CMAKE_BUILD_TYPE ${GLOBAL_CMAKE_BUILD_TYPE})
# Set a default build type for single-configuration
# CMake generators if no build type is set.
INCLUDE (CheckIncludeFiles)

CHECK_INCLUDE_FILES(ctype.h CTYPE_H)
CHECK_INCLUDE_FILES(dirent.h DIRENT_H)
CHECK_INCLUDE_FILES(getopt.h GETOPT_H)
CHECK_INCLUDE_FILES(map MAP)
CHECK_INCLUDE_FILES(stdio.h STDIO_H)
CHECK_INCLUDE_FILES(stdlib.h STDLIB_H)
CHECK_INCLUDE_FILES(string STRING)
CHECK_INCLUDE_FILES(string.h STRING_H)
CHECK_INCLUDE_FILES(sys/param.h SYS_PARAM_H)
CHECK_INCLUDE_FILES(sys/stat.h SYS_STAT_H)
CHECK_INCLUDE_FILES(sys/types.h SYS_TYPES_H)
CHECK_INCLUDE_FILES(unistd.h UNISTD_H)
CHECK_INCLUDE_FILES(vector VECTOR)

find_package(BISON)
find_package(FLEX)

IF(NOT CMAKE_CONFIGURATION_TYPES AND NOT CMAKE_BUILD_TYPE)
   SET(CMAKE_BUILD_TYPE Debug)
ENDIF(NOT CMAKE_CONFIGURATION_TYPES AND NOT CMAKE_BUILD_TYPE)

set(CMAKE_CXX_FLAGS_RELEASE "-Wall -c -o3 -I${INCLUDES}")
set(CMAKE_CXX_FLAGS_DEBUG   "-Wall -c -g -I${INCLUDES}")
SET(CMAKE_CXX_FLAGS_DEFAULT ${CMAKE_CXX_FLAGS_DEBUG})
#to avoid duplication i commented next line
#SET(CMAKE_CXX_FLAGS         ${CMAKE_CXX_FLAGS_DEBUG})

set(CMAKE_CXX_LINK_EXECUTABLE "${CMAKE_CXX_COMPILER} -o ${EXECUTABLE} <OBJECTS>")

#set (CMAKE_C_FLAGS ${CMAKE_CXX_FLAGS})
#if not lexx fail..
set (CMAKE_C_FLAGS ${CMAKE_CXX_FLAGS_DEBUG})
set (CMAKE_C_FLAGS_DEBUG ${CMAKE_CXX_FLAGS_DEBUG})

set(LEX  ${FLEX_EXECUTABLE})
set(YACC ${BISON_EXECUTABLE})

set_source_files_properties(lexer.c PROPERTIES GENERATED TRUE)
# lex
add_custom_command(
   OUTPUT lexer.c
   COMMAND ${SCRIPTS_DIR}flex_execute.pl
   DEPENDS parser.c ${INCLUDES}parser.h
)   

set_source_files_properties(ts.cpp PROPERTIES GENERATED TRUE)
set_source_files_properties(ts.h PROPERTIES GENERATED TRUE)

add_custom_command(
   OUTPUT ts.cpp ts.h
   COMMAND echo "coping ts.cpp from preprocessor"
   COMMAND cp ../../preprocessor/ts.cpp .
   COMMAND cp ../../preprocessor/ts.h .   
)

set_source_files_properties(${INCLUDES}parser.h PROPERTIES GENERATED TRUE)
set_source_files_properties(parser.c PROPERTIES GENERATED TRUE)

#yacc
add_custom_command(
   OUTPUT  ${INCLUDES}parser.h parser.c
   COMMAND ${SCRIPTS_DIR}bison_execute.pl
   DEPENDS parser.y
)

set_source_files_properties(tokens.h PROPERTIES GENERATED TRUE)

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
  COMMAND ${SCRIPTS_DIR}check_directories.pl
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
	generator_check_include_files
	main	
)

add_dependencies(${EXECUTABLE} parser_descent_cpp generator_class_diagram_cpp config_h)

add_custom_target(
  indent_run
  COMMAND ${SCRIPTS_DIR}check_indent.sh
  COMMAND ${SCRIPTS_DIR}indent_run.sh
  DEPENDS ${EXECUTABLE}
)

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

set(ADDITIONAL_MAKE_CLEAN_FILES
  ${INCLUDES}config.h
  parser.c
  ts.h
)

# install
#set(INSTALL_BIN_LINKS_DIR "/usr/local/bin/")
set(INSTALL_BIN_LINKS_DIR ${CMAKE_INSTALL_PREFIX})
set(INSTALL_LIB_PYTHON_LINKS_DIR "/usr/lib/python2.6/dist-packages/")
set(INSTALL_DIR "/opt/abidos/")
set(INSTALL_INCLUDES_DIR "/opt/abidos/includes")

INSTALL(FILES ${EXECUTABLE} DESTINATION ${INSTALL_DIR}
  PERMISSIONS OWNER_READ OWNER_WRITE WORLD_READ WORLD_EXECUTE
)
INSTALL(FILES ${SCRIPTS_DIR}abidos_make_process.pl DESTINATION ${INSTALL_DIR}
  PERMISSIONS OWNER_READ OWNER_WRITE WORLD_READ WORLD_EXECUTE
)
INSTALL(FILES ${PYTHON_THIRD_PARTY}xdot_run.py DESTINATION ${INSTALL_DIR}
  PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE
)
INSTALL(FILES ${PYTHON_THIRD_PARTY}xdot.py     DESTINATION ${INSTALL_DIR}
  PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE
)

INSTALL(FILES ${TEST_INCLUDES}stdio.h           DESTINATION ${INSTALL_INCLUDES_DIR})
INSTALL(FILES ${TEST_INCLUDES}std.h             DESTINATION ${INSTALL_INCLUDES_DIR})
INSTALL(FILES ${TEST_INCLUDES}inttypes.h        DESTINATION ${INSTALL_INCLUDES_DIR})

INSTALL( CODE" EXECUTE_PROCESS(COMMAND echo \"   link ${INSTALL_DIR}abidos <- ${INSTALL_BIN_LINKS_DIR}abidos\")")
INSTALL( CODE" EXECUTE_PROCESS(COMMAND ln; -s;${INSTALL_DIR}abidos; ${INSTALL_BIN_LINKS_DIR})")

INSTALL( CODE" EXECUTE_PROCESS(COMMAND echo \"   link ${INSTALL_DIR}abidos_make_process.pl <- ${INSTALL_BIN_LINKS_DIR}abidos_make_process.pl\")")
INSTALL( CODE" EXECUTE_PROCESS(COMMAND ln; -s;${INSTALL_DIR}abidos_make_process.pl; ${INSTALL_BIN_LINKS_DIR})")

INSTALL( CODE" EXECUTE_PROCESS(COMMAND echo \"   link ${INSTALL_DIR}xdot_run.py <- ${INSTALL_BIN_LINKS_DIR}xdot_run.py\")")
INSTALL( CODE" EXECUTE_PROCESS(COMMAND ln; -s;${INSTALL_DIR}xdot_run.py; ${INSTALL_BIN_LINKS_DIR};)")

INSTALL( CODE" EXECUTE_PROCESS(COMMAND echo \"   link ${INSTALL_DIR}xdot_run.py <- ${INSTALL_BIN_LINKS_DIR}xdot_run.py\")")
INSTALL( CODE" EXECUTE_PROCESS(COMMAND ln; -s;${INSTALL_DIR}xdot.py; ${INSTALL_LIB_PYTHON_LINKS_DIR})")


# uninstall
add_custom_target(
  uninstall
	COMMAND echo "deleting"
	COMMAND echo   "  ${INSTALL_DIR}"
	COMMAND rm -rf    ${INSTALL_DIR}

	COMMAND echo "  ${INSTALL_BIN_LINKS_DIR}${EXECUTABLE}"
	COMMAND rm -f   ${INSTALL_BIN_LINKS_DIR}${EXECUTABLE}

	COMMAND echo "  ${INSTALL_BIN_LINKS_DIR}abidos_make_process.pl"
	COMMAND rm -f   ${INSTALL_BIN_LINKS_DIR}abidos_make_process.pl

	COMMAND echo "  ${INSTALL_BIN_LINKS_DIR}xdot_run.py"
	COMMAND rm -f   ${INSTALL_BIN_LINKS_DIR}xdot_run.py

	COMMAND echo "  ${INSTALL_LIB_PYTHON_LINKS_DIR}xdot.py"
	COMMAND rm -f   ${INSTALL_LIB_PYTHON_LINKS_DIR}xdot.py

	COMMAND echo "  ${INSTALL_LIB_PYTHON_LINKS_DIR}xdot.pyc"
	COMMAND rm -f   ${INSTALL_LIB_PYTHON_LINKS_DIR}xdot.pyc
)

