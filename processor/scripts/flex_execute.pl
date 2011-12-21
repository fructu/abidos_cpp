#!/usr/bin/perl -w
#
# autor:Manuel Hevia
# description:
#
#-------------------------------------------
use strict;
my $LEX = "lex";

my $estado = '';
$estado=`$LEX -t lexer.l >lexer.c`;
