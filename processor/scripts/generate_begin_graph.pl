#!/usr/bin/perl -w
#
# autor:Manuel Hevia
# description:
#
#-------------------------------------------
use strict;

sub p1
{
  open(f_in,"< ../python/out_begin_2.gv")||die("error open < file");
  my @raw_data=<f_in>;
  open(f_out,"> ../out/generate_begin_graph.cpp")||die("error open > file");

  foreach my $l (@raw_data)
  {
    chomp($l);
    $_= $l;
    s/"/\\"/g;
		print f_out "fprintf(f_out, \"$_\\n\");\n";
  }

  close(f_in);
  close(f_out);
}

p1;
