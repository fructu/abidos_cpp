#!/usr/bin/env python

import os
import gtk
import gtk.gdk


import xdot

class MyDotWindow(xdot.DotWindow):

    def __init__(self):
        xdot.DotWindow.__init__(self)
        self.widget.connect('clicked', self.on_url_clicked)

    def on_url_clicked(self, widget, url, event):
#        dialog = gtk.MessageDialog(
#                parent = self,
#                buttons = gtk.BUTTONS_OK,
#                message_format="%s clicked" % url)
#        dialog.connect('response', lambda dialog, response: dialog.destroy())
#        dialog.run()
        to_run="gedit %s" % url
        error = os.system(to_run)
        return True

dotcode = """

