## Process this file with automake to generate Makefile.in.

if HACKING_DYNAMIC_MODULES
lib_LTLIBRARIES = libprojectorsLINKHACK.la
else
noinst_LTLIBRARIES = libprojectors.la
endif

## Remove any linklibraries detected by configure, as they are not
## needed in the submodules -- only in the toplevel libCoin build.
LIBS = 

INCLUDES = -I$(top_srcdir)/include -I$(top_builddir)/include

## All source files used when building this submodule, including all
## headers.

ALLSOURCES = \
	SbCylinderPlaneProjector.cpp \
	SbCylinderPlaneProjector.h \
	SbCylinderProjector.cpp \
	SbCylinderProjector.h \
	SbCylinderSectionProjector.cpp \
	SbCylinderSectionProjector.h \
	SbCylinderSheetProjector.cpp \
	SbCylinderSheetProjector.h \
	SbLineProjector.cpp \
	SbLineProjector.h \
	SbPlaneProjector.cpp \
	SbPlaneProjector.h \
	SbProjector.cpp \
	SbProjector.h \
	SbSpherePlaneProjector.cpp \
	SbSpherePlaneProjector.h \
	SbSphereProjector.cpp \
	SbSphereProjector.h \
	SbSphereSectionProjector.cpp \
	SbSphereSectionProjector.h \
	SbSphereSheetProjector.cpp \
	SbSphereSheetProjector.h

libprojectors_la_SOURCES = $(ALLSOURCES)
libprojectorsLINKHACK_la_SOURCES = $(ALLSOURCES)

## Installation information.

libprojectorsincdir = $(includedir)/Inventor/projectors
libprojectorsLINKHACKincdir = $(includedir)/Inventor/projectors

ALLINSTHEADERS = \
	$(top_srcdir)/include/Inventor/projectors/SbCylinderPlaneProjector.h \
	$(top_srcdir)/include/Inventor/projectors/SbCylinderProjector.h \
	$(top_srcdir)/include/Inventor/projectors/SbCylinderSectionProjector.h \
	$(top_srcdir)/include/Inventor/projectors/SbCylinderSheetProjector.h \
	$(top_srcdir)/include/Inventor/projectors/SbLineProjector.h \
	$(top_srcdir)/include/Inventor/projectors/SbPlaneProjector.h \
	$(top_srcdir)/include/Inventor/projectors/SbProjector.h \
	$(top_srcdir)/include/Inventor/projectors/SbSpherePlaneProjector.h \
	$(top_srcdir)/include/Inventor/projectors/SbSphereProjector.h \
	$(top_srcdir)/include/Inventor/projectors/SbSphereSectionProjector.h \
	$(top_srcdir)/include/Inventor/projectors/SbSphereSheetProjector.h


libprojectorsinc_HEADERS = $(ALLINSTHEADERS)
libprojectorsLINKHACKinc_HEADERS = $(ALLINSTHEADERS)

## FIXME: this is a temporary workaround for a bug in Automake. The
## bug is fixed in the latest Automake CVS, but not in any "official"
## release (?). Last checked with Automake 1.4a. 19990912 mortene.

install-libprojectorsincHEADERS: $(libprojectorsinc_HEADERS)
	@$(NORMAL_INSTALL)
	$(mkinstalldirs) $(DESTDIR)$(libprojectorsincdir)
	@list='$(libprojectorsinc_HEADERS)'; for p in $$list; do \
	  if test -f "$$p"; then d= ; else d="$(srcdir)/"; fi; \
	  f="`echo $$p | sed -e 's|^.*/||'`"; \
	  echo " $(INSTALL_DATA) $$d$$p $(DESTDIR)$(libprojectorsincdir)/$$f"; \
	  $(INSTALL_DATA) $$d$$p $(DESTDIR)$(libprojectorsincdir)/$$f; \
	done

uninstall-libprojectorsincHEADERS:
	@$(NORMAL_UNINSTALL)
	@list='$(libprojectorsinc_HEADERS)'; for p in $$list; do \
	  f="`echo $$p | sed -e 's|^.*/||'`"; \
	  echo " rm -f $(DESTDIR)$(libprojectorsincdir)/$$f"; \
	  rm -f $(DESTDIR)$(libprojectorsincdir)/$$f; \
	done


install-libprojectorsLINKHACKincHEADERS: $(libprojectorsLINKHACKinc_HEADERS)
	@$(NORMAL_INSTALL)
	$(mkinstalldirs) $(DESTDIR)$(libprojectorsLINKHACKincdir)
	@list='$(libprojectorsLINKHACKinc_HEADERS)'; for p in $$list; do \
	  if test -f "$$p"; then d= ; else d="$(srcdir)/"; fi; \
	  f="`echo $$p | sed -e 's|^.*/||'`"; \
	  echo " $(INSTALL_DATA) $$d$$p $(DESTDIR)$(libprojectorsLINKHACKincdir)/$$f"; \
	  $(INSTALL_DATA) $$d$$p $(DESTDIR)$(libprojectorsLINKHACKincdir)/$$f; \
	done

uninstall-libprojectorsLINKHACKincHEADERS:
	@$(NORMAL_UNINSTALL)
	@list='$(libprojectorsLINKHACKinc_HEADERS)'; for p in $$list; do \
	  f="`echo $$p | sed -e 's|^.*/||'`"; \
	  echo " rm -f $(DESTDIR)$(libprojectorsLINKHACKincdir)/$$f"; \
	  rm -f $(DESTDIR)$(libprojectorsLINKHACKincdir)/$$f; \
	done
