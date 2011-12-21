#!/usr/bin/perl -w
#
# autor:Manuel Hevia
# description:
#
#-------------------------------------------
use strict;
my $YACC = "yacc";
my $INCLUDES = "../includes";

my $estado = '';
$estado=`$YACC -dt parser.y`;
$estado=`mv -f y.tab.c parser.c`;
$estado=`echo '#ifndef parser_h' > $INCLUDES/parser.h`;
$estado=`echo '#define parser_h' >> $INCLUDES/parser.h`;
$estado=`cat y.tab.h >> $INCLUDES/parser.h`;
$estado=`echo '#endif' >> $INCLUDES/parser.h`;
$estado=`rm -f y.tab.h`;
