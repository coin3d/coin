## Process this file with automake to generate Makefile.in.

noinst_LTLIBRARIES = libcaches.la

## Remove any linklibraries detected by configure, as they are not
## needed in the submodules -- only in the toplevel libCoin build.
LIBS = 

INCLUDES = -I$(top_srcdir)/include/

## All source files used when building this submodule, including all
## headers.

libcaches_la_SOURCES = \
	SoBoundingBoxCache.cpp \
	SoBoundingBoxCache.h \
	SoCache.cpp \
	SoCache.h \
	SoConvexDataCache.cpp \
	SoConvexDataCache.h \
	SoNormalCache.cpp \
	SoNormalCache.h \
	SoTextureCoordinateCache.cpp \
	SoTextureCoordinateCache.h

## Installation information.

libcachesincdir = $(includedir)/Inventor/caches

libcachesinc_HEADERS = \
	$(top_srcdir)/include/Inventor/caches/SoBoundingBoxCache.h \
	$(top_srcdir)/include/Inventor/caches/SoCache.h \
	$(top_srcdir)/include/Inventor/caches/SoConvexDataCache.h \
	$(top_srcdir)/include/Inventor/caches/SoNormalCache.h \
	$(top_srcdir)/include/Inventor/caches/SoTextureCoordinateCache.h


## FIXME: this is a temporary workaround for a bug in Automake. The
## bug is fixed in the latest Automake CVS, but not in any "official"
## release (?). Last checked with Automake 1.4a. 19990912 mortene.

install-libcachesincHEADERS: $(libcachesinc_HEADERS)
	@$(NORMAL_INSTALL)
	$(mkinstalldirs) $(DESTDIR)$(libcachesincdir)
	@list='$(libcachesinc_HEADERS)'; for p in $$list; do \
	  if test -f "$$p"; then d= ; else d="$(srcdir)/"; fi; \
	  f="`echo $$p | sed -e 's|^.*/||'`"; \
	  echo " $(INSTALL_DATA) $$d$$p $(DESTDIR)$(libcachesincdir)/$$f"; \
	  $(INSTALL_DATA) $$d$$p $(DESTDIR)$(libcachesincdir)/$$f; \
	done

uninstall-libcachesincHEADERS:
	@$(NORMAL_UNINSTALL)
	@list='$(libcachesinc_HEADERS)'; for p in $$list; do \
	  f="`echo $$p | sed -e 's|^.*/||'`"; \
	  echo " rm -f $(DESTDIR)$(libcachesincdir)/$$f"; \
	  rm -f $(DESTDIR)$(libcachesincdir)/$$f; \
	done
