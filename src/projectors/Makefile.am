## Process this file with automake to generate Makefile.in.

noinst_LTLIBRARIES = libprojectors.la

## Remove any linklibraries detected by configure, as they are not
## needed in the submodules -- only in the toplevel libCoin build.
LIBS = 

INCLUDES = -I$(top_srcdir)/include/

## All source files used when building this submodule, including all
## headers.

libprojectors_la_SOURCES = \
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

## Installation information.

libprojectorsincdir = $(includedir)/Inventor/projectors

libprojectorsinc_HEADERS = \
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
