#
# Copyright (c) 2012, Daynix Computing LTD (www.daynix.com)
# All rights reserved.
#
# Maintained by bricklets@daynix.com
#
# This file is a part of the Bricklets library, please see the wiki page
# on https://github.com/daynix/bricklets/wiki/Bricklets-library for more.
#
# This code is licensed under standard 3-clause BSD license.
# See file LICENSE supplied with this package for the full license text.
#

# This script generates file list sections of VS project files
# Usage: dnx_list_files.pl <root> <config> <buildcfg> <solution> <cfg template> <cfg list template>
#
# The scripts reads VS project templates from standard input, looks for
# markers, replaces them with generated content and prints out the result
# to standard output.
#
# The set of files added to XML output is controled by
# text config file dnx_list_files.cfg.
#

use strict;
use File::Find;

# Internal script configuration
my $do_debug_trace = 0;

my @all_project_files = ();
my %build_configurations = ();

my @show_rules = ();
my @hide_rules = ();

my $project_root = shift || die "Missing directory name\n";
my $config_file = shift || die "Missing configuration file\n";
my $buildcfg_file = shift || die "Missing build configurations file\n";
my $solution_file = shift || die "Missing solution file\n";
my $build_cfg_template = shift || die "Missing configuration template\n";
my $build_cfg_list_template = shift || die "Missing configuration list template\n";

my $project_root_win = $project_root;
$project_root_win =~ s/\//\\/g;
my $project_root_regexp = path_to_regexp($project_root_win);

sub build_file_list()
{
  find({wanted => \&process_file_found, no_chdir => 1},"$project_root");
}

my $action_build = "build";
my $action_clean = "clean";
my $action_rebuild = "rebuild";
my $config_defines = "defines";
my $config_includes = "includes";
my $config_inc_dirs = "inc_dirs";

sub is_known_buildcfg_cfgtype($)
{
  my $cfgtype = shift;

  $cfgtype eq $action_build or
  $cfgtype eq $action_clean or
  $cfgtype eq $action_rebuild or
  $cfgtype eq $config_defines or
  $cfgtype eq $config_includes or
  $cfgtype eq $config_inc_dirs or
  return 0;

  return 1;
}

