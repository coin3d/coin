## Process this file with automake to generate Makefile.in.

BUILT_SOURCES = \
	inttypes.h

DISTCLEANFILES = \
	inttypes.h

EXTRA_DIST = \
	inttypes.h.in \
	gl.h

inttypes.h: $(srcdir)/inttypes.h.in $(top_builddir)/config.status
	@( cd $(top_builddir); $(MAKE) include/Inventor/system/inttypes.h )

# header installation information

systemincdir = $(includedir)/Inventor/system
nodist_systeminc_HEADERS = \
	inttypes.h

