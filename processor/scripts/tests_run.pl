#!/usr/bin/perl -w
#
# autor:Manuel Hevia
# description:
#  scripts/tests_run.pl src/abidos test/ test_includes/ test_out/ .abidos
#
#-------------------------------------------
use strict;
use File::Copy;

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
  my $url_hash = shift;
  my $log_file = shift;

  print " [$f]->";
  my $command = "$executable_with_dir --includes $test_includes_dir --out_dir $tests_dir_out --test_all_tokens_consumed_flag --test_original --ts_show --verbose $tests_dir$f > ${tests_dir_out}out_$f.txt";
  system($command);
  print $log_file "  $command\n";

  my $result = is_test_ok("${tests_dir_out}out_$f.txt");

  if( $result eq "ok" ) {
    print " [$result]";
  } else {
    print " [$result] <--##\n";
  }

	# i want a dot file for each test file

	copy("${tests_dir_out}files_output.dot","${tests_dir_out}out_$f.dot");

  file_put_urls($tests_dir_out, $f, $url_hash, $log_file);

  system("perl ../scripts/trace_prune.pl ${tests_dir_out}trace_${f}.gv >> ${tests_dir_out}prune.log");
  system("perl ../scripts/trace_prune.pl ${tests_dir_out}trace_${f}_urls.gv >> ${tests_dir_out}prune.log");

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
  my $url_hash = shift;
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
      $result = test_run_tokens_consumed($executable_with_dir, $tests_dir, $test_includes_dir, $tests_dir_out, $f, $url_hash, $log_file);
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

sub create_url_hash
{
  my $abidos_working_dir = shift;
  my $url_hash = shift;
  my $log_file = shift;

  my $result = "fail";
  my $method = "";

  my $f = "${abidos_working_dir}files_output.dot";

  open(f_in,"< $f")||die("error open < $f");
  my @raw_data=<f_in>;

  foreach my $l (@raw_data)
  {
    chomp($l);
    $_= $l;

    #
    # jump the first token because is the name of the class
    #
    #
    if( $_ =~ /URL="c_parser_descent\[[^\]]*\];([^"]*)"/ )
		{
      $method = $1.";";
			$result = "ok";

      while ($method =~ m/([^\[]*)\[([^\]]*)\];/g) {
        $url_hash->{$1} = $2;
      }

      close(f_in);
      return $result;
		}
  }

  close(f_in);
  return $result;
}

sub file_put_urls
{
  my $tests_dir_out = shift;
  my $f = shift;
  my $url_hash = shift;
  my $log_file = shift;

  my $method = "";
  my $method_url = "";

  my $f_in  = "$tests_dir_out/trace_$f.gv";
  my $f_out = "$tests_dir_out/trace_${f}_urls.gv";

  open(f_in,"< $f_in")||die("error open < $f_in");
  my @raw_data=<f_in>;

  open(f_out,"> $f_out")||die("error open < $f_out");

  foreach my $l (@raw_data)
  {
    chomp($l);
    $_= $l;

    if( $_ =~ /_[\d]+_([^\[]+)\[label=/){
      if( exists( $url_hash->{$1}) ) {
        $method = $1;
        $method_url = $url_hash->{$method};
        $l =~ s/\[label=/\[URL="${method}[$method_url]", label=/;
		    print f_out "$l\n";
      }else{
        print f_out "$l\n";
      }
		}else{
      print f_out "$l\n";
		}
  }

  close(f_in);
  close(f_out);
}

sub main
{
  my $num_args = $#ARGV;

  if ($num_args != 4) {
    print "\n need <executable_with_dir> <tests_dir> <test_includes_dir> <tests_dir_out>\n";
    return;
  }

  my $executable_with_dir = $ARGV[0];
  my $tests_dir           = $ARGV[1];
  my $test_includes_dir   = $ARGV[2];
  my $tests_dir_out       = $ARGV[3];
  my $abidos_working_dir  = $ARGV[4];

  my %url_hash=();

#  flex_execute($ARGV[0], $ARGV[1]);
  print "abidos runing suit tests [$version] \n";
  my $result_total = 0;
  print "{\n";
  print " executable_with_dir [$executable_with_dir]\n";
  print " test_dir            [$tests_dir]\n";
  print " test_includes_dir   [$test_includes_dir]\n";
  print " test_out dir        [$tests_dir_out]\n";
  print " abidos_working_dir  [$abidos_working_dir]\n";

  system ("$executable_with_dir --test_all_tokens_consumed_flag --no_std --out_dir $abidos_working_dir --loader ${abidos_working_dir}/files_input") == 0 or die "system failed: $?";

  open( log_file, ">${tests_dir_out}test_out.log") or die $!;
  print log_file "abidos runing suit tests [$version]\n";
  print log_file "{\n";
  create_url_hash($abidos_working_dir, \%url_hash, *log_file);
  $result_total = all_tests($executable_with_dir, $tests_dir, $test_includes_dir, $tests_dir_out, \%url_hash, *log_file);

  print log_file "}\n";
  close(log_file);

  print "}\n";
  exit($result_total);
}

main();

