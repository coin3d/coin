# NOTES:
#   This Makefile.am relies on $(htmldir) being AC_SUBST'ed into place...

if BUILD_HTMLPAGES

# **************************************************************************
# build trickery

all-local: filelist.txt

clean-local:
	@if test -f filelist.txt; then \
	  cat filelist.txt | xargs rm -f filelist.txt; \
	fi

# **************************************************************************
# install trickery
# - uninstall after "make clean" will regenerate docs - ought to be fixed

install-data-local: install-html

uninstall-local: uninstall-html

install-html: filelist.txt
	@$(NORMAL_INSTALL)
	$(mkinstalldirs) $(DESTDIR)$(htmldir)
	@list="`cat filelist.txt`"; \
	for i in $$list; do \
	  echo " $(INSTALL_DATA) $$i $(DESTDIR)$(htmldir)/$$i"; \
	  $(INSTALL_DATA) $$i $(DESTDIR)$(htmldir)/$$i; \
	done

uninstall-html: filelist.txt
	@$(NORMAL_UNINSTALL)
	@list="`cat filelist.txt`"; \
	for i in $$list; do \
	  echo " rm -f $(DESTDIR)$(htmldir)/$$i"; \
	  rm -f $(DESTDIR)$(htmldir)/$$i; \
	done
	@echo " rmdir $(DESTDIR)$(htmldir)"; \
	rmdir $(DESTDIR)$(htmldir) 2>/dev/null; \
	:

endif

$(top_builddir)/docs/coin.doxygen: $(top_srcdir)/docs/coin.doxygen.in $(top_builddir)/config.status
	@( cd $(top_builddir); $(MAKE) docs/coin.doxygen )

index.html: $(top_builddir)/docs/coin.doxygen
	@echo " @sim_ac_doxygen_exe@ $(top_builddir)/docs/coin.doxygen"; \
	@sim_ac_doxygen_exe@ $(top_builddir)/docs/coin.doxygen

filelist.txt: index.html
	@ls -1 *.html *.gif *.css >filelist.txt

