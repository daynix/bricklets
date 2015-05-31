#!/usr/bin/perl
#
 
# Copyright (c) 2013, Daynix Computing LTD (www.daynix.com)
# All rights reserved.
#
# Maintained by bricklets@daynix.com
#
# This file is a part of the Bricklets library, please see the wiki page
# on https://github.com/daynix/bricklets/wiki/Bricklets-library for more.
#
# This code is licensed under standard 3-clause BSD license.
# See file LICENSE supplied with this package for the full license text.

use strict;
use warnings;
use English;

##########################################################################
#
# IOMETER configuration
#

my $iometer_path_name     = "C:\\Program Files (x86)\\Iometer.org\\Iometer 2006.07.27\\Iometer.exe";
my $cfg_file_name         = ".Iometer_config_by_iomeasure.icf.tmp";
my $raw_results_file_name = ".Iometer_raw_results.tmp";

#
##########################################################################

##########################################################################
#
# Test suite configuration
#

my %ReadPercentages = ("R" => 100, "W" => 0);

my @BufferSizes =     ( 512, 1024, 2048, 4096, 8192, 16384, 32768, 65536 );

my %RandomnessPercentages = ( "Rand" => 100, "Seq" => 0 );

my $RunTimeH       =   0;
my $RunTimeM       =   0;
my $RunTimeS       =  30;
my $RampUpTimeS    =  10;
my $TestFileSizeMb = 512;

# Hash that defines which results from Iometer log to be present in the report
# Keys are result names as given in Iometer log
# Values are result names in this script's report
my @reported_results_values = ( { raw => "Access Specification Name", report => "Test parameters"      }, 
                                { raw => "MBps",                      report => "Throughput (MBps)"    }, 
                                { raw => "\% CPU Utilization",        report => "CPU Utilization (\%)" }
                              );

#
##########################################################################

sub show_help
{
    printf STDERR "IoMeasure.pl\n";
    printf STDERR "    This is an IO performance measurment script.\n";
    printf STDERR "    It performs standartized set of IO measurements for storage\n";
    printf STDERR "    device specified and provides consolidated report on tests performed.\n";
    printf STDERR "    \n";
    printf STDERR "    Usage: IoMeasure.pl <disk letter>: <results file name>\n";
    printf STDERR "    \n";
}

my $target_disk = shift   || (show_help && die "ERROR: Argument missing: target disk letter\n");
my $results_file = shift  || (show_help && die "ERROR: Argument missing: results file name\n");

sub build_access_specification
{
    my $mode = shift;
    my $read_percentage = shift;
    my $randomness_name = shift;
    my $randomness_percentage = shift;
    my $buff_size = shift;
    my $names_list_ref = shift;
    
    my $spec_name = "${mode}_${randomness_name}_${buff_size}";
    push(@$names_list_ref, $spec_name);
    
    my $result = <<END_OF_SPEC;
'Access specification name,default assignment
	$spec_name,DISK
'size,\%% of size,\%% reads,\%% random,delay,burst,align,reply
	$buff_size,100,$read_percentage,$randomness_percentage,0,1,0,0
END_OF_SPEC

    return $result;
}

sub build_access_specifications_list
{
    my $names_list_ref = shift;
    
    my $result_str;
    foreach my $mode (sort keys %ReadPercentages)
    {
        foreach my $randomness (sort keys %RandomnessPercentages)
        {
            foreach my $buff_size (@BufferSizes)
            {
                $result_str = $result_str.build_access_specification($mode, $ReadPercentages{$mode},
                                                                     $randomness, $RandomnessPercentages{$randomness},
                                                                     $buff_size, $names_list_ref);
            }
        }
    }

    chomp($result_str);
    return $result_str;
}

sub print_names_list
{
    my $names_list_ref = shift;
    my $result_str;
    
    foreach my $name (@$names_list_ref)
    {
      $result_str = $result_str."\t$name\n";
    }
    
    chomp($result_str);
    return $result_str;
}

