## Process this file with automake to generate Makefile.in.

## Installation information.

incdir = $(includedir)/Inventor

inc_HEADERS = \
	$(top_builddir)/include/Inventor/SbBasic.h


## FIXME: this is a temporary workaround for a bug in Automake. The
## bug is fixed in the latest Automake CVS, but not in any "official"
## release (?). Last checked with Automake 1.4a. 19990912 mortene.

install-incHEADERS: $(inc_HEADERS)
	@$(NORMAL_INSTALL)
	$(mkinstalldirs) $(DESTDIR)$(incdir)
	@list='$(inc_HEADERS)'; for p in $$list; do \
	  if test -f "$$p"; then d= ; else d="$(srcdir)/"; fi; \
	  f="`echo $$p | sed -e 's|^.*/||'`"; \
	  echo " $(INSTALL_DATA) $$d$$p $(DESTDIR)$(incdir)/$$f"; \
	  $(INSTALL_DATA) $$d$$p $(DESTDIR)$(incdir)/$$f; \
	done

uninstall-incHEADERS:
	@$(NORMAL_UNINSTALL)
	@list='$(inc_HEADERS)'; for p in $$list; do \
	  f="`echo $$p | sed -e 's|^.*/||'`"; \
	  echo " rm -f $(DESTDIR)$(incdir)/$$f"; \
	  rm -f $(DESTDIR)$(incdir)/$$f; \
	done
