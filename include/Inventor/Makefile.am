## Process this file with automake to generate Makefile.in.

BUILT_SOURCES = \
	SbBasic.h

SUBDIRS = lock system

SbBasic.h: $(srcdir)/SbBasic.h.in
	@( cd $(top_builddir); $(MAKE) include/Inventor/SbBasic.h )

libCoinincdir = $(includedir)/Inventor
libCoininc_HEADERS = \
	SbBasic.h