sub build_config
{
    my @names_list = ();
    my $disk_file_size_sectors = $TestFileSizeMb * 1024 * 2; #Converting megabytes to sectors of 512 bytes
    open(CONFIG, "> $cfg_file_name") or die "Cannot open $cfg_file_name for write: $!";
    
    printf CONFIG <<IOMEASURE_END_OF_CONFIG;
Version 2006.07.27 
'TEST SETUP ====================================================================
'Test Description
	
'Run Time
'	hours      minutes    seconds
	$RunTimeH\t$RunTimeM\t$RunTimeS
'Ramp Up Time (s)
	$RampUpTimeS
'Default Disk Workers to Spawn
	1
'Default Network Workers to Spawn
	0
'Record Results
	ALL
'Worker Cycling
'	start      step       step type
	1          1          LINEAR
'Disk Cycling
'	start      step       step type
	1          1          LINEAR
'Queue Depth Cycling
'	start      end        step       step type
	1          32         2          EXPONENTIAL
'Test Type
	NORMAL
'END test setup
'RESULTS DISPLAY ===============================================================
'Update Frequency,Update Type
	0,WHOLE_TEST
'Bar chart 1 statistic
	Total I/Os per Second
'Bar chart 2 statistic
	Total MBs per Second
'Bar chart 3 statistic
	Average I/O Response Time (ms)
'Bar chart 4 statistic
	Maximum I/O Response Time (ms)
'Bar chart 5 statistic
	\%% CPU Utilization (total)
'Bar chart 6 statistic
	Total Error Count
'END results display
'ACCESS SPECIFICATIONS =========================================================
@{[ build_access_specifications_list(\@names_list) ]}
'END access specifications
'MANAGER LIST ==================================================================
'Manager ID, manager name
	1,LOCALHOST
'Manager network address
	127.0.0.1
'Worker
	Worker 1
'Worker type
	DISK
'Default target settings for worker
'Number of outstanding IOs,test connection rate,transactions per connection
	1,DISABLED,1
'Disk maximum size,starting sector
	$disk_file_size_sectors,0
'End default target settings for worker
'Assigned access specs
@{[ print_names_list(\@names_list) ]}
'End assigned access specs
'Target assignments
'Target
	$target_disk
'Target type
	DISK
'End target
'End target assignments
'End worker
'End manager
'END manager list
Version 2006.07.27 
IOMEASURE_END_OF_CONFIG

    close(CONFIG);
}

sub run_iometer
{
    die "ERROR: Iometer not found at path specified (\"$iometer_path_name\").\
Install Iometer or provide correct path (variable \"iometer_path_name\" in this script)"
        unless (-e $iometer_path_name);

    my @args = ($iometer_path_name, $cfg_file_name, $raw_results_file_name);
    (system(@args) == 0) or die "ERROR: Failed to execute Iometer: $!";
}

sub process_data_entry
{
    my $header_line = shift;
    my $data_line = shift;
    my $results_array_ref = shift;
    
    my @columns     = split(',', $header_line);
    my @data_values = split(',', $data_line);
    my %data_entry_hash;
    
    my $columns = @columns;
    
    while($columns)
    {
        $data_entry_hash{pop(@columns)} = pop(@data_values);
        $columns = @columns;
    }
    
    push(@$results_array_ref, \%data_entry_hash);
}

sub parse_results
{
    my $results_array_ref = shift;
    
    my $errors = 0;
    my $header_line;
    open(RAW_RES, "< $raw_results_file_name") or die "Cannot open $raw_results_file_name for read: $!";
    
    while(<RAW_RES>)
    {
        my $line = $_;
        if($line =~ /^ERROR\:/)
        {
          printf STDERR $line;
          $errors = 1;
          next;
        }
        if($line =~ /^\'Target Type/)
        {
          $header_line = $line;
          next;
        }
        if($line =~ /^ALL\,All/)
        {
          process_data_entry($header_line, $line, $results_array_ref);
          next;
        }
    }
    
    close(RAW_RES);
    if($errors > 0)
    {
      printf STDERR "One or more errors found during results processing";
    }
}

sub dump_results
{
    my $column_separator = " " x 5;
    my $results_array_ref = shift;
    open(RESULTS, "> $results_file") or die "Cannot open $results_file for write: $!";

    #Print header
    foreach my $value_names (@reported_results_values)
    {
        printf RESULTS ("%s$column_separator", $value_names->{report});
    }
    printf RESULTS "\n";

    #Print header separator
    foreach my $value_names (@reported_results_values)
    {
        printf RESULTS ("-" x length($value_names->{report}) . "$column_separator");
    }
    printf RESULTS "\n";

    #Print data
    foreach my $data_set (@$results_array_ref)
    {
        foreach my $value_names (@reported_results_values)
        {
          printf RESULTS ("%-".length($value_names->{report})."s$column_separator", $data_set->{$value_names->{raw}});
        }
        printf RESULTS "\n";
    }
    
    close(RESULTS);
    
    #Also dump to console for convenience
    printf RESULTS "\n";
    open(RESULTS, "< $results_file") or die "Cannot open $results_file for read: $!";
    print <RESULTS>;
    close(RESULTS);
}

sub remove_config
{
    (not -e $cfg_file_name) || unlink($cfg_file_name) || die "ERROR: Failed to remove temporary config file ($cfg_file_name): $!";
}

sub remove_raw_results
{
    (not -e $raw_results_file_name) || unlink($raw_results_file_name) || die "ERROR: Failed to remove temporary results file ($raw_results_file_name): $!";
}

my @results;

build_config;

# It is important to ensure there is no old raw results file
# because Iometer not overwrites but appends results
remove_raw_results;
run_iometer;
parse_results(\@results);
dump_results(\@results);

# Give IOMETER service some time to shut down
# otherwise we will be unable to delete temporary config file
sleep 5;

remove_config;
remove_raw_results;
