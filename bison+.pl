#!/usr/bin/perl
#
# Perl script to convert a bison generated parser into a thread-safe
# C++ parser
#

use strict;
use Getopt::Long;

my $debug = 0;

# Main
{
  my ($input_file, $class_name, $no_lines, $cpp_file, $h_file);

  GetOptions(
    "input=s" => \$input_file,
    "class=s" => \$class_name,
    "no-lines" => \$no_lines,
    "cpp=s" => \$cpp_file,
    "hdr=s" => \$h_file,
  );

  usage() if (!$input_file || !$class_name || !$cpp_file || !$h_file);

  debugout("Running bison...\n");

  unlink("y.tab.c");
  my @runyacc = ();
  push(@runyacc, "bison");
  push(@runyacc, "-y");
  push(@runyacc, "-l") if (defined($no_lines));
  push(@runyacc, $input_file);
  my $yaccstat = system(@runyacc);
  die "@runyacc failed.\n" if ($yaccstat != 0);

  open(YTABC, "<y.tab.c") || die("Could not open y.tab.c for reading: $!");
  open(HFILE, ">$h_file") || die("Could not open $h_file for writing: $!");
  open(CPPFILE, ">$cpp_file") || die("Could not open $cpp_file for writing: $!");

  my $state = 0;
  # Process y.tab.c and write .h and .cpp
  my $ytab = '';
  while (my $line = <YTABC>)
  {
    $ytab .= $line;
  }

  my $headerdef = '';
  my $classdef = '';
  my $constructor_init = '';
  $headerdef = "$1\n" if ($ytab =~ s/\/\*\s*headerdef\s*\n?(.*?)\*\///s);
  $classdef = $1 if ($ytab =~ s/\/\*\s*classdef\s*\n?(.*?)\*\///s);
  $constructor_init = $1 if ($ytab =~ s/\/\*\s*constructor_init\s*\n?(.*?)\n?\*\///s);

  my $class_protected = '';
  my $class_public = '';

  my $includes_found = 0;
  my $h_include_written = 0;
  my $cond_level = 0;
  my $to_class = 0;
  my $first_func = 1;

  my @lines = split(/\n/, $ytab);
  my $line_index = 0;
  my $curr_line = 0;
  my $prev_line = 0;
  while ($line_index < scalar(@lines))
  {
    $prev_line = $curr_line;
    $curr_line = $line_index;
    my $line = $lines[$line_index];
    $line_index++;
    while ($line =~ s/\s*\\\s*$/ / || $line =~ /^\s*static\s+void\s*$/ || $line =~ /^\s*int\s*$/)
    {
      $line .= ' ' . $lines[$line_index];
      $line_index++;
    }

    debugout("state=$state, define=$cond_level, first_func=$first_func\n");
    #debugout("  $line\n");
    if ($state == 0)
    {
      # beginning
      if ($line =~ /^\s*\#define/)
      {
        $state = 1;
        $line_index = $curr_line;
        next;
      }
      print CPPFILE "$line\n";
    }
    elsif ($state == 1)
    {
      # tokens to go into the class
      if ($line =~ /^\s*\/\*\s*cppdef/)
      {
        $state = 2;
        next;
      }
      # Bypass defines for tokens, they are in the token enum
      next if ($line =~ /^\s*\#define/);

      $class_public .= "$line\n";
    }
    elsif ($state == 2)
    {
      # CPP header
      if ($line =~ /^\s*\#include/)
      {
        $includes_found = 1;
      }
      elsif ($includes_found && !$h_include_written)
      {
        $h_include_written = 1;
        print CPPFILE "#include \"$h_file\"\n";
      }

      if ($line =~ /\#if\s*!\s*defined\s*YYSTYPE/)
      {
        debugout("Found YYSTYPE...\n");
        $state = 3;
        $class_public .= "  $line\n";
        next;
      }
      print CPPFILE "$line\n";
    }
    elsif ($state == 3)
    {
      # writing YYSTYPE
      if ($line =~ /^\s*\#endif\s*$/)
      {
        debugout("Found end of YYSTYPE...\n");
        $state = 4;
      }
      $class_public .= "  $line\n";
    }
    elsif ($state == 4)
    {
      # various definitions into header file
      if ($line =~ /^\s*static\s+const\s+yytype_uint8\s+yytranslate/)
      {
        debugout("Found end of header defs...\n");
        $state = 5;
        $line_index = $curr_line;
        next;
      }
      if (($line =~ /^\s*\#ifn?def/ || $line =~ /^\s*\#if\s+/))
      {
        $cond_level++;

        # Check if this define needs to be in the class
        if ($cond_level == 1)
        {
          $to_class = $line =~ /yyoverflow/;
        }
      }
      if (($cond_level > 0 && !$to_class) || ($cond_level == 0 && $line =~ /^\s*\#define/))
      {
        $headerdef .= "$line\n";
      }
      else
      {
        $class_protected .= "  $line\n";
      }
      if ($line =~ /^\s*\#endif/)
      {
        $cond_level--;
        die ("#define underflow\n") if ($cond_level < 0);
      }
    }
    elsif ($state == 5)
    {
      #if ($line =~ s/^\s*static\s+const\s+yytype\_(.*?)\s+(.*?)\[\] =/static const yytype_$1 ${class_name}::$2\[\] =/)
      #{
      #  $class_protected .= "  static const yytype_$1 $2\[\];\n";
      #}
      if ($line =~ s/^\s*static\s+void\s+(.*)$/void ${class_name}::$1/)
      {
        my $func_name = $1;
        #print "** Prev: $lines[$prev_line]\n** Line: $line\n** Next: $lines[$line_index]\n";
        my $pre_cond = '';
        my $succ_cond = '';
        $pre_cond = get_pre_cond($prev_line, \@lines) if ($first_func);
        $first_func = 0;
        $succ_cond = get_succ_cond($line_index, \@lines);

        my ($params_lines, $linecount) = get_param_lines($line_index, \@lines);
        $line_index += $linecount;

        $class_protected .= "$pre_cond  void $func_name\n$params_lines;\n$succ_cond";
      }
      # int function (...)
      elsif ($line =~ s/^\s*int ([\w_]*)\s*\((.*)\)/int ${class_name}::$1($2)/)
      {
        my ($func_name, $func_params) = ($1, $2);
        my $pre_cond = '';
        my $succ_cond = '';
        $pre_cond = get_pre_cond($prev_line, \@lines) if ($first_func);
        $first_func = 0;
        $succ_cond = get_succ_cond($line_index, \@lines);

        my ($params_lines, $linecount) = get_param_lines($line_index, \@lines);
        $line_index += $linecount;

        # Don't write function declaration without a body, they're just forward declarations
        next if ($line =~ /;/);

        #print "** Prec: $pre_cond\n** Line: $line\n** Succ: $succ_cond\n";
        $class_protected .= "$pre_cond  int $func_name($func_params);\n$succ_cond";
      }
      elsif ($line =~ /^\s*$/)
      {
        $first_func = 1;
      }

      # Variable definitions
      if ($line =~ /^\s*(int|YYSTYPE)\s+(\w+)\s*;\s*$/)
      {
        $class_protected .= "  $1 $2;\n";
        next;
      }

      print CPPFILE "$line\n";
    }
    else
    {
      die("Undefined state: $state");
    }
  }

  $class_public .= qq|  virtual int yylex() = 0;
  virtual void yyerror(char *) = 0;
|;

  # Write header file

  $class_public =~ s/\s*\/\*.*?\*\///gs;
  $class_protected =~ s/\s*\/\*.*?\*\///gs;

  $class_public =~ s/\n{2,}/\n/gs;
  $class_protected =~ s/\n{2,}/\n/gs;

  print HFILE qq|\
#ifndef ${class_name}_h
#define ${class_name}_h

$headerdef

class $class_name
{
$classdef
public:
$class_public
protected:
$class_protected
|;

  print HFILE "public:\n  ${class_name}() : $constructor_init {}\n" if ($constructor_init);

  print HFILE qq|\
};
#endif ${class_name}_h;
|;

  close(CFILE);
  close(HFILE);
}

sub get_param_lines($$)
{
  my ($line_no, $lines_ref) = @_;

  my $data = '';
  my $line_count = 0;

  while ($lines_ref->[$line_no] =~ /;\s*$/)
  {
    $data .= $lines_ref->[$line_no] . "\n";
    $line_count++;
    $line_no++;
  }
  return ($data, $line_count);
}

sub get_pre_cond($$)
{
  my ($line_no, $lines_ref) = @_;

  my $data = '';

  my $cond_line = $line_no;
  while ($lines_ref->[$cond_line] =~ /^\s*\#/)
  {
    my $line = $lines_ref->[$cond_line];
    my $line_index = $cond_line + 1;
    while ($line =~ s/\s*\\\s*$/ / || $line =~ /^\s*static\s+void\s*$/)
    {
      $line .= ' ' . $lines_ref->[$line_index];
      $line_index++;
    }

    $data = $line . "\n$data";

    $cond_line--;
  }
  return $data;
}

sub get_succ_cond($$)
{
  my ($line_no, $lines_ref) = @_;

  my $data = '';

  my $cond_line = $line_no;
  while ($lines_ref->[$cond_line] !~ /^\s*\#/ && $lines_ref->[$cond_line] !~ /^\s*\{/)
  {
    $cond_line++;
  }
  while ($lines_ref->[$cond_line] =~ /^\s*\#/)
  {
    my $line = $lines_ref->[$cond_line];
    my $line_index = $cond_line + 1;
    while ($line =~ s/\s*\\\s*$/ / || $line =~ /^\s*static\s+void\s*$/)
    {
      $line .= ' ' . $lines_ref->[$line_index];
      $line_index++;
    }
    $data .= $line . "\n";
    $cond_line++;
  }
  return $data;
}

sub write_class($$$$$$)
{
}

sub close_class()
{
  print HFILE "}\n";
}

sub usage()
{
  print qq|
Usage: $0 [--help] [--no-lines] --input infile --class ClassName --cpp file.cpp --hdr file.h
|;
  exit;
}

sub debugout($)
{
  my ($str) = @_;

  print $str if ($debug);
}



