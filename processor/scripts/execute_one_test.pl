#!/usr/bin/perl -w
#
# autor:Manuel Hevia
# description:
#
#-------------------------------------------
use strict;
my $YACC = "yacc";
my $INCLUDES = "../includes";

use strict;
my $dir = '.abidos_cpp';

sub create_directories
{
  my @directories = ($dir);

  foreach (@directories) {
    print "  checking existence $_ ...";

    unless(-d $_){
        print "creating...";
        mkdir $_ or die "  Couldn't create dir: [$_]\n";
        print "[ok]\n";
    }
    else
    {
        print "[ok]\n";
    }
  }
}

sub test_run
{
  my $file = shift;
  my $estado = '';

	$estado=`mkdir -p .abidos_cpp`;
	$estado=`cp ../python/xdot.py .abidos_cpp/`;
	$estado=`cp ../python/xdot_run.py .abidos_cpp/`;	
	$estado=`./abidos_cpp --verbose --ts_show --includes ../test_includes/ ../test/$file > ../test_out/out_$file.txt`;
	$estado=`cp ../test/$file .abidos_cpp/$file`;	
	$estado=`.abidos_cpp/xdot_run.py`;
}

sub main
{
  my $num_args = $#ARGV;

  if ($num_args != 0) {
    print "\n need file_test\n";
    return;
  }

  test_run($ARGV[0]);
}

main();
