##$ TEMPLATE Make-Install(-xxx-)

# Replaced the ordinary install to compare files and only install if
# there's any changes. mortene.

install--xxx-incHEADERS: $(-xxx-inc_HEADERS)
	@$(NORMAL_INSTALL)
	@$(mkinstalldirs) $(DESTDIR)$(-xxx-incdir)
	@list='$(-xxx-inc_HEADERS)'; for p in $$list; do \
	  if test -f "$$p"; then d= ; else d="$(srcdir)/"; fi; \
	  f="`echo $$p | sed -e 's|^.*/||'`"; \
	  if ! cmp -s $$d$$p $(DESTDIR)$(-xxx-incdir)/$$f; then echo " Installing $(DESTDIR)$(-xxx-incdir)/$$f"; \
	  $(INSTALL_DATA) $$d$$p $(DESTDIR)$(-xxx-incdir)/$$f; fi; \
	done

