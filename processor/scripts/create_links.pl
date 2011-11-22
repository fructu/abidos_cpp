#!/usr/bin/perl -w
#
# autor:Manuel Hevia
# description:
#
#-------------------------------------------
use strict;

sub files_process()
{
  my $files = shift;

  foreach my $f (@$files)
  {
    chomp($f);
    print "file [$f]";
    my $name = '';
    if( $f =~ /..\/[\w]*\/([\w\.]*)/){
      $name = $1;    
    }
    chomp($name);
    print "name [$name]\n";

    my $comand = "rm $name";
    `$comand`;
    $comand = "ln -s $f $name";
    print "$comand\n";
    `$comand`;    
  }  
}

print "create links [v0.0.01] first do make all\n";
my @files = `ls ../src/*.cpp`;
&files_process(\@files);
my @files = `ls ../src/*.h`;
&files_process(\@files);
my @files = `ls ../includes/*.h`;
&files_process(\@files);

