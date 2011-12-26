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
my $dir = '.abidos';

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

	$estado=`mkdir -p .abidos`;
	$estado=`cp ../python/xdot.py .abidos/`;
	$estado=`cp ../python/xdot_run.py .abidos/`;	
	$estado=`./abidos --verbose --ts_show --includes ../test_includes/ ../test/$file`;
	$estado=`cp ../test/$file .abidos/$file`;	
	$estado=`.abidos/xdot_run.py`;
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
