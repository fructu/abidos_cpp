#!/usr/bin/env python

import gtk
import gtk.gdk

import xdot

class MyDotWindow(xdot.DotWindow):

    def __init__(self):
        xdot.DotWindow.__init__(self)
        self.widget.connect('clicked', self.on_url_clicked)

    def on_url_clicked(self, widget, label, event):
        dialog = gtk.MessageDialog(
                parent = self,
                buttons = gtk.BUTTONS_OK,
                message_format="%s clicked" % label)
        dialog.connect('response', lambda dialog, response: dialog.destroy())
        dialog.run()
        return True

dotcode = """

