#!/usr/bin/perl -w
#
# autor:Manuel Hevia
# description:
#
#-------------------------------------------
use strict;

#${PROJECT_SOURCE_DIR}

sub check_directories
{
  my $project_dir = shift;
  my @directories = ("${project_dir}out", "${project_dir}test_out", "${project_dir}.abidos_cpp");

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

sub main
{
  my $num_args = $#ARGV;

  if ($num_args != 0) {
    print "\n need <project path>\n";
    return;
  }

  check_directories($ARGV[0]);
}

main();
