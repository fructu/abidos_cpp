#!/usr/bin/perl
#
# autor:Manuel Hevia
# description:
#
#-------------------------------------------
sub create_class
{
  $n = $_[0];

  print f_out "class A$n\n";
  print f_out "{\n";

  $i2 = 0;
  for( $i2 = 0; $i2 < 5; $i2++ )
  {
    print f_out "  int a$n\_$i2 ;\n";
  }

  print f_out "};\n";
}

sub create_class_with_parents
{
  $n = $_[0];
  $p = 0;

  print f_out "class A$n:\n";
  for( $p = int(($n)/2); $p < ($n-1); $p++ )
  {
    print f_out "  A$p ,\n";
  }

  print f_out "  A$p\n";

  print f_out "{\n";

  $i2 = 0;
  for( $i2 = 0; $i2 < 5; $i2++ )
  {
    print f_out "  int a$n\_$i2;\n";
  }

  print f_out "};\n";
}

sub p1
{
  open(f_out,"> test/mass_clases.cpp")||die("error open > file");

  $method_detected = 0;
  $method_id = "";
  $i = 0;

  create_class($i);

  for( $i = 1; $i < 10; $i++)
  {
	create_class_with_parents($i);
  }

  close(f_out);
}

p1;

