#!/usr/bin/perl -w
#
# autor:Manuel Hevia
# description:
#
#-------------------------------------------
use strict;

my $tests_dir = "../test_real/";
#my $tests_dir = "../test/";

sub is_test_ok
{
  my $f = $_[0];

  my $result = "fail";

  open(f_in,"< $f")||die("error open < $f");
  my @raw_data=<f_in>;

  foreach my $l (@raw_data)
  {
    chomp($l);
    $_= $l;

    if( $_ =~ /ALL_TOKENS_CONSUMED/ )
		{
			$result = "ok"
		}
  }

  close(f_in);

  return $result;
}

sub test_run_tokens_consumed
{
  my $f = $_[0];

  print " [$f]->";
  system "./abidos --test_all_tokens_consumed_flag --test_original --ts_show --verbose $tests_dir$f > ../test_out/out_$f.txt";

  my $result = is_test_ok("../test_out/out_$f.txt");

  if( $result eq "ok" ) {
    print " [$result]";
  } else {
    print " [$result] <--##\n";
  }

  return $result;
}

sub test_gcc_diff
{
  my $f = $_[0];
  
  print "# [$f]->";
  
  my @args = ("test/g++", "-s", "$f");
  system(@args) == 0 or die "system @args failed: $?";
}   

sub all_tests
{
#	$tests_dir = "../test/";
  my $result = "";
  my $tests_total = 0;
  my $tests_ok    = 0;
  
  my $columns = 0;
	opendir(IMD, $tests_dir) || die("Cannot open directory");
	my @tests_files= readdir(IMD);
	closedir(IMD);

	my @tests_files_sorted = sort(@tests_files);

	foreach my $f (@tests_files_sorted)
	{
		unless ( ($f eq ".") || ($f eq "..") )
		{
      $result = test_run_tokens_consumed($f);
      if( $result eq "ok" )
      {
        $tests_ok++;
      }
      $tests_total++;
      if(length($f) < 10) {
        if($columns < 5) {
          ++$columns;
        } else {
          $columns = 0;
          print "\n";
        }
      } else {
        $columns = 0;
        print "\n";
      }
#			test_gcc_diff($f);
		}
	}
  print "\n  -----------------------------------\n";
  print "  tests ok $tests_ok/$tests_total\n";
  print "  -----------------------------------\n";
  if( $tests_ok == $tests_total )
  {
    return 0;
  }
  else
  {
    return -1;
  }
}

print "abidos runing suit tests [v0.0.01]\n";
my $result_total = 0;
print "{\n";
$result_total = all_tests;
print "}\n";
exit($result_total);
