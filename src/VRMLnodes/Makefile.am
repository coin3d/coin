## Process this file with automake to generate Makefile.in.

noinst_LTLIBRARIES = libVRMLnodes.la

INCLUDES = -I$(top_srcdir)/include/

## All source files used when building this submodule, including all
## headers.

libVRMLnodes_la_SOURCES = \
	SoVRMLInterpOutput.cpp \
	SoVRMLInterpOutput.h \
	SoVRMLInterpolator.cpp \
	SoVRMLInterpolator.h

## Installation information.

libVRMLnodesincdir = $(includedir)/Inventor/VRMLnodes

libVRMLnodesinc_HEADERS = \
	$(top_srcdir)/include/Inventor/VRMLnodes/SoVRMLInterpOutput.h \
	$(top_srcdir)/include/Inventor/VRMLnodes/SoVRMLInterpolator.h


## FIXME: this is a temporary workaround for a bug in Automake. The
## bug is fixed in the latest Automake CVS, but not in any "official"
## release (?). Last checked with Automake 1.4a. 19990912 mortene.

install-libVRMLnodesincHEADERS: $(libVRMLnodesinc_HEADERS)
	@$(NORMAL_INSTALL)
	$(mkinstalldirs) $(DESTDIR)$(libVRMLnodesincdir)
	@list='$(libVRMLnodesinc_HEADERS)'; for p in $$list; do \
	  if test -f "$$p"; then d= ; else d="$(srcdir)/"; fi; \
	  f="`echo $$p | sed -e 's|^.*/||'`"; \
	  echo " $(INSTALL_DATA) $$d$$p $(DESTDIR)$(libVRMLnodesincdir)/$$f"; \
	  $(INSTALL_DATA) $$d$$p $(DESTDIR)$(libVRMLnodesincdir)/$$f; \
	done

uninstall-libVRMLnodesincHEADERS:
	@$(NORMAL_UNINSTALL)
	@list='$(libVRMLnodesinc_HEADERS)'; for p in $$list; do \
	  f="`echo $$p | sed -e 's|^.*/||'`"; \
	  echo " rm -f $(DESTDIR)$(libVRMLnodesincdir)/$$f"; \
	  rm -f $(DESTDIR)$(libVRMLnodesincdir)/$$f; \
	done
