
if BUILD_LIBRARY
if MAC_FRAMEWORK
baseincdir = $(MAC_FRAMEWORK_PREFIX)/$(MAC_FRAMEWORK_NAME).framework/Versions/$(MAC_FRAMEWORK_VERSION)/Headers/C/base
else
baseincdir = $(includedir)/Inventor/C/base
endif
baseinc_HEADERS = \
	hash.h \
	heap.h \
	memalloc.h \
	rbptree.h \
	time.h \
	string.h \
	list.h

install-baseincHEADERS: $(baseinc_HEADERS)
	@$(NORMAL_INSTALL)
	$(mkinstalldirs) $(DESTDIR)$(baseincdir)
	@list='$(baseinc_HEADERS)'; for p in $$list; do \
	  if test -f "$$p"; then d=; else d="$(srcdir)/"; fi; \
	  f="`echo $$p | sed -e 's|^.*/||'`"; \
	  src="$$d$$p"; dst="$(DESTDIR)$(baseincdir)/$$f"; \
	  if cmp -s "$$src" "$$dst"; then :; else \
	    echo " $(INSTALL_HEADER) $$src $$dst"; \
	    $(INSTALL_HEADER) "$$src" "$$dst"; \
	  fi \
	done
endif

