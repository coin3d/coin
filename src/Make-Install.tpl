##$ TEMPLATE Make-Install(-xxx-)

# FIXME: this is a temporary workaround for a bug in Automake. The
# bug is fixed in the latest Automake CVS, but not in any "official"
# release (?). Last checked with Automake 1.4a. 19990912 mortene.

install--xxx-incHEADERS: $(-xxx-inc_HEADERS)
	@$(NORMAL_INSTALL)
	@$(mkinstalldirs) $(DESTDIR)$(-xxx-incdir)
	@list='$(-xxx-inc_HEADERS)'; for p in $$list; do \
	  if test -f "$$p"; then d= ; else d="$(srcdir)/"; fi; \
	  f="`echo $$p | sed -e 's|^.*/||'`"; \
	  if cmp -s $$d$$p $(DESTDIR)$(-xxx-incdir)/$$f; then :; else echo "Installing $(-xxx-incdir)/$$f"; $(INSTALL_DATA) $$d$$p $(DESTDIR)$(-xxx-incdir)/$$f; fi \
	done

uninstall--xxx-incHEADERS:
	@$(NORMAL_UNINSTALL)
	@list='$(-xxx-inc_HEADERS)'; for p in $$list; do \
	  f="`echo $$p | sed -e 's|^.*/||'`"; \
	  echo "Uninstalling $(DESTDIR)$(-xxx-incdir)/$$f"; \
	  rm -f $(DESTDIR)$(-xxx-incdir)/$$f; \
	done

