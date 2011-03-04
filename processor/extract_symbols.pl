#!/usr/bin/perl
#
# autor:Manuel Hevia
# description: transform de ouput rspec of
#    selenium-id to rsel
#
#-------------------------------------------
sub p1
{
  
  open(f_in,"< parser.c")||die("error open < file");
  @raw_data=<f_in>;
  open(f_out,"> tokens.h")||die("error open > file");

  print f_out "#ifndef tokens_h\n";
  print f_out "#define tokens_h\n";
  print f_out "static const char *const yytokens[] =\n";
  print f_out "{\n";
  
  $i = 0;

  for( $i = 0; $i < 258; $i++)
  {
    if($i < 33 or $i > 126)
    {
      print f_out "  \"$i UNDEFINED\",\n";
    }
    else
    {
      $c = chr($i);
      if($c eq "\"")
      {
        print f_out "  \"$i->\\\"\",\n";
      }
      elsif ($c eq "\\")
      {
        print f_out "  \"$i->\\\\\",\n";
      }
      else
      {
        print f_out "  \"$i->$c\",\n";
      }
    }
  }

  $yytyytokentype_inside = 0;
  foreach $l (@raw_data)
  {
        chomp($l);
        $_= $l;
        if ( $_ =~ /enum yytokentype {/ )
        {
          $yytyytokentype_inside = 1;                
        }

        if ($yytyytokentype_inside == 1)
        {
          if ( $_ =~ /}/ )
          {
            $yytyytokentype_inside = 2;
          }
          else
          {
            if ( $_ =~ /=/ )
            {
              s/([\S]+) = [\S]+/"$1",/g;
              print f_out "$_\n";
            }            
          }
        }
  } 

  if( $yytyytokentype_inside == 2)
  {
    print f_out "  0\n";
    print f_out "};\n";
    print f_out "#endif\n";
  }

  close(f_in);
  close(f_out);
}

p1;
