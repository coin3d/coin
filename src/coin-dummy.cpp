// This file is only present to trick the Automake setup to link with
// CXXLD instead of CCLD.
//
// Without it, the only source code file in this base directory is
// tidbits.c, which makes Automake set us up with C linking of
// libCoin. That has been found to at least cause problems with gcc
// 3.2 on Solaris.

static void dummy_cplusplus_function(void) { }
