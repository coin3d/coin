## Process this file with automake to generate Makefile.in.

if HACKING_DYNAMIC_MODULES
lib_LTLIBRARIES = libmiscLINKHACK.la
else
noinst_LTLIBRARIES = libmisc.la
endif

## Remove any linklibraries detected by configure, as they are not
## needed in the submodules -- only in the toplevel libCoin build.
LIBS = 

INCLUDES = -I$(top_srcdir)/include -I$(top_builddir)/include

## All source files used when building this submodule, including all
## headers.

ALLSOURCES = \
	SoBase.cpp \
	SoBase.h \
	SoChildList.cpp \
	SoChildList.h \
	SoGL.cpp \
	SoGL.h \
	SoGLImage.cpp \
	SoGLImage.h \
	SoNormalGenerator.cpp \
	SoNormalGenerator.h \
	SoNotification.cpp \
	SoNotification.h \
	SoState.cpp \
	SoState.h \
	SoDB.cpp \
	SoDB.h \
	SoFullPath.cpp \
	SoFullPath.h \
	SoImageInterface.cpp \
	SoImageInterface.h \
	SoInput.cpp \
	SoInput.h \
	SoInteraction.cpp \
	SoInteraction.h \
	SoOutput.cpp \
	SoOutput.h \
	SoPath.cpp \
	SoPath.h \
	SoPickedPoint.cpp \
	SoPickedPoint.h \
	SoPrimitiveVertex.cpp \
	SoPrimitiveVertex.h \
	SoSceneManager.cpp \
	SoSceneManager.h \
	SoType.cpp \
	SoType.h \
	SoGenerate.cpp


libmisc_la_SOURCES = $(ALLSOURCES)
libmiscLINKHACK_la_SOURCES = $(ALLSOURCES)

## Installation information.

libmiscincdir = $(includedir)/Inventor/misc
libmiscLINKHACKincdir = $(includedir)/Inventor/misc

ALLINSTHEADERS = \
	$(top_srcdir)/include/Inventor/misc/SoAuditorList.h \
	$(top_srcdir)/include/Inventor/misc/SoBase.h \
	$(top_srcdir)/include/Inventor/misc/SoBasic.h \
	$(top_srcdir)/include/Inventor/misc/SoCallbackList.h \
	$(top_srcdir)/include/Inventor/misc/SoChildList.h \
	$(top_srcdir)/include/Inventor/misc/SoNotification.h \
	$(top_srcdir)/include/Inventor/misc/SoState.h

libmiscinc_HEADERS = $(ALLINSTHEADERS)
libmiscLINKHACKinc_HEADERS = $(ALLINSTHEADERS)

## FIXME: this is a temporary workaround for a bug in Automake. The
## bug is fixed in the latest Automake CVS, but not in any "official"
## release (?). Last checked with Automake 1.4a. 19990912 mortene.

install-libmiscincHEADERS: $(libmiscinc_HEADERS)
	@$(NORMAL_INSTALL)
	$(mkinstalldirs) $(DESTDIR)$(libmiscincdir)
	@list='$(libmiscinc_HEADERS)'; for p in $$list; do \
	  if test -f "$$p"; then d= ; else d="$(srcdir)/"; fi; \
	  f="`echo $$p | sed -e 's|^.*/||'`"; \
	  echo " $(INSTALL_DATA) $$d$$p $(DESTDIR)$(libmiscincdir)/$$f"; \
	  $(INSTALL_DATA) $$d$$p $(DESTDIR)$(libmiscincdir)/$$f; \
	done

uninstall-libmiscincHEADERS:
	@$(NORMAL_UNINSTALL)
	@list='$(libmiscinc_HEADERS)'; for p in $$list; do \
	  f="`echo $$p | sed -e 's|^.*/||'`"; \
	  echo " rm -f $(DESTDIR)$(libmiscincdir)/$$f"; \
	  rm -f $(DESTDIR)$(libmiscincdir)/$$f; \
	done


install-libmiscLINKHACKincHEADERS: $(libmiscLINKHACKinc_HEADERS)
	@$(NORMAL_INSTALL)
	$(mkinstalldirs) $(DESTDIR)$(libmiscLINKHACKincdir)
	@list='$(libmiscLINKHACKinc_HEADERS)'; for p in $$list; do \
	  if test -f "$$p"; then d= ; else d="$(srcdir)/"; fi; \
	  f="`echo $$p | sed -e 's|^.*/||'`"; \
	  echo " $(INSTALL_DATA) $$d$$p $(DESTDIR)$(libmiscLINKHACKincdir)/$$f"; \
	  $(INSTALL_DATA) $$d$$p $(DESTDIR)$(libmiscLINKHACKincdir)/$$f; \
	done

uninstall-libmiscLINKHACKincHEADERS:
	@$(NORMAL_UNINSTALL)
	@list='$(libmiscLINKHACKinc_HEADERS)'; for p in $$list; do \
	  f="`echo $$p | sed -e 's|^.*/||'`"; \
	  echo " rm -f $(DESTDIR)$(libmiscLINKHACKincdir)/$$f"; \
	  rm -f $(DESTDIR)$(libmiscLINKHACKincdir)/$$f; \
	done
