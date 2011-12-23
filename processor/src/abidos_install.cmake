EXECUTE_PROCESS(
#  COMMAND ${SCRIPTS_DIR}create_links.pl
  COMMAND echo "creating links";
  COMMAND ln; -s;/opt/abidos/abidos; /usr/local/bin/;
  COMMAND ln; -s;/opt/abidos/abidos_make_process.pl; /usr/local/bin/;
)
