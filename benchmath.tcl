#!/usr/bin/env tclsh

proc logmath {{range 1000}} {

    set val 0.0
    for {set i 1} {$i < $range} {incr i} {
        set val [expr {log($i)*log($i)+log($i)*log($i)+log($i)*log($i)+log($i)*log($i)+log($i)*log($i)+log($i)*log($i)+log($i)*log($i)+log($i)*log($i)+log($i)*log($i)+log($i)*log($i)+log($i)*log($i)+log($i)*log($i)+log($i)*log($i)+log($i)*log($i)+log($i)*log($i)+log($i)*log($i)+$val}]
    }
    return $val
}

proc log10math {{range 1000}} {

    set val 0.0
    for {set i 1} {$i < $range} {incr i} {
        set val [expr {log10($i)*log10($i)+log10($i)*log10($i)+log10($i)*log10($i)+log10($i)*log10($i)+log10($i)*log10($i)+log10($i)*log10($i)+log10($i)*log10($i)+log10($i)*log10($i)+log10($i)*log10($i)+log10($i)*log10($i)+log10($i)*log10($i)+log10($i)*log10($i)+log10($i)*log10($i)+log10($i)*log10($i)+log10($i)*log10($i)+log10($i)*log10($i)+$val}]
    }
    return $val
}

proc powmath {{range 1000}} {

    set val 0.0
    for {set i 1} {$i < $range} {incr i} {
        set val [expr {pow($i,0.1)*pow($i,0.1)+pow($i,0.1)*pow($i,0.1)+pow($i,0.1)*pow($i,0.1)+pow($i,0.1)*pow($i,0.1)+pow($i,0.1)*pow($i,0.1)+pow($i,0.1)*pow($i,0.1)+pow($i,0.1)*pow($i,0.1)+pow($i,0.1)*pow($i,0.1)+pow($i,0.1)*pow($i,0.1)+pow($i,0.1)*pow($i,0.1)+pow($i,0.1)*pow($i,0.1)+pow($i,0.1)*pow($i,0.1)+pow($i,0.1)*pow($i,0.1)+pow($i,0.1)*pow($i,0.1)+pow($i,0.1)*pow($i,0.1)+pow($i,0.1)*pow($i,0.1)+$val}]
    }
    return $val
}

set tv microseconds
if {$tcl_version < 8.5} { set tv clicks }

set val 0.0
puts "test for log()"
for {set i 0} {$i < 5} {incr i} {
    set tstart [clock $tv]
    set val [logmath 100001]
    set tstop [clock $tv]
    puts [format "% 2d %8.5f | %.15g" $i [expr {($tstop - $tstart)/1.e6}] $val]
}

puts "test for log10()"
for {set i 0} {$i < 5} {incr i} {
    set tstart [clock $tv]
    set val [log10math 100001]
    set tstop [clock $tv]
    puts [format "% 2d %8.5f | %.15g" $i [expr {($tstop - $tstart)/1.e6}] $val]
}

puts "test for pow()"
for {set i 0} {$i < 5} {incr i} {
    set tstart [clock $tv]
    set val [powmath 100001]
    set tstop [clock $tv]
    puts [format "% 2d %8.5f | %.15g" $i [expr {($tstop - $tstart)/1.e6}] $val]
}


