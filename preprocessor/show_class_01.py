#!/usr/bin/env python

import gtk
import gtk.gdk

import xdot

class MyDotWindow(xdot.DotWindow):

    def __init__(self):
        xdot.DotWindow.__init__(self)
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
                label = "{Animal|+ name : string\l+ age : int\l|+ die() : void\l}"
        ]
        A [
                label = "{A|+ a1 : string\l+ age : int\l|+ die() : void\l}"
        ]

    graph [fontsize=10 fontname="Verdana"];
    node [shape=record fontsize=10 fontname="Verdana"];



    subgraph cluster_7 {
//        rotate=90;
//        nodesep=20.0;
//       clusterrank=TD;
//         node[fontzise = 10, width="0.5", height = "0.5", margin = 0, nodesep=0, pad=0, nodesep=0];
        ranksep = 10.5;
        nodesep = 100.25;
        edge [style="invis", style="setlinewidth(0.5)"];
        "Y";
        "z" -> "X";
        "X" -> "Y";
        label = "s_1";
        color=yellow;

   }

/*
                ranksep = 1.5;
                nodesep = .25;
                edge [style="setlinewidth(3)"];

*/


     node [shape = record,height=0.08];
     node1[label = <
   <TABLE BORDER="0" CELLBORDER="1" CELLSPACING="0" CELLPADDING="4">
     <TR>
        <TD ROWSPAN="3" >hello<BR/>world</TD>
        <TD COLSPAN="3" >b</TD>
        <TD ROWSPAN="3">g</TD>
        <TD ROWSPAN="3">h</TD>
      </TR>
      <TR>  <TD>c</TD><TD PORT="here">d</TD><TD>e</TD>  </TR>
      <TR>  <TD COLSPAN="3">f</TD>  </TR>
    </TABLE>>];


    subgraph cluster_1 {
        margin="15,15"

        node [style=filled];
        "Item 3" "Item 4";
        label = "Container B";
        color=blue;
    }

    subgraph cluster_2 {
        node [style=filled];
        clusterrank=TD;
        "Item 5";
        "Item 6";
        label = "Container C";
        color=blue;
    }

    // Renders fine
    "Item 1" -> "Item 2";
    "Item 2" -> "Item 3";

    // Both of these create new nodes
    cluster_1 -> cluster_2;
    "Container A" -> "Container C";



  compound=true;
  subgraph cluster0 {
  a -> b;
  a -> c;
  b -> d;
  c -> d;
  }
  subgraph cluster1 {
  e -> g;
  e -> f;
  }
  b -> f [lhead=cluster1];
  d -> e;
  c -> g [ltail=cluster0,
  lhead=cluster1];
  c -> e [ltail=cluster0];
  d -> h;
  compound=true;
  subgraph cluster0 {
  a -> b;
  a -> c;
  b -> d;
  c -> d;
  }
  subgraph cluster1 {
  e -> g;
  e -> f;
  }

  b -> f [lhead=cluster1];
  d -> e;
  c -> g [ltail=cluster0,
  lhead=cluster1];
  c -> e [ltail=cluster0];
  d -> h;


rankdir="TD"

    "Step1" -> "Step2" -> "Step3";

    subgraph step2detail {

        "Step2" -> "note1";
        "Step2" -> "note2";
        "Step2" -> "note3";
        "Step2" -> "note4";

   }


/*    "S" -> "S";
*/

	graph [label=<
		<table border="0" cellborder="1" cellspacing="0">
		  <tr>
		    <td href="http://localhost/\N_g1.html">left1</td>
		    <td href="http://localhost/\N_g2.html">middle1</td>
		    <td href="http://localhost/\N_g3.html">right1</td>
		  </tr>
		</table>
	>]
	Anode [shape=plaintext label=<
		<table border="0" cellborder="1" cellspacing="0">
		  <tr>
		    <td href="http://localhost/\N_p1.html" port="p1">left2</td>
		    <td href="http://localhost/\N_p2.html" port="p2">middle2</td>
		    <td href="http://localhost/\N_p3.html" port="p3">right2</td>
		  </tr>
		</table>
	>]
	Anode:p2 -> Bnode [label=<
		<table border="0" cellborder="1" cellspacing="0">
		  <tr>
		    <td URL="http://localhost/\E_e1.html">left3</td>
		    <td URL="http://localhost/\E_e2.html">middle3</td>
		    <td URL="http://localhost/\E_e3.html">right3</td>
		  </tr>
		</table>
	>]

                        subgraph cluster_host1_package2
                        {
                                color=green;
                                label="";
                                {
                                        host_1_process5 [label="process5", pos="-5,2"];
                                        host_1_process6 [label="process6", pos="-6,2"];
                                        host_1_process7 [label="process7", pos="-7,2"];
                                        host_1_process8 [label="process8", pos="-8,2"];
                                        host_1_process9 [label="process9", pos="-9,2"];
                                }
                        }


}
"""

def main():
    window = MyDotWindow()
    window.set_dotcode(dotcode)
    window.connect('destroy', gtk.main_quit)
    gtk.main()

if __name__ == '__main__':
    main()

