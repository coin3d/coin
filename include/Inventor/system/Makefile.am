## Process this file with automake to generate Makefile.in.

BUILT_SOURCES = \
	inttypes.h

DISTCLEANFILES = \
	inttypes.h

EXTRA_DIST = \
	inttypes.h.in

inttypes.h: $(srcdir)/inttypes.h.in
	@( cd $(top_builddir); $(MAKE) built-sources )

# header installation information

systemincdir = $(includedir)/Inventor/system
nodist_systeminc_HEADERS = \
	inttypes.h

