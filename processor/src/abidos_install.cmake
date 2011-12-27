message("creating links")
message("  /usr/local/bin/abidos")
message("  /usr/local/bin/abidos_make_process.pl")
message("  /usr/local/bin/xdot_run.py")
message("  /usr/lib/python2.6/dist-packages/xdot.py")

EXECUTE_PROCESS(
  COMMAND ln; -s;/opt/abidos/abidos; /usr/local/bin/;
  COMMAND ln; -s;/opt/abidos/abidos_make_process.pl; /usr/local/bin/;
  COMMAND ln; -s;/opt/abidos/xdot_run.py; /usr/local/bin/;
  COMMAND ln; -s;/opt/abidos/xdot.py; /usr/lib/python2.6/dist-packages/;
)

