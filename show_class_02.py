#!/usr/bin/env python
# -*- coding: utf-8 -*-

import gtk
import gtk.gdk

import xdot2

class MyDotWindow(xdot2.DotWindow):

    def __init__(self):
        xdot2.DotWindow.__init__(self)
        self.widget.connect('clicked', self.on_url_clicked)

    def on_url_clicked(self, widget, url, event):
        dialog = gtk.MessageDialog(
                parent = self,
                buttons = gtk.BUTTONS_OK,
                message_format="%s clicked" % url)
        dialog.connect('response', lambda dialog, response: dialog.destroy())
        dialog.run()
        return True

dotcode = """
digraph G {
        fontname = "Bitstream Vera Sans"
        fontsize = 8

        node [
                fontname = "Bitstream Vera Sans"
                fontsize = 8
                shape = "record"
        ]

        edge [
                fontname = "Bitstream Vera Sans"
                fontsize = 8
        ]

        Animal [
                URL="animal;name;age;hi",
                label = "{Animal|+ name : string\l+ age : int\l|+ hi() : void\l}"
        ]
        A [
		URL="A;a1;age;die",
                label = "{A|+ a1 : string\l+ age : int\l|+ die() : void\l}"
        ]
}
"""

def main():
    window = MyDotWindow()
    window.set_dotcode(dotcode)
    window.connect('destroy', gtk.main_quit)
    gtk.main()

if __name__ == '__main__':
    main()

