
if BUILD_WITH_SUPERGLU
SUPERGLUDIR = superglu
else
SUPERGLUDIR =
endif

if BUILD_VRML97
VRML97DIR = vrml97/src
else
VRML97DIR =
endif

SUBDIRS = $(SUPERGLUDIR) $(VRML97DIR)

DIST_SUBDIRS = . $(SUPERGLUDIR) $(VRML97DIR)

## by setting DIST_SUBDIRS like this, "make dist" will include or exclude
## superglu from the source distribution depending on whether it is included
## in the source repository we've configured from.  [20011029 larsa]

