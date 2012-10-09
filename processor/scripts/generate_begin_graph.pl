#!/usr/bin/perl -w
#
# autor:Manuel Hevia
# description:
#
#-------------------------------------------
use strict;

sub p1
{

  my $PYTHON_THIRD_PARTY = shift;
  my $out_dir = shift;

  open(f_in,"< ${PYTHON_THIRD_PARTY}out_begin_2.gv")||die("error open < file");
  my @raw_data=<f_in>;
  open(f_out,"> ${out_dir}generate_begin_graph.cpp")||die("error open > file");

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

sub main
{
  my $num_args = $#ARGV;

  if ($num_args != 1) {
    print "\n need <python third party> <out_path>\n";
    return;
  }

  p1($ARGV[0], $ARGV[1]);
}

main();
