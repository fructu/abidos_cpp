#!/usr/bin/perl -w
#
# autor:Manuel Hevia
# description:
#
#-------------------------------------------
use strict;

sub create_class
{
  my $n = $_[0];

  print f_out "class A$n\n";
  print f_out "{\n";

  my $i2 = 0;
  for( $i2 = 0; $i2 < 5; $i2++ )
  {
    print f_out "  int a$n\_$i2 ;\n";
  }

  print f_out "};\n";
}

sub create_class_with_parents
{
  my $n = $_[0];
  my $p = 0;

  print f_out "class A$n:\n";
  for( $p = int(($n)/2); $p < ($n-1); $p++ )
  {
    print f_out "  A$p ,\n";
  }

  print f_out "  A$p\n";

  print f_out "{\n";

  my $i2 = 0;
  for( $i2 = 0; $i2 < 5; $i2++ )
  {
    print f_out "  int a$n\_$i2;\n";
  }

  print f_out "};\n";
}

sub p1
{
  open(f_out,"> test/mass_clases.cpp")||die("error open > file");

  my $i = 0;

  create_class($i);

  for( $i = 1; $i < 10; $i++)
  {
	create_class_with_parents($i);
  }

  close(f_out);
}

p1;

