##$ TEMPLATE Make-Install(-xxx-)

# FIXME: this is a temporary workaround for a bug in Automake. The
# bug is fixed in the latest Automake CVS, but not in any "official"
# release (?). Last checked with Automake 1.4a. 19990912 mortene.

install-lib-xxx-incHEADERS: $(lib-xxx-inc_HEADERS)
	@$(NORMAL_INSTALL)
	$(mkinstalldirs) $(DESTDIR)$(lib-xxx-incdir)
	@list='$(lib-xxx-inc_HEADERS)'; for p in $$list; do \
	  if test -f "$$p"; then d= ; else d="$(srcdir)/"; fi; \
	  f="`echo $$p | sed -e 's|^.*/||'`"; \
	  echo " $(INSTALL_DATA) $$d$$p $(DESTDIR)$(lib-xxx-incdir)/$$f"; \
	  $(INSTALL_DATA) $$d$$p $(DESTDIR)$(lib-xxx-incdir)/$$f; \
	done

uninstall-lib-xxx-incHEADERS:
	@$(NORMAL_UNINSTALL)
	@list='$(lib-xxx-inc_HEADERS)'; for p in $$list; do \
	  f="`echo $$p | sed -e 's|^.*/||'`"; \
	  echo " rm -f $(DESTDIR)$(lib-xxx-incdir)/$$f"; \
	  rm -f $(DESTDIR)$(lib-xxx-incdir)/$$f; \
	done

