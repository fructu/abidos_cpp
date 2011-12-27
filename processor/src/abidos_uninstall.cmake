message("deleting /opt/abidos")
message("  /usr/local/bin/abidos")
message("deleting links")  
message("  /usr/local/bin/abidos_make_process.pl")
message("  /usr/local/bin/xdot_run.py")
message("  /usr/lib/python2.6/dist-packages/xdot.py")
message("  /usr/lib/python2.6/dist-packages/xdot.pyc")  
  
EXECUTE_PROCESS(  
	COMMAND rm -rf ${INSTALL_DIR}
	COMMAND rm -f  ${INSTALL_BIN_LINKS_DIR}${EXECUTABLE}
	COMMAND rm -f  ${INSTALL_BIN_LINKS_DIR}abidos_make_process.pl
	COMMAND rm -f  ${INSTALL_BIN_LINKS_DIR}xdot_run.py
	COMMAND rm -f  ${INSTALL_LIB_PYTHON_LINKS_DIR}xdot.py
	COMMAND rm -f  ${INSTALL_LIB_PYTHON_LINKS_DIR}xdot.pyc
)

