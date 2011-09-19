#!/usr/bin/perl
#
# autor:Manuel Hevia
# description:
#
#-------------------------------------------

sub is_test_ok
{
  $f = $_[0];

  $result = "fail";

  open(f_in,"< $f")||die("error open < $f");
  @raw_data=<f_in>;

  foreach $l (@raw_data)
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
  $f = $_[0];

  print "  [$f] ->";
  system "./abidos --test_all_tokens_consumed_flag --test_original --ts_show --verbose $tests_dir$f > ../test_out/out_$f.txt";

  $result = is_test_ok("../test_out/out_$f.txt");

  print "  [$f] -> [$result]\n";
  
#  print "./abidos --test_all_tokens_consumed_flag --test_original --ts_show --verbose $tests_dir$f > test_out/out_$f.txt\n";

  return $result;
}

sub test_gcc_diff
{
  $f = $_[0];
  
  print "#  [$f] ->";
  
  @args = ("test/g++", "-s", "$f");
  system(@args) == 0 or die "system @args failed: $?";
}   

sub all_tests
{
	$tests_dir = "../test/";
  $result = "";
  $tests_total = 0;
  $tests_ok    = 0;
	opendir(IMD, $tests_dir) || die("Cannot open directory");
	@tests_files= readdir(IMD);
	closedir(IMD);

	@tests_files_sorted = sort(@tests_files);

	foreach $f (@tests_files_sorted)
	{
		unless ( ($f eq ".") || ($f eq "..") )
		{
      $result = test_run_tokens_consumed($f);
      if( $result eq "ok" )
      {
        $tests_ok++;
      }
      $tests_total++;
#			test_gcc_diff($f);
		}
	}
  print "  -----------------------------------\n";
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
$result = 0;
print "{\n";
$result = all_tests;
print "}\n";
exit($result);
