#!/usr/bin/perl -w
#
# autor:Manuel Hevia
# description: 
#
#-------------------------------------------
use strict;

sub p1
{
  open(f_in,"< parser_descent_rules.cpp")||die("error open < file");
  my @raw_data=<f_in>;
  open(f_out,"> parser_descent_rules.cpp_no_debug")||die("error open > file");

  my $method_detected = 0;
  my $method_id = "";
  foreach my $l (@raw_data)
  {
        chomp($l);
        $_= $l;

#		if( $_ =~ /printf\("##/ )
        if( $_ =~ /  trace\(/ )
		{
			# i does not save this line
		}
		else
		{
			print f_out "$_\n";
		}
  }

  close(f_in);
  close(f_out);
}

p1;
