## Process this file with automake to generate Makefile.in.

BUILT_SOURCES = \
	gl.h \
	inttypes.h

DISTCLEANFILES = \
	gl.h \
	inttypes.h

EXTRA_DIST = \
	gl.h.in \
	inttypes.h.in

gl.h: $(srcdir)/inttypes.h.in $(top_builddir)/config.status
	@( cd $(top_builddir); $(MAKE) include/Inventor/system/inttypes.h )

inttypes.h: $(srcdir)/gl.h.in $(top_builddir)/config.status
	@( cd $(top_builddir); $(MAKE) include/Inventor/system/gl.h )

# header installation information

systemincdir = $(includedir)/Inventor/system
nodist_systeminc_HEADERS = \
	gl.h \
	inttypes.h

install-nodist_systemincHEADERS: $(nodist_systeminc_HEADERS)
	@$(NORMAL_INSTALL)
	$(mkinstalldirs) $(DESTDIR)$(systemincdir)
	@list='$(nodist_systeminc_HEADERS)'; for p in $$list; do \
	  if test -f "$$p"; then d=; else d="$(srcdir)/"; fi; \
	  f="`echo $$p | sed -e 's|^.*/||'`"; \
	  src="$$d$$p"; dst="$(DESTDIR)$(systemincdir)/$$f"; \
	  if cmp -s "$$src" "$$dst"; then :; else \
	    echo " $(INSTALL_HEADER) $$src $$dst"; \
	    $(INSTALL_HEADER) "$$src" "$$dst"; \
	  fi \
	done

