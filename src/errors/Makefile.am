## Process this file with automake to generate Makefile.in.

noinst_LTLIBRARIES = liberrors.la

## Remove any linklibraries detected by configure, as they are not
## needed in the submodules -- only in the toplevel libCoin build.
LIBS = 

INCLUDES = -I$(top_srcdir)/include -I$(top_builddir)/include

## All source files used when building this submodule, including all
## headers.

liberrors_la_SOURCES = \
	SoDebugError.cpp \
	SoDebugError.h \
	SoError.cpp \
	SoError.h \
	SoMemoryError.cpp \
	SoMemoryError.h \
	SoReadError.cpp \
	SoReadError.h

## Installation information.

liberrorsincdir = $(includedir)/Inventor/errors

liberrorsinc_HEADERS = \
	$(top_srcdir)/include/Inventor/errors/SoDebugError.h \
	$(top_srcdir)/include/Inventor/errors/SoError.h \
	$(top_srcdir)/include/Inventor/errors/SoMemoryError.h \
	$(top_srcdir)/include/Inventor/errors/SoReadError.h


## FIXME: this is a temporary workaround for a bug in Automake. The
## bug is fixed in the latest Automake CVS, but not in any "official"
## release (?). Last checked with Automake 1.4a. 19990912 mortene.

install-liberrorsincHEADERS: $(liberrorsinc_HEADERS)
	@$(NORMAL_INSTALL)
	$(mkinstalldirs) $(DESTDIR)$(liberrorsincdir)
	@list='$(liberrorsinc_HEADERS)'; for p in $$list; do \
	  if test -f "$$p"; then d= ; else d="$(srcdir)/"; fi; \
	  f="`echo $$p | sed -e 's|^.*/||'`"; \
	  echo " $(INSTALL_DATA) $$d$$p $(DESTDIR)$(liberrorsincdir)/$$f"; \
	  $(INSTALL_DATA) $$d$$p $(DESTDIR)$(liberrorsincdir)/$$f; \
	done

uninstall-liberrorsincHEADERS:
	@$(NORMAL_UNINSTALL)
	@list='$(liberrorsinc_HEADERS)'; for p in $$list; do \
	  f="`echo $$p | sed -e 's|^.*/||'`"; \
	  echo " rm -f $(DESTDIR)$(liberrorsincdir)/$$f"; \
	  rm -f $(DESTDIR)$(liberrorsincdir)/$$f; \
	done
