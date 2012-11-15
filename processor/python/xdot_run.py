#!/usr/bin/env python
# -*- coding: utf-8 -*-
#
# example of xdot with multiline support
# author: F. Manuel Hevia (fructu@gmail.com)
# part of abidos https://github.com/fructu/abidos
#
# original code_
#  http://code.google.com/p/jrfonseca/wiki/XDot
#  Copyright 2008 Jose Fonseca
#

import os
import sys
import gtk
import gtk.gdk

import xdot

class MyDotWindow(xdot.DotWindow):

    def __init__(self):
        #activate/desactivate the feature here:
        self.multi_line = 1
        xdot.DotWindow.__init__(self, self.multi_line)
        self.widget.connect('clicked', self.on_url_clicked)

    def on_url_clicked(self, widget, url, event):
        if url is not None:
          if 1 == self.multi_line:
            call_parts = url.split(":")
            file_parts = call_parts[0].split("[")
            line_parts = call_parts[1].split("]")

#            command = "echo url "+ file_parts[1]+" +"+ line_parts[0]
#gedit
            command = "gedit "+ file_parts[1]+" +"+ line_parts[0]
#kate
#            command = "kate " + file_parts[1]+" -l"+ line_parts[0]
          else:
            command = "echo 'url "+ url + "'"
            
        to_run="%s" % command
        error = os.system(to_run)            

        return True

def main():
    file_dot = ".abidos/files.dot"

    if len(sys.argv) > 1:
      file_dot = sys.argv[1]

    if not os.path.exists(file_dot):
      sys.exit('ERROR: %s not found!' % file_dot)

    window = MyDotWindow()
    window.set_filter("dot")
    window.open_file(file_dot);
    window.connect('destroy', gtk.main_quit)
    gtk.main()

if __name__ == '__main__':
    main()
