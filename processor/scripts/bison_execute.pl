#!/usr/bin/perl -w
#
# autor:Manuel Hevia
# description:
#
#-------------------------------------------
use strict;

sub bison_execute
{
  my $YACC     = shift;
  my $SOURCE   = shift;
  my $INCLUDES = shift;
#  my $YACC = "yacc";
#  my $INCLUDES = "../includes";

  my $estado = '';
  print "$YACC -dt ${SOURCE}parser.y\n";
  $estado=`$YACC -dt ${SOURCE}parser.y`;
  $estado=`mv -f ${SOURCE}parser.tab.c ${SOURCE}parser.c`;
  $estado=`echo '#ifndef parser_h' > ${INCLUDES}parser.h`;
  $estado=`echo '#define parser_h' >> ${INCLUDES}parser.h`;
  $estado=`cat ${SOURCE}parser.tab.h >> ${INCLUDES}parser.h`;
  $estado=`echo '#endif' >> ${INCLUDES}parser.h`;
  $estado=`rm -f ${SOURCE}parser.tab.h`;
}

sub main
{
  my $num_args = $#ARGV;

  if ($num_args != 2) {
    print "\n need <yacc path> <source path> and <includes path>\n";
    return;
  }

  bison_execute($ARGV[0], $ARGV[1], $ARGV[2]);
}

main();
