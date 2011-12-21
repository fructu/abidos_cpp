#
# project abidos
# Manuel Hevia
# make VERBOSE=1
#
message("# includes generating config.h ----")
configure_file (
  config.h.in
  config.h
)

