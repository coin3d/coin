
if BUILD_LIBRARY
if MAC_FRAMEWORK
threadsincdir = $(MAC_FRAMEWORK_PREFIX)/$(MAC_FRAMEWORK_NAME).framework/Versions/$(MAC_FRAMEWORK_VERSION)/Headers/C/threads
else
threadsincdir = $(includedir)/Inventor/C/threads
endif
threadsinc_HEADERS = \
	common.h \
	thread.h \
	mutex.h \
	condvar.h \
	rwmutex.h \
	storage.h \
	worker.h \
	wpool.h \
	sched.h \
	sync.h \
	fifo.h \
	barrier.h

install-threadsincHEADERS: $(threadsinc_HEADERS)
	@$(NORMAL_INSTALL)
	$(mkinstalldirs) $(DESTDIR)$(threadsincdir)
	@list='$(threadsinc_HEADERS)'; for p in $$list; do \
	  if test -f "$$p"; then d=; else d="$(srcdir)/"; fi; \
	  f="`echo $$p | sed -e 's|^.*/||'`"; \
	  src="$$d$$p"; dst="$(DESTDIR)$(threadsincdir)/$$f"; \
	  if cmp -s "$$src" "$$dst"; then :; else \
	    echo " $(INSTALL_HEADER) $$src $$dst"; \
	    $(INSTALL_HEADER) "$$src" "$$dst"; \
	  fi \
	done

endif
