## Process this file with automake to generate Makefile.in.

if BUILD_EXAMPLES
EXAMPLEDIR = examples
else
EXAMPLEDIR =
endif

if BUILD_TOOLS
TOOLDIR = tools
else
TOOLDIR = 
endif

SUBDIRS = src $(TOOLDIR) $(EXAMPLEDIR)


doxygen-doc:
	top_builddir=$(top_builddir) top_srcdir=$(top_srcdir) \
	doxygen $(top_srcdir)/docs/coin.doxygen
