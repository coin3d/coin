## Process this file with automake to generate Makefile.in.

## FIXME: this file should be rendered obsolete, as the information
## contained in inttypes.h and kosher.h should be moved to config.h.
## 19990912 mortene.

## Installation information.

systemincdir = $(includedir)/Inventor/system

systeminc_HEADERS = \
	$(top_srcdir)/include/Inventor/system/inttypes.h \
	$(top_srcdir)/include/Inventor/system/kosher.h


## FIXME: this is a temporary workaround for a bug in Automake. The
## bug is fixed in the latest Automake CVS, but not in any "official"
## release (?). Last checked with Automake 1.4a. 19990912 mortene.

install-systemincHEADERS: $(systeminc_HEADERS)
	@$(NORMAL_INSTALL)
	$(mkinstalldirs) $(DESTDIR)$(systemincdir)
	@list='$(systeminc_HEADERS)'; for p in $$list; do \
	  if test -f "$$p"; then d= ; else d="$(srcdir)/"; fi; \
	  f="`echo $$p | sed -e 's|^.*/||'`"; \
	  echo " $(INSTALL_DATA) $$d$$p $(DESTDIR)$(systemincdir)/$$f"; \
	  $(INSTALL_DATA) $$d$$p $(DESTDIR)$(systemincdir)/$$f; \
	done

uninstall-systemincHEADERS:
	@$(NORMAL_UNINSTALL)
	@list='$(systeminc_HEADERS)'; for p in $$list; do \
	  f="`echo $$p | sed -e 's|^.*/||'`"; \
	  echo " rm -f $(DESTDIR)$(systemincdir)/$$f"; \
	  rm -f $(DESTDIR)$(systemincdir)/$$f; \
	done
