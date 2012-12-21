#!/usr/bin/perl -w
#
# autor:Manuel Hevia
# description:
#
# use:
#  /processor$ perl -cw scripts/trace_prune.pl
#  /processor$ perl scripts/trace_prune.pl test_out/trace_t001.cpp.gv
#
#  gedit test_out/trace_t001.cpp.gv.pruned.gv
#  xdot_run.py test_out/trace_t001.cpp.gv.pruned.gv
#
#
#-------------------------------------------
use strict;
my %nodes=();

#
# here i mark the nodes with this:
#   is_not
#
# and this nodes does not have
#   is
#
sub prune_direct_nodes
{
  my $file_name_input = shift;
  open my $file_input, "<$file_name_input" or die "Can't open $file_name_input for reading: $!";

  while( my $line = <$file_input> )
  {
    if( $line =~ /(_[\d]+_[^\[]+)\[/ ) {
      my $node=$1;
      if( $line =~ / is / ) {
        $nodes{$node}=1;
      }elsif( $line =~ / is_not / ) {
        $nodes{$node}=0;
      }
    }
  }

  close($file_input);
}

#
# search case like:
#   mother -> son_pruned
#
#
#
sub prune_mothers
{
  my $file_name_input = shift;
  my $n_changes = 0;
  open my $file_input, "<$file_name_input" or die "Can't open $file_name_input for reading: $!";

  while( my $line = <$file_input> )
  {
    if( $line =~ /(_[\d]+_[^\-]+)->(_[\d]+_[^;]+);/ ) {
      my $node_mother=$1;
      my $node_son=$2;
#      printf "### $node_mother -> $node_son\n";

      if( defined($nodes{$node_son}) and ($nodes{$node_son} == 0) ) {
        if( defined($nodes{$node_mother}) != 1 ) {
          $nodes{$node_mother}=0;
          ++$n_changes;
        }
      }

      if( defined($nodes{$node_son}) and ($nodes{$node_son} == 1) ) {
        if( defined($nodes{$node_mother}) != 1 ) {
          $nodes{$node_mother}=1;
          ++$n_changes;
        } else {
          if( 0 == $nodes{$node_mother} ){
            $nodes{$node_mother}=1;
            ++$n_changes;
          }
        }
      }

    }
  }

  printf "  prune_mothers number changes=$n_changes\n";

  close($file_input);

  return $n_changes;
}

#
# put // in the begin of lines with pruned nodes
#
sub generate_dot
{
  my $file_name_input = shift;
  my $file_name_output = "";

  if( $file_name_input =~ /([\d\s\_\/\da-z.]+)\.gv/ ){
    $file_name_output = $1."_pruned.gv";;
  }else{
    $file_name_output = $file_name_input.".pruned.gv";
  }
  print "  file_output=[$file_name_output]\n";

  open my $file_input, "<$file_name_input" or die "Can't open $file_name_input for reading: $!";
  open my $file_output, ">$file_name_output" or die "Can't open $file_name_output for writing: $!";

  while( my $line = <$file_input> )
  {
    if( $line =~ /(_[\d]+_[^\[]+)\[/ ) {
      my $node=$1;

      if( defined($nodes{$node}) and ($nodes{$node} == 0) ) {
        $line = "//$line";
      }
    }elsif( $line =~ /(_[\d]+_[^\-]+)->(_[\d]+_[^;]+);/ ) {
      my $node_mother=$1;
      my $node_son=$2;

      if( defined($nodes{$node_mother}) and ($nodes{$node_mother} == 0) ) {
        $line = "//$line";
      }elsif( defined($nodes{$node_son}) and ($nodes{$node_son} == 0) ) {
        $line = "//$line";
      }
    }

    print $file_output $line;
  }

  close($file_output);
  close($file_input);
}

sub main
{
  my $num_args = $#ARGV;

  if ($num_args != 0) {
    print "\n need <trace_file.gv>\n";
    return;
  }

  my $file_input = $ARGV[0];

  print "trace_prune\n";
  print "{\n";
  print "  file_input=[$file_input]\n";
  prune_direct_nodes($file_input);

  my $n_changes = prune_mothers($file_input);
  while($n_changes > 0){
    $n_changes = prune_mothers($file_input);
  }

  generate_dot($file_input);
  print "}\n";
}


main();

