## Process this file with automake to generate Makefile.in.

noinst_LTLIBRARIES = liblists.la

## Remove any linklibraries detected by configure, as they are not
## needed in the submodules -- only in the toplevel libCoin build.
LIBS = 

INCLUDES = -I$(top_srcdir)/include/

## All source files used when building this submodule, including all
## headers.

liblists_la_SOURCES = \
	SbIntList.cpp \
	SbIntList.h \
	SbList.cpp \
	SbList.h \
	SbPList.cpp \
	SbPList.h \
	SbStringList.cpp \
	SbStringList.h \
	SoActionMethodList.cpp \
	SoActionMethodList.h \
	SoAuditorList.cpp \
	SoAuditorList.h \
	SoBaseList.cpp \
	SoBaseList.h \
	SoCallbackList.cpp \
	SoCallbackList.h \
	SoDetailList.cpp \
	SoDetailList.h \
	SoEnabledElementsList.cpp \
	SoEnabledElementsList.h \
	SoEngineList.cpp \
	SoEngineList.h \
	SoEngineOutputList.cpp \
	SoEngineOutputList.h \
	SoFieldList.cpp \
	SoFieldList.h \
	SoNodeList.cpp \
	SoNodeList.h \
	SoPathList.cpp \
	SoPathList.h \
	SoPickedPointList.cpp \
	SoPickedPointList.h \
	SoTypeList.cpp \
	SoTypeList.h \
	SoVRMLInterpOutputList.cpp \
	SoVRMLInterpOutputList.h

## Installation information.

liblistsincdir = $(includedir)/Inventor/lists

liblistsinc_HEADERS = \
	$(top_srcdir)/include/Inventor/lists/SbIntList.h \
	$(top_srcdir)/include/Inventor/lists/SbList.h \
	$(top_srcdir)/include/Inventor/lists/SbPList.h \
	$(top_srcdir)/include/Inventor/lists/SbStringList.h \
	$(top_srcdir)/include/Inventor/lists/SoActionMethodList.h \
	$(top_srcdir)/include/Inventor/lists/SoAuditorList.h \
	$(top_srcdir)/include/Inventor/lists/SoBaseList.h \
	$(top_srcdir)/include/Inventor/lists/SoCallbackList.h \
	$(top_srcdir)/include/Inventor/lists/SoDetailList.h \
	$(top_srcdir)/include/Inventor/lists/SoEnabledElementsList.h \
	$(top_srcdir)/include/Inventor/lists/SoEngineList.h \
	$(top_srcdir)/include/Inventor/lists/SoEngineOutputList.h \
	$(top_srcdir)/include/Inventor/lists/SoFieldList.h \
	$(top_srcdir)/include/Inventor/lists/SoNodeList.h \
	$(top_srcdir)/include/Inventor/lists/SoPathList.h \
	$(top_srcdir)/include/Inventor/lists/SoPickedPointList.h \
	$(top_srcdir)/include/Inventor/lists/SoTypeList.h \
	$(top_srcdir)/include/Inventor/lists/SoVRMLInterpOutputList.h


## FIXME: this is a temporary workaround for a bug in Automake. The
## bug is fixed in the latest Automake CVS, but not in any "official"
## release (?). Last checked with Automake 1.4a. 19990912 mortene.

install-liblistsincHEADERS: $(liblistsinc_HEADERS)
	@$(NORMAL_INSTALL)
	$(mkinstalldirs) $(DESTDIR)$(liblistsincdir)
	@list='$(liblistsinc_HEADERS)'; for p in $$list; do \
	  if test -f "$$p"; then d= ; else d="$(srcdir)/"; fi; \
	  f="`echo $$p | sed -e 's|^.*/||'`"; \
	  echo " $(INSTALL_DATA) $$d$$p $(DESTDIR)$(liblistsincdir)/$$f"; \
	  $(INSTALL_DATA) $$d$$p $(DESTDIR)$(liblistsincdir)/$$f; \
	done

uninstall-liblistsincHEADERS:
	@$(NORMAL_UNINSTALL)
	@list='$(liblistsinc_HEADERS)'; for p in $$list; do \
	  f="`echo $$p | sed -e 's|^.*/||'`"; \
	  echo " rm -f $(DESTDIR)$(liblistsincdir)/$$f"; \
	  rm -f $(DESTDIR)$(liblistsincdir)/$$f; \
	done
