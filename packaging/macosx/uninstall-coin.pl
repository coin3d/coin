#! /usr/bin/perl

# Script to remove the complete Coin installation, as done by the
# binary installers for Coin-1.0.3, Coin-1.0.4, Coin-2.0.0 and/or 
# Coin-2.0.0.

# This script is based on uninstall-devtools.pl by Apple. It's a shame
# that the recommended/only way of uninstalling packages seems to be
# Perl hacking, but I figured that anything is better than having a
# binary installer with no uninstall function.

# To run this script, open a Terminal and type 
# sudo ./uninstall-coin.pl


use strict;
#use vars qw ($receipts_dir);

my $receipts_dir = "/Library/Receipts";

$| = 1;
die "ERROR: Must be run as root.\n" if ($< != 0);


sub remove_main_packages  {

    # These are the versions we had binary installers for.
    my @pkglist = ('Coin', 
                   'CoinTools',
                   'Coin-2.0.0',
                   'Coin-1.0.4',
                   'Coin-1.0.3');
    
    foreach (@pkglist) {
        
        my $pkgname = $_;
        my $pkgname_fullpath = "$receipts_dir/$pkgname.pkg";
        my $pkgname_bom_fullpath = "$pkgname_fullpath/Contents/Archive.bom";
        
        print("Removing package $pkgname.pkg.\n");
        
        # If the Package is not installed, continue. 
        next if (! -d "$pkgname_fullpath" );
        next if (! -f "$pkgname_bom_fullpath" );
        
        # 1. Delete all files listed in BOM.
        
        open (LSBOM, "/usr/bin/lsbom -l -f -p f '$pkgname_bom_fullpath' |") 
            or next;

        while (<LSBOM>) {
            chomp;
            m#^\.(/.*)$#;
            my $filename = $1;
            next if (!defined ($filename) || $filename eq "");
            
            remove_file ($filename);
        }
        close (LSBOM);
        
        # 2. Delete all empty directories listed in BOM.
        
        my $rooth = { };
        
        open (LSBOM, "/usr/bin/lsbom -d -p f '$pkgname_bom_fullpath' |") 
            or next;

        while (<LSBOM>) {
            chomp;
            m#^\.(/.*)$#;
            my $directory = $1;
            next if (!defined ($directory) || $directory eq "");
            if (-d $directory) {
                $rooth = add_directory_to_tree ($directory, $rooth);
            }
        }
        close (LSBOM);
        
        remove_empty_directories ($rooth, "/");
        
        # 3. Remove package receipts.
        remove_package_receipts ("$pkgname.pkg");
    }
}


sub add_directory_to_tree
{
    my $dir = shift;
    my $rooth = shift;
    my $p = $rooth;
    
    my @pathcomp = split /\//, $dir;
    
    foreach (@pathcomp) {
        my $cur_name = $_;
        if ($cur_name eq "" || !defined ($cur_name)) {
            $cur_name = "/";
        }
        if (!defined ($p->{"$cur_name"})) {
            $p->{$cur_name} = { };
        }
        $p = $p->{$cur_name};
    }
    return $rooth;
}


sub remove_empty_directories
{
    my $rooth = shift;
    my $path = shift;
    my $children = (scalar (keys %{$rooth}));
    my $dirs_remain = 0;
    
    if ($children > 0) {
        foreach my $dirname (sort keys %{$rooth}) {
            my $printpath;
            $printpath = "$path/$dirname";
            $printpath =~ s#^/*#/#;
            remove_empty_directories ($rooth->{$dirname}, "$printpath");
            $dirs_remain = 1 if (-d "$printpath");
        } 
    }
    

    # Sometimes we end up having a .DS_Store file left in the directory.
    # Kill it.

    if ($dirs_remain == 0) {
        remove_ds_store ("$path");
    }   
    remove_directory ("$path");
}

sub remove_file  
{
    my $file = shift;
    return if (!defined ($file) || $file eq "");
    return if (! -f $file && ! -l $file);
    
    if ((unlink $file) != 1)  {
        print ("ERROR: Encountered error while trying to remove \"$file\"\n");
    }   
}


sub remove_directory
{
    my $dir = shift;

    return if (!defined ($dir) || $dir eq "" || $dir eq "/" || $dir eq "/usr");
    return if (! -d $dir);
    
    rmdir $dir;
}

sub remove_package_receipts
{
    my $pkgname = shift;
    $pkgname =~ s#/##g;  # There shouldn't be any path seps in the pkg name...
    return if (!defined ($pkgname) || $pkgname eq "" 
               || $pkgname eq "." || $pkgname eq "..");
    
    my $pkgdir = "$receipts_dir/$pkgname";
    return if (!defined ($pkgdir) || $pkgdir eq "" || ! -d $pkgdir);
    
    my @rmcmd = ('/bin/rm', '-rf', "$pkgdir");
    system @rmcmd;
    my $retcode = $? >> 8;
    if ($retcode != 0) {
        print ("Warning: Problem removing \"$pkgdir\"\n");
    }
}

sub remove_ds_store {
    my $path = shift;
    my $filecount = 0;
    return if (!defined ($path) || $path eq "" || 
               $path eq "/" || $path eq "/usr");
    return if (! -f "$path/.DS_Store");
    
    open (LS, "/bin/ls -a '$path' |");
    while (<LS>) {
        chomp;
        next if (m#^\.$# || m#^\.\.$#);
        $filecount++;
    }
    close (LS);
    
    if ($filecount == 1 && -f "$path/.DS_Store") {
        remove_file ("$path/.DS_Store");
    }
}

remove_main_packages ();
print "Done. Have a nice day! :)\n";

