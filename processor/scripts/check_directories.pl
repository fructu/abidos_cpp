#!/usr/bin/perl -w
#
# autor:Manuel Hevia
# description:
#
#-------------------------------------------
use strict;

my @directories = ('../out', '../test_out');

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



