
baseincdir = $(includedir)/Inventor/C/base
baseinc_HEADERS = \
	dict.h \
	debug.h \
	hash.h \
	heap.h \
	rbptree.h \
	time.h \
	string.h \
	list.h

EXTRA_DIST = \
	basep.h \
	hashp.h \
	heapp.h \
	name.h \
	string.h

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

