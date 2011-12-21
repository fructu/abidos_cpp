#!/usr/bin/perl -w
#
# autor:Manuel Hevia
# description: 
#
#-------------------------------------------
use strict;

#rename( oldname, newname);

my $temporal_file = "file_0.tmp";

sub write_ifndef
{
  my $file   = shift;
  my $header = shift;

  if (! (-e $file))
  {
    print "[$file] not found\n";
    return
  }

  open(f_in,"< $file")||die("error open < file");
  my @raw_data_0=<f_in>;
  foreach my $l (@raw_data_0)
  {
		if($l =~ /^#ifndef/){
      close(f_in);		
		  return;
		}
  }
  close(f_in);

  rename( $file, $temporal_file);

  open(f_in,"< $temporal_file")||die("error open < file");
  my @raw_data=<f_in>;
  open(f_out,"> $file")||die("error open > file");

  print f_out "#ifndef $header\n";
  print f_out "#define $header\n";

  foreach my $l (@raw_data)
  {
		print f_out "$l";
  }
  print f_out "#endif\n";  

  close(f_in);
  close(f_out);

  unlink($temporal_file);  
}

sub write_date
{
  my $file   = shift;

  if (! (-e $file))
  {
    print "[$file] not found\n";
    return
  }

  rename( $file, $temporal_file);

  open(f_in,"< $temporal_file")||die("error open < file");
  my @raw_data=<f_in>;
  open(f_out,"> $file")||die("error open > file");

  my ($sec,$min,$hour,$mday,$mon,$year,$wday,
      $yday,$isdst)=localtime(time);

  foreach my $l (@raw_data)
  {
    if($l =~ /PROJECT_COMPILATION_TIME/) {
		  printf f_out "#define PROJECT_COMPILATION_TIME \"%4d-%02d-%02d %02d:%02d:%02d\"\n" ,
                  $year+1900,$mon+1,$mday,$hour,$min,$sec;
    } else {
      print f_out "$l";
    }
  }

  close(f_in);
  close(f_out);

  unlink($temporal_file);
}

sub main
{
  my $num_args = $#ARGV;

  if ($num_args != 1) {
    print "\n 2 parameters needed file string\n";
    return;
  }

  write_ifndef($ARGV[0], $ARGV[1]);
  write_date($ARGV[0]);
}

main();
