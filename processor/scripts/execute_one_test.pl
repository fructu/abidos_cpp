#!/usr/bin/perl -w
#
# autor:Manuel Hevia
# description:
#
#-------------------------------------------
use strict;
my $YACC = "yacc";
my $INCLUDES = "../includes";

sub test_run
{
  my $file = shift;
  my $estado = '';

	$estado=`mkdir -p .abidos`;
	$estado=`cp ../python/xdot2.py ../out/`;
	$estado=`./abidos --verbose --ts_show --includes ../test_includes/ ../test/$file`;
	$estado=`cat ../python/out_begin.py > ../out/out_run.py`;
	$estado=`cat .abidos/out_$file.gv >> ../out/out_run.py`;
	$estado=`cat ../python/out_end.py >> ../out/out_run.py`;
	$estado=`chmod +x ../out/out_run.py`;
	$estado=`../out/out_run.py`;
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
