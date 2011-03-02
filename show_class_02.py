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
                URL="Animal;name;age;hi",
                label = "{Animal\l + name : string\l+ age : int\l + hi() : void\l}"
        ]
        A [
        URL="A;a1;age;die",
                label = "{A|+ a1 : string\l+ age : int\l|+ die() : void\l}"
        ]
        
        B [
        URL="B;b1;b2;b3;b4;b5;b6;b7;b8;b9;b10;b11;b12;b13;b14;b15;b16;b17;b18;b19;b20",
                label = "{B\lb1\lb2\lb3\lb4\lb5\lb6\lb7\lb8\lb9\lb10\lb11\lb12\lb13\lb14\lb15\lb16\lb17\lb18\lb19\lb20\l}"
        ]
        
        B->A;
        B->Animal;
}
"""

def main():
    window = MyDotWindow()
    window.set_dotcode(dotcode)
    window.connect('destroy', gtk.main_quit)
    gtk.main()

if __name__ == '__main__':
    main()

