## Process this file with automake to generate Makefile.in.

## Installation information.

lockincdir = $(includedir)/Inventor/lock

lockinc_HEADERS = \
	$(top_srcdir)/include/Inventor/lock/SoLockMgr.h


##$ BEGIN TEMPLATE Make-Install(lock)

# Replaced the ordinary install to compare files and only install if
# there's any changes. mortene.

install-lockincHEADERS: $(lockinc_HEADERS)
	@$(NORMAL_INSTALL)
	@$(mkinstalldirs) $(DESTDIR)$(lockincdir)
	@list='$(lockinc_HEADERS)'; for p in $$list; do \
	  if test -f "$$p"; then d= ; else d="$(srcdir)/"; fi; \
	  f="`echo $$p | sed -e 's|^.*/||'`"; \
	  if cmp -s $$d$$p $(DESTDIR)$(lockincdir)/$$f; then : ; else echo " Installing $(DESTDIR)$(lockincdir)/$$f"; \
	  $(INSTALL_DATA) $$d$$p $(DESTDIR)$(lockincdir)/$$f; fi; \
	done

##$ END TEMPLATE Make-Install
