## Process this file with automake to generate Makefile.in.

noinst_LTLIBRARIES = libmisc.la

INCLUDES = -I$(top_srcdir)/include/

## All source files used when building this submodule, including all
## headers.

libmisc_la_SOURCES = \
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
	SoTessellator.cpp \
	SoTessellator.h

## Installation information.

libmiscincdir = $(includedir)/Inventor/misc

libmiscinc_HEADERS = \
	$(top_srcdir)/include/Inventor/misc/SoAuditorList.h \
	$(top_srcdir)/include/Inventor/misc/SoBase.h \
	$(top_srcdir)/include/Inventor/misc/SoBasic.h \
	$(top_srcdir)/include/Inventor/misc/SoCallbackList.h \
	$(top_srcdir)/include/Inventor/misc/SoChildList.h \
	$(top_srcdir)/include/Inventor/misc/SoNotification.h \
	$(top_srcdir)/include/Inventor/misc/SoState.h

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
