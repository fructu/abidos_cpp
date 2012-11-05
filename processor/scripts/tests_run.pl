#!/usr/bin/perl -w
#
# autor:Manuel Hevia
# description:
#  scripts/tests_run.pl src/abidos test/ test_includes/ test_out/
#
#-------------------------------------------
use strict;

#my $tests_dir = "../test_real/";
#my $tests_dir = "../test/";

my $version = "0.0.06";

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
  my $executable_with_dir = shift;
  my $tests_dir = shift;
  my $test_includes_dir = shift;
  my $tests_dir_out = shift;
  my $f = shift;
  my $log_file = shift;

  print " [$f]->";
  my $command = "$executable_with_dir --includes $test_includes_dir --test_all_tokens_consumed_flag --test_original --ts_show --verbose $tests_dir$f > ${tests_dir_out}out_$f.txt";
  system($command);
  print $log_file "  $command\n";

  my $result = is_test_ok("${tests_dir_out}out_$f.txt");

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
  my $executable_with_dir = shift;
  my $tests_dir = shift;
  my $test_includes_dir = shift;
  my $tests_dir_out = shift;
  my $log_file = shift;

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
      $result = test_run_tokens_consumed($executable_with_dir, $tests_dir, $test_includes_dir, $tests_dir_out, $f, $log_file);
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

sub main
{
  my $num_args = $#ARGV;

  if ($num_args != 3) {
    print "\n need <executable_with_dir> <tests_dir> <test_includes_dir> <tests_dir_out>\n";
    return;
  }

  my $executable_with_dir = $ARGV[0];
  my $tests_dir           = $ARGV[1];
  my $test_includes_dir   = $ARGV[2];
  my $tests_dir_out       = $ARGV[3];

#  flex_execute($ARGV[0], $ARGV[1]);
  print "abidos runing suit tests [$version] \n";
  my $result_total = 0;
  print "{\n";
  print " executable_with_dir [$executable_with_dir]\n";
  print " test_dir            [$tests_dir]\n";
  print " test_includes_dir   [$test_includes_dir]\n";
  print " test_out dir        [$tests_dir_out]\n";

  open( log_file, ">${tests_dir_out}test_out.log") or die $!;
  print log_file "abidos runing suit tests [$version]\n";
  print log_file "{\n";

  $result_total = all_tests($executable_with_dir, $tests_dir, $test_includes_dir, $tests_dir_out, *log_file);
  
  print log_file "}\n";
  close(log_file);

  print "}\n";
  exit($result_total);
}

main();

