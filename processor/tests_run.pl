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

sub test_run
{
  $f = $_[0];

  print "  [$f] ->";
  system "./abidos --test_all_tokens_consumed_flag --test_original --ts_show --verbose $tests_dir/$f > test_out/out_$f.txt";

  $result = is_test_ok("test_out/out_$f.txt");

  print "  [$f] -> [$result]\n";
  
}

sub all_tests
{
	$tests_dir = "test/";
	opendir(IMD, $tests_dir) || die("Cannot open directory");
	@tests_files= readdir(IMD);
	closedir(IMD);

	@tests_files_sorted = sort(@tests_files);

	foreach $f (@tests_files_sorted)
	{
		unless ( ($f eq ".") || ($f eq "..") )
		{
			test_run($f)
		}
	}
}

print "abidos runing suit tests [v0.0.01]\n";
print "{\n";
all_tests;
print "}\n";

