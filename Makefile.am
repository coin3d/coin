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


SUBDIRS = include data build src $(TOOLDIR) $(EXAMPLEDIR)

bin_SCRIPTS = coin-config
BUILT_SOURCES = coin-config
coin-config: coin-config.in

EXTRA_DIST = conf-macros/coin.m4

m4datadir = $(datadir)/aclocal
m4data_DATA = conf-macros/coin.m4


doxygen-doc:
	top_builddir=$(top_builddir) top_srcdir=$(top_srcdir) \
	doxygen $(top_srcdir)/docs/coin.doxygen
