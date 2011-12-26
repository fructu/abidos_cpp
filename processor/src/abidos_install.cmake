EXECUTE_PROCESS(
#  COMMAND ${SCRIPTS_DIR}create_links.pl
  COMMAND echo "creating links";
  COMMAND ln; -s;/opt/abidos/abidos; /usr/local/bin/;
  COMMAND ln; -s;/opt/abidos/abidos_make_process.pl; /usr/local/bin/;
  COMMAND ln; -s;/opt/abidos/xdot_run.py; /usr/local/bin/;
  COMMAND ln; -s;/opt/abidos/xdot.py; /usr/lib/python2.6/dist-packages/;
)
