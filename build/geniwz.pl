#! /usr/bin/perl -w
####################################################################
# This script will construct a configuration block for Installshield
# out of the list of input header files.
####################################################################

use strict;

### Globals ########################################################

# Detect if we have ``cygpath'' available.
my $CYGPATH = "cygpath -w";
`$CYGPATH /bin > /dev/null 2>&1`;
if ($? != 0) { $CYGPATH="echo"; }

# 1st arg.
my $VERSION=$ARGV[0];
shift(@ARGV);
# 2nd arg.
my $DLLFILE=`$CYGPATH $ARGV[0]`;
chomp($DLLFILE);
shift(@ARGV);
# 3rd arg.
my $LIBFILE=`$CYGPATH $ARGV[0]`;
chomp($LIBFILE);
shift(@ARGV);

# Remaining arguments is a list of all the header files.
my @INC_FILES = @ARGV;

# Global hashish for header file groups.
my %INCDATA;
my ($NRBYTES, $NRFILES, $FILES) = (1, 2, 3);

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

print "
[InstallShield Wizard]
iDate=0
iTime=0
Flag=fe000000
ISX.EXE Size=679936
ISX.EXE Date=11:21:52AM  4/8/1999
ISX.EXE Ver=2.1.2.62
SETUP.EXE Size=45312
SETUP.EXE Date=9:12:24AM  3/23/1999
SETUP.EXE Ver=3.0.111.0
SETUP.INS Size=81342
SETUP.INS Date=11:26:40AM  4/8/1999
SETUP.INS Ver=Not available
_INST16.EX_ Size=283522
_INST16.EX_ Date=9:12:22AM  3/23/1999
_INST16.EX_ Ver=Not available
_INST32I.EX_ Size=294079
_INST32I.EX_ Date=9:12:22AM  3/23/1999
_INST32I.EX_ Ver=Not available
ISDEPEND.INI Size=294079
ISDEPEND.INI Date=9:12:22AM  3/23/1999
ISDEPEND.INI Ver=Not available
SWDEPEND.INI Size=294079
SWDEPEND.INI Date=9:12:22AM  3/23/1999
SWDEPEND.INI Ver=Not available
ICOMP.EXE Size=119808
ICOMP.EXE Date=9:12:24AM  3/23/1999
ICOMP.EXE Ver=3.00.062
SPLIT.EXE Size=90624
SPLIT.EXE Date=9:12:24AM  3/23/1999
SPLIT.EXE Ver=3.00.060
PACKLIST.EXE Size=87552
PACKLIST.EXE Date=9:12:24AM  3/23/1999
PACKLIST.EXE Ver=3.00.060
Version=2.12
DevTool=Generic Windows
Platform=Win32
TabsVisit=1111111111111111100011
LangNum=451
LangDir=LANG\\ENG\\

[VisualDesign]
AppName=Coin v$VERSION
AppExe=
AppExeFile=
Version=$VERSION
Company=Systems in Motion
Title=Coin v$VERSION Installation
TitleType=1
BackgrndBmp=
BackgrndAlign=4
Backgrnd=1
BackgrndColor=10
Uninstall=1
UninstallChecked=1
Silent=1
SmsMode=0

[Types]
Types=1
Type1Components=1
Type1Name=Complete
Type1ComponentList=1 
";

my @SUBDIRS = keys(%INCDATA);
my $numgroups = scalar(@SUBDIRS) + 2;

print "
[Components]
Components=1
Component1Groups=$numgroups
Component1Name=Coin Library SDK
Component1Description=Execution environment and development
Component1GroupList=";

my $i = 1;
while ($i <= $numgroups) { print "$i "; $i++; }

my $dllsize = (-e $DLLFILE) ? (-s $DLLFILE) : 0;
my $libsize = (-e $LIBFILE) ? (-s $LIBFILE) : 0;

print "

[Groups]
Groups=$numgroups
Group1Size=$dllsize
Group1Files=1
Group1Name=DLL
Group1Dir=<WINSYSDIR>
Group1Update=0
Group1TargetOS=-1
Group1File1=$DLLFILE
Group2Size=$libsize
Group2Files=1
Group2Name=LIB
Group2Dir=<INSTALLDIR>\\libs
Group2Update=0
Group2TargetOS=-1
Group2File1=$LIBFILE
";


my $groupnr = 3;
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

print "
[Sequence]
DestinationLocationDir=<ProgramFilesDir>\\SIM\\Coin
SelectProgramFolderName=Coin
DestinationLocation=1
ProgressBar=1

[Uninstall]
UserUninstall=0
";

### Successful completion ##########################################

exit(0);
