#
# project abidos
# Manuel Hevia
# make VERBOSE=1
#

#set(CMAKE_BUILD_TYPE ${GLOBAL_CMAKE_BUILD_TYPE})

configure_file (
  ${INCLUDES}config.h.in
  ${INCLUDES}config.h
)

add_custom_target(
  config_h
	COMMAND ${SCRIPTS_DIR}put_ifndef_date.pl ${INCLUDES}config.h config_h
	DEPENDS ${INCLUDES}config.h
)

add_dependencies(config_h config.h)
