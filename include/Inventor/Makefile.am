## Process this file with automake to generate Makefile.in.

SUBDIRS = lock system

BUILT_SOURCES = \
	SbBasic.h

DISTCLEANFILES = \
	SbBasic.h

EXTRA_DIST = \
	SbBasic.h.in

SbBasic.h: $(srcdir)/SbBasic.h.in
	@( cd $(top_builddir); $(MAKE) built-sources )

libCoinincdir = $(includedir)/Inventor
nodist_libCoininc_HEADERS = \
	SbBasic.h

