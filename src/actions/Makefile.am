## Process this file with automake to generate Makefile.in.
## 19990904 mortene.

if HACKING_DYNAMIC_MODULES
lib_LTLIBRARIES = libactionsLINKHACK.la
else
noinst_LTLIBRARIES = libactions.la
endif

## Remove any linklibraries detected by configure, as they are not
## needed in the submodules -- only in the toplevel libCoin build.
LIBS = 

INCLUDES = -I$(top_srcdir)/include -I$(top_builddir)/include

## All source files used when building this submodule, including all
## headers.

ALLSOURCES = \
	SoAction.cpp \
	SoAction.h \
	SoBoxHighlightRenderAction.cpp \
	SoBoxHighlightRenderAction.h \
	SoCallbackAction.cpp \
	SoCallbackAction.h \
	SoGLRenderAction.cpp \
	SoGLRenderAction.h \
	SoGetBoundingBoxAction.cpp \
	SoGetBoundingBoxAction.h \
	SoGetMatrixAction.cpp \
	SoGetMatrixAction.h \
	SoGetPrimitiveCountAction.cpp \
	SoGetPrimitiveCountAction.h \
	SoGlobalSimplifyAction.cpp \
	SoGlobalSimplifyAction.h \
	SoHandleEventAction.cpp \
	SoHandleEventAction.h \
	SoLineHighlightRenderAction.cpp \
	SoLineHighlightRenderAction.h \
	SoPickAction.cpp \
	SoPickAction.h \
	SoRayPickAction.cpp \
	SoRayPickAction.h \
	SoReorganizeAction.cpp \
	SoReorganizeAction.h \
	SoSearchAction.cpp \
	SoSearchAction.h \
	SoShapeSimplifyAction.cpp \
	SoShapeSimplifyAction.h \
	SoSimplifyAction.cpp \
	SoSimplifyAction.h \
	SoToVRML2Action.cpp \
	SoToVRML2Action.h \
	SoToVRMLAction.cpp \
	SoToVRMLAction.h \
	SoWriteAction.cpp \
	SoWriteAction.h

libactions_la_SOURCES = $(ALLSOURCES)
libactionsLINKHACK_la_SOURCES = $(ALLSOURCES)

## Installation information.

libactionsincdir = $(includedir)/Inventor/actions
libactionsLINKHACKincdir = $(includedir)/Inventor/actions

ALLINSTHEADERS = \
	$(top_srcdir)/include/Inventor/actions/SoAction.h \
	$(top_srcdir)/include/Inventor/actions/SoBoxHighlightRenderAction.h \
	$(top_srcdir)/include/Inventor/actions/SoCallbackAction.h \
	$(top_srcdir)/include/Inventor/actions/SoGLRenderAction.h \
	$(top_srcdir)/include/Inventor/actions/SoGetBoundingBoxAction.h \
	$(top_srcdir)/include/Inventor/actions/SoGetMatrixAction.h \
	$(top_srcdir)/include/Inventor/actions/SoGetPrimitiveCountAction.h \
	$(top_srcdir)/include/Inventor/actions/SoGlobalSimplifyAction.h \
	$(top_srcdir)/include/Inventor/actions/SoHandleEventAction.h \
	$(top_srcdir)/include/Inventor/actions/SoLineHighlightRenderAction.h \
	$(top_srcdir)/include/Inventor/actions/SoPickAction.h \
	$(top_srcdir)/include/Inventor/actions/SoRayPickAction.h \
	$(top_srcdir)/include/Inventor/actions/SoReorganizeAction.h \
	$(top_srcdir)/include/Inventor/actions/SoSearchAction.h \
	$(top_srcdir)/include/Inventor/actions/SoShapeSimplifyAction.h \
	$(top_srcdir)/include/Inventor/actions/SoSimplifyAction.h \
	$(top_srcdir)/include/Inventor/actions/SoToVRML2Action.h \
	$(top_srcdir)/include/Inventor/actions/SoToVRMLAction.h \
	$(top_srcdir)/include/Inventor/actions/SoWriteAction.h


libactionsinc_HEADERS = $(ALLINSTHEADERS)
libactionsLINKHACKinc_HEADERS = $(ALLINSTHEADERS)


## FIXME: this is a temporary workaround for a bug in Automake. The
## bug is fixed in the latest Automake CVS, but not in any "official"
## release (?). Last checked with Automake 1.4a. 19990912 mortene.

install-libactionsincHEADERS: $(libactionsinc_HEADERS)
	@$(NORMAL_INSTALL)
	$(mkinstalldirs) $(DESTDIR)$(libactionsincdir)
	@list='$(libactionsinc_HEADERS)'; for p in $$list; do \
	  if test -f "$$p"; then d= ; else d="$(srcdir)/"; fi; \
	  f="`echo $$p | sed -e 's|^.*/||'`"; \
	  echo " $(INSTALL_DATA) $$d$$p $(DESTDIR)$(libactionsincdir)/$$f"; \
	  $(INSTALL_DATA) $$d$$p $(DESTDIR)$(libactionsincdir)/$$f; \
	done

uninstall-libactionsincHEADERS:
	@$(NORMAL_UNINSTALL)
	@list='$(libactionsinc_HEADERS)'; for p in $$list; do \
	  f="`echo $$p | sed -e 's|^.*/||'`"; \
	  echo " rm -f $(DESTDIR)$(libactionsincdir)/$$f"; \
	  rm -f $(DESTDIR)$(libactionsincdir)/$$f; \
	done


install-libactionsLINKHACKincHEADERS: $(libactionsLINKHACKinc_HEADERS)
	@$(NORMAL_INSTALL)
	$(mkinstalldirs) $(DESTDIR)$(libactionsLINKHACKincdir)
	@list='$(libactionsLINKHACKinc_HEADERS)'; for p in $$list; do \
	  if test -f "$$p"; then d= ; else d="$(srcdir)/"; fi; \
	  f="`echo $$p | sed -e 's|^.*/||'`"; \
	  echo " $(INSTALL_DATA) $$d$$p $(DESTDIR)$(libactionsLINKHACKincdir)/$$f"; \
	  $(INSTALL_DATA) $$d$$p $(DESTDIR)$(libactionsLINKHACKincdir)/$$f; \
	done

uninstall-libactionsLINKHACKincHEADERS:
	@$(NORMAL_UNINSTALL)
	@list='$(libactionsLINKHACKinc_HEADERS)'; for p in $$list; do \
	  f="`echo $$p | sed -e 's|^.*/||'`"; \
	  echo " rm -f $(DESTDIR)$(libactionsLINKHACKincdir)/$$f"; \
	  rm -f $(DESTDIR)$(libactionsLINKHACKincdir)/$$f; \
	done
