## Process this file with automake to generate Makefile.in.

## FIXME: this file should eventually be rendered obsolete, as the
## inttypes.h should be moved to include/inttypes.h. I think. Looks
## like poor design, but it _is_ what OIV does. The _contents_ of
## inttypes.h should be moved to SbBasic.h.in, though, so we can
## decide and set the integer types from configure. 19990912 mortene.

## Installation information.

systemincdir = $(includedir)/Inventor/system

systeminc_HEADERS = \
	$(top_srcdir)/include/Inventor/system/inttypes.h

##$ BEGIN TEMPLATE Make-Install(system)

# Replaced the ordinary install to compare files and only install if
# there's any changes. mortene.

install-systemincHEADERS: $(systeminc_HEADERS)
	@$(NORMAL_INSTALL)
	@$(mkinstalldirs) $(DESTDIR)$(systemincdir)
	@list='$(systeminc_HEADERS)'; for p in $$list; do \
	  if test -f "$$p"; then d= ; else d="$(srcdir)/"; fi; \
	  f="`echo $$p | sed -e 's|^.*/||'`"; \
	  if cmp -s $$d$$p $(DESTDIR)$(systemincdir)/$$f; then : ; else echo " Installing $(DESTDIR)$(systemincdir)/$$f"; \
	  $(INSTALL_DATA) $$d$$p $(DESTDIR)$(systemincdir)/$$f; fi; \
	done

##$ END TEMPLATE Make-Install
