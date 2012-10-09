#!/usr/bin/perl -w
#
# autor:Manuel Hevia
# description:
#
#-------------------------------------------
use strict;

sub flex_execute
{
  #my $LEX = "lex";
  my $LEX    = shift;
  my $source = shift;

  my $estado = '';
  $estado=`$LEX -t ${source}lexer.l > ${source}lexer.c`;
}

sub main
{
  my $num_args = $#ARGV;

  if ($num_args != 1) {
    print "\n need <flex path> <source path>\n";
    return;
  }

  flex_execute($ARGV[0], $ARGV[1]);
}

main();
