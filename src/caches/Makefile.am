## Process this file with automake to generate Makefile.in.

if HACKING_DYNAMIC_MODULES
lib_LTLIBRARIES = libcachesLINKHACK.la
else
noinst_LTLIBRARIES = libcaches.la
endif

## Remove any linklibraries detected by configure, as they are not
## needed in the submodules -- only in the toplevel libCoin build.
LIBS = 

INCLUDES = -I$(top_srcdir)/include -I$(top_builddir)/include

## All source files used when building this submodule, including all
## headers.

ALLSOURCES = \
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

libcaches_la_SOURCES = $(ALLSOURCES)
libcachesLINKHACK_la_SOURCES = $(ALLSOURCES)

## Installation information.

libcachesincdir = $(includedir)/Inventor/caches
libcachesLINKHACKincdir = $(includedir)/Inventor/caches

ALLINSTHEADERS = \
	$(top_srcdir)/include/Inventor/caches/SoBoundingBoxCache.h \
	$(top_srcdir)/include/Inventor/caches/SoCache.h \
	$(top_srcdir)/include/Inventor/caches/SoConvexDataCache.h \
	$(top_srcdir)/include/Inventor/caches/SoNormalCache.h \
	$(top_srcdir)/include/Inventor/caches/SoTextureCoordinateCache.h

libcachesinc_HEADERS = $(ALLINSTHEADERS)
libcachesLINKHACKinc_HEADERS = $(ALLINSTHEADERS)

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



install-libcachesLINKHACKincHEADERS: $(libcachesLINKHACKinc_HEADERS)
	@$(NORMAL_INSTALL)
	$(mkinstalldirs) $(DESTDIR)$(libcachesLINKHACKincdir)
	@list='$(libcachesLINKHACKinc_HEADERS)'; for p in $$list; do \
	  if test -f "$$p"; then d= ; else d="$(srcdir)/"; fi; \
	  f="`echo $$p | sed -e 's|^.*/||'`"; \
	  echo " $(INSTALL_DATA) $$d$$p $(DESTDIR)$(libcachesLINKHACKincdir)/$$f"; \
	  $(INSTALL_DATA) $$d$$p $(DESTDIR)$(libcachesLINKHACKincdir)/$$f; \
	done

uninstall-libcachesLINKHACKincHEADERS:
	@$(NORMAL_UNINSTALL)
	@list='$(libcachesLINKHACKinc_HEADERS)'; for p in $$list; do \
	  f="`echo $$p | sed -e 's|^.*/||'`"; \
	  echo " rm -f $(DESTDIR)$(libcachesLINKHACKincdir)/$$f"; \
	  rm -f $(DESTDIR)$(libcachesLINKHACKincdir)/$$f; \
	done
