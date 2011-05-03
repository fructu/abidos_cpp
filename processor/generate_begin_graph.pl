#!/usr/bin/perl
#
# autor:Manuel Hevia
# description:
#
#-------------------------------------------
sub p1
{
  open(f_in,"< python/out_begin_2.gv")||die("error open < file");
  @raw_data=<f_in>;
  open(f_out,"> out/generate_begin_graph.cpp")||die("error open > file");

  $method_detected = 0;
  $method_id = "";
  foreach $l (@raw_data)
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
