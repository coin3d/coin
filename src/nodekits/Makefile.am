## Process this file with automake to generate Makefile.in.

noinst_LTLIBRARIES = libnodekits.la

INCLUDES = -I$(top_srcdir)/include/

## All source files used when building this submodule, including all
## headers.

libnodekits_la_SOURCES = \
	SoNodeKit.cpp \
	SoNodeKit.h

## Installation information.

libnodekitsincdir = $(includedir)/Inventor/nodekits

libnodekitsinc_HEADERS = \
	$(top_srcdir)/include/Inventor/nodekits/SoNodeKit.h


## FIXME: this is a temporary workaround for a bug in Automake. The
## bug is fixed in the latest Automake CVS, but not in any "official"
## release (?). Last checked with Automake 1.4a. 19990912 mortene.

install-libnodekitsincHEADERS: $(libnodekitsinc_HEADERS)
	@$(NORMAL_INSTALL)
	$(mkinstalldirs) $(DESTDIR)$(libnodekitsincdir)
	@list='$(libnodekitsinc_HEADERS)'; for p in $$list; do \
	  if test -f "$$p"; then d= ; else d="$(srcdir)/"; fi; \
	  f="`echo $$p | sed -e 's|^.*/||'`"; \
	  echo " $(INSTALL_DATA) $$d$$p $(DESTDIR)$(libnodekitsincdir)/$$f"; \
	  $(INSTALL_DATA) $$d$$p $(DESTDIR)$(libnodekitsincdir)/$$f; \
	done

uninstall-libnodekitsincHEADERS:
	@$(NORMAL_UNINSTALL)
	@list='$(libnodekitsinc_HEADERS)'; for p in $$list; do \
	  f="`echo $$p | sed -e 's|^.*/||'`"; \
	  echo " rm -f $(DESTDIR)$(libnodekitsincdir)/$$f"; \
	  rm -f $(DESTDIR)$(libnodekitsincdir)/$$f; \
	done