sub build_configurations_list()
{
  open(my $buildcfgFileHandle,$buildcfg_file) || die "Cannot open build configurations file ($buildcfg_file) for reading: $!";
  my $buildcfg_line_number = 0;

  while (my $buildcfg_line=<$buildcfgFileHandle>)
  {
    $buildcfg_line_number = $buildcfg_line_number +1;

    chomp($buildcfg_line);
    $buildcfg_line = trim($buildcfg_line);

    if (!$buildcfg_line)
    {
        next;
    }
    if ($buildcfg_line =~ /^\#.*/)
    {
        next;
    }
    if ($buildcfg_line =~ /^(.+)\.(\S+)\s*\=\s*(.+)$/)
    {
        my $cfg_name = trim($1);
        my $cfgtype = trim($2);
        my $cmd = trim($3);

        if(is_known_buildcfg_cfgtype($cfgtype))
        {
          $build_configurations{$cfg_name}{$cfgtype} = $cmd;
          next;
        }
    }

    die "Unrecognized line $buildcfg_file:$buildcfg_line_number: \"$buildcfg_line\"";
  }
  close($buildcfgFileHandle);
}

sub process_file_found()
{
  my $file_name = $File::Find::name;

  #Do not include directories into the search results
  return if(-d $file_name);

  #Fix slash types
  $file_name =~ s/\//\\/g;

  #Replace root with .\
  print_trace("Matching file \"$file_name\" to remove search root \"$project_root_regexp\"\n");
  $file_name =~ s/^$project_root_regexp(.*)/\.\\$1/;

  # Eliminate multiple slashes
  $file_name =~ s/\\+/\\/g;

  #Check exclude rules match
  map
  {
    my $exclude_rule = $_;

    print_trace("Matching file \"$file_name\" for exclude rule \"$exclude_rule\"\n");

    return if($file_name =~ /\.\\$exclude_rule/);

  } @hide_rules;

  #Check include rules match
  map
  {
    my $include_rule = $_;

    print_trace("Matching file \"$file_name\" for include rule \"$include_rule\"\n");

    if($file_name =~ /\.\\$include_rule/)
    {
       push @all_project_files, $file_name; 
       return;
    }
  } @show_rules;
}

sub parse_config_file()
{
  open(my $configFileHandle,$config_file) || die "Cannot open config file ($config_file) for reading: $!";
  my $config_line_number = 0;

  while (my $config_line=<$configFileHandle>)
  {
        chomp($config_line);
        $config_line = trim($config_line);
        $config_line_number = $config_line_number + 1;

        if (!$config_line)
        {
            print_trace("Empty line $config_line_number skipped\n");
            next;
        }
        if ($config_line =~ /^\#.*/)
        {
            print_trace("Comment line \"$config_line\" skipped\n");
            next;
        }
        if ($config_line =~ /^SHOW\s+[^\s]+/)
        {
            print_trace("Show line \"$config_line\" processed\n");
            $config_line =~ s/^SHOW\s+([^\s]+)/$1/;
            print_trace("Show rule \"$config_line\" processed\n");

            push @show_rules, $config_line;
            next;
        }
        if ($config_line =~ /^HIDE\s+[^\s]+/)
        {
            print_trace("Hide line \"$config_line\" processed\n");
            $config_line =~ s/^HIDE\s+([^\s]+)/$1/;
            print_trace("Hide rule \"$config_line\" processed\n");

            push @hide_rules, $config_line;
            next;
        }

        die "Unknown directive in $config_file:$config_line_number: \"$config_line\"";
    }
    close($configFileHandle);
}

my $vs10_files;
my $vs10_filters;
my $vs10_filterdefs;

sub dump_vs10_file($$)
{
  my $filedir = shift;
  my $filename = shift;

  $vs10_files = $vs10_files."\t<ClCompile Include=\"".trim_leading_dot("$filedir\\$filename")."\" />\n";
}

sub dump_vs10_filter($$)
{
  my $filedir = shift;
  my $filename = shift;

  $vs10_filters = $vs10_filters."\t<ClCompile Include=\"".trim_leading_dot("$filedir\\$filename")."\">\n";
  if ($filedir ne ".")
  {
    $vs10_filters = $vs10_filters."\t\t<Filter>".trim_leading_dot($filedir)."</Filter>\n";
  }
  $vs10_filters = $vs10_filters."\t</ClCompile>\n";
}

sub dump_vs10_filterdef($)
{
  my $filedir = shift;

  if ($filedir ne ".")
  {
    $vs10_filterdefs = $vs10_filterdefs."\t<Filter Include=\"".trim_leading_dot($filedir)."\">\n".
                                        "\t\t<UniqueIdentifier>". get_guid() . "</UniqueIdentifier>\n".
                                        "\t</Filter>\n";
  }
}

sub enum_vs10_files($$$$)
{
  no strict 'refs';

  my $curr_dir = shift;
  my $dir_callback = shift;
  my $file_callback = shift;
  my $filelist = shift;

  my $curr_dir_regexp = path_to_regexp($curr_dir);
  print_trace("Processing results list directory \"$curr_dir\", regexp \"$curr_dir_regexp\"\n");
  print_trace("Current results list @{ $filelist }\n");

  my @curr_dir_list = grep(/^$curr_dir_regexp\\.*/, @{ $filelist });

  &{$dir_callback}($curr_dir) if $dir_callback;

  my $processed_entry_regexp;

  while(@curr_dir_list)
  {
    my $curr_entry_full = $curr_dir_list[0];
    my $curr_entry = $curr_dir_list[0];
    $curr_entry =~ s/^$curr_dir_regexp\\(.*)/$1/;

    print_trace("Processing results list entry $curr_entry\n");

    if($curr_entry !~ m/\\/)
    {
       #This is a file, process it and remove from list
       &{$file_callback}($curr_dir, $curr_entry) if $file_callback;
       $processed_entry_regexp = path_to_regexp("\^$curr_dir\\$curr_entry\$");
    }
    else
    {
       #This is a folder, go into recursion
       my $sub_dir = $curr_entry;
       $sub_dir =~ s/^(.*?)\\.*/$1/;

       enum_vs10_files("$curr_dir\\$sub_dir", $dir_callback, $file_callback, \@curr_dir_list);

       $processed_entry_regexp = path_to_regexp("\^$curr_dir\\$sub_dir\\");
    }

    @curr_dir_list = grep(!/$processed_entry_regexp/, @curr_dir_list);
  }
}

sub vs10_file_dumper($$)
{
  my $filedir = shift;
  my $filename = shift;

  dump_vs10_file($filedir, $filename);
  dump_vs10_filter($filedir, $filename);
}

sub vs10_dir_dumper($)
{
  my $filedir = shift;

  dump_vs10_filterdef($filedir);
}

sub vs10_do_enum($)
{
  my $start_dir = shift;

  $vs10_files = "";
  $vs10_filters = "";
  $vs10_filterdefs = "";

  enum_vs10_files($start_dir, "vs10_dir_dumper", "vs10_file_dumper", \@all_project_files);
}

sub dump_vs10_files()
{
   print($vs10_files);
}

sub dump_vs10_filters()
{
   print($vs10_filters);
}

sub dump_vs10_filter_defs()
{
   print($vs10_filterdefs);
}

my $guid_ctnr = 0;

sub get_guid($)
{
  return sprintf("{5a264196-f8b1-9f13-d23b-%04x%08x}", $guid_ctnr++, rand(0xFFFFFFFF));
}

sub trim_leading_dot($)
{
  my $path = shift;
  $path =~ s/^\.\\//;
  return $path;
}

sub clean_vs10_solution()
{
  # This function cleans out build configurations list from solution
  # This forces VS to rebuild solution configurations list
  # based on projects configuration list
  # If this step is missing VS will leave solution configurations
  # list as-is even if project configurations list changed

  # If there is no solution - nothing to do
  # VS will create a new solution with recent build configurations
  open SLNFILE, "+< $solution_file" or return;

  my $prev_read_pos = 0;

  while (<SLNFILE>) {
    my $line=$_;
    my $chomped_line = $line;
    chomp($chomped_line);
    $chomped_line = trim($chomped_line);

    if($chomped_line eq "Global")
    {
      truncate(SLNFILE, $prev_read_pos) || die "Failed to truncate solution: $!";
      last;
    }
    $prev_read_pos = tell(SLNFILE);
  }
  close CFGTPL;
}

sub dump_vs10_build_configurations($)
{
  my $template = shift;
  my $cfgname;
  my $cfgtypes = {};

  while ( ($cfgname, $cfgtypes) = each %build_configurations ) {
    open CFGTPL, "$template" or die $!;
    while (<CFGTPL>) {
      my $line=$_;
      $line =~ s/__CFG_NAME__/$cfgname/g;
      $line =~ s/__CFG_BUILD__/$cfgtypes->{$action_build}/g;
      $line =~ s/__CFG_CLEAN__/$cfgtypes->{$action_clean}/g;
      $line =~ s/__CFG_REBUILD__/$cfgtypes->{$action_rebuild}/g;
      $line =~ s/__CFG_DEFINES__/$cfgtypes->{$config_defines}/g;
      $line =~ s/__CFG_INCLUDES__/$cfgtypes->{$config_includes}/g;
      $line =~ s/__CFG_INC_DIRS__/$cfgtypes->{$config_inc_dirs}/g;
      print $line;
    }
    close CFGTPL;
  }
}

sub print_trace($)
{
    print(shift) if($do_debug_trace);
}

sub print_log($)
{
   printf(STDERR shift."\n");
}

sub trim($)
{
    my $string = shift;
    $string =~ s/^\s+//;
    $string =~ s/[\s\r]+$//;
    return $string;
}

sub path_to_regexp($)
{
  my $path = shift;
  $path =~ s/([\:\\\/\.\,\[\]\+\-\*])/\\$1/g;
  return $path;
}

print_log("Visual Studio project generator started...");
print_log("Parsing config file...");

parse_config_file();
map { print_trace("SHOW: $_\n"); } @show_rules;
map { print_trace("HIDE: $_\n"); } @hide_rules;

print_log("DONE");

print_log("Building file list...");
build_file_list();
print_log("DONE");

print_log("Building configurations list...");
build_configurations_list();
print_log("DONE");

print_log("Enumerating files...");
vs10_do_enum(".");
print_log("DONE");

print_log("Generating project...");

while (<STDIN>)
{
    my $line = $_;
    my $chomped_line = $line;
    chomp($chomped_line);

    my @temp_files_list = @all_project_files;

    if(trim($chomped_line) eq "__VS10_FILES__")
      { dump_vs10_files(); }
    elsif(trim($chomped_line) eq "__VS10_FILTER__")
      { dump_vs10_filters(); }
    elsif(trim($chomped_line) eq "__VS10_DEFS__")
      { dump_vs10_filter_defs(); }
    elsif(trim($chomped_line) eq "__VS10_CFGS__")
      { dump_vs10_build_configurations($build_cfg_template);
        clean_vs10_solution(); }
    elsif(trim($chomped_line) eq "__VS10_CFGS_LIST__")
      { dump_vs10_build_configurations($build_cfg_list_template); }
    else
      { print $line; }

    @all_project_files = @temp_files_list;
}

print_log("DONE");

print_log("Visual Studio project generation done");
