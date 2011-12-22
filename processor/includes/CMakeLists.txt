#
# project abidos
# Manuel Hevia
# make VERBOSE=1
#

#set(CMAKE_BUILD_TYPE ${GLOBAL_CMAKE_BUILD_TYPE})

configure_file (
  config.h.in
  config.h
)

add_custom_target(
  config_h
	COMMAND ${SCRIPTS_DIR}put_ifndef_date.pl config.h config_h
	DEPENDS config.h
)

add_dependencies(config_h config.h)
