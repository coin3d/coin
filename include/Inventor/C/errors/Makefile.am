errorsincdir = $(includedir)/Inventor/C/errors
errorsinc_HEADERS = \
	error.h \
	debugerror.h

install-errorsincHEADERS: $(errorsinc_HEADERS)
	@$(NORMAL_INSTALL)
	$(mkinstalldirs) $(DESTDIR)$(errorsincdir)
	@list='$(errorsinc_HEADERS)'; for p in $$list; do \
	  if test -f "$$p"; then d=; else d="$(srcdir)/"; fi; \
	  f="`echo $$p | sed -e 's|^.*/||'`"; \
	  src="$$d$$p"; dst="$(DESTDIR)$(errorsincdir)/$$f"; \
	  if cmp -s "$$src" "$$dst"; then :; else \
	    echo " $(INSTALL_HEADER) $$src $$dst"; \
	    $(INSTALL_HEADER) "$$src" "$$dst"; \
	  fi \
	done
