#! /usr/bin/perl -w
####################################################################
# This script will construct a configuration block for Installshield
# out of the list of input header files.
####################################################################

use strict;

### Globals ########################################################

# Arguments are list of header files.
my @INC_FILES = @ARGV;

# Global hashish for header file groups.
my %INCDATA;
my ($NRBYTES, $NRFILES, $FILES) = (1, 2, 3);

# Detect if we have ``cygpath'' available.
my $CYGPATH = "cygpath -w";
`$CYGPATH /bin > /dev/null 2>&1`;
if ($? != 0) { $CYGPATH="echo"; }

### Process header files ###########################################

foreach (@INC_FILES) {
    my $inc_file = $_;
    my $subdir = "";
    $inc_file =~ /Inventor\/(\w+\/)?\w+\.h$/;
    if (defined($1)) { $subdir = $1; chop($subdir); }

    if (! defined($INCDATA{$subdir}{$NRBYTES})) {
        $INCDATA{$subdir}{$NRBYTES} = 0;
        $INCDATA{$subdir}{$NRFILES} = 0;
        $INCDATA{$subdir}{$FILES} = "";
    }

    $INCDATA{$subdir}{$NRBYTES} += (-s $inc_file);
    $INCDATA{$subdir}{$NRFILES}++;
    $INCDATA{$subdir}{$FILES} .= "$inc_file ";
}

### Dump Installshield template on stdout ##########################

my @SUBDIRS = keys(%INCDATA);
my $groupnr = 1;
foreach (@SUBDIRS) {
    print "Group${groupnr}Size=$INCDATA{$_}{$NRBYTES}\n";

    print "Group${groupnr}Files=$INCDATA{$_}{$NRFILES}\n";

    my $groupname;
    if ($_ eq "") { $groupname = "BASEHEADERS"; }
    else { $groupname = uc($_) . "HEADERS"; }
    print "Group${groupnr}Name=$groupname\n";

    print "Group${groupnr}Dir=<INSTALLDIR>\\include\\Inventor";
    if ($_ ne "") { print "\\$_"; }
    print "\n";

    print "Group${groupnr}Update=0\n"; # FIXME: should this be 1? 20001031 mortene.
    print "Group${groupnr}TargetOS=-1\n";

    my @files = split(/ /, $INCDATA{$_}{$FILES});
    my $j = 1;
    foreach(@files) {
        my $name = `$CYGPATH $_`;
        chomp($name);
        print "Group${groupnr}File${j}=$name\n";
        $j++;
    }

    $groupnr++;
}

### Successful completion ##########################################

exit(0);
