
threadsincdir = $(includedir)/Inventor/C/threads
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
	fifo.h
#	barrier.h

## These headers are not installed - they are internal to Coin...
EXTRA_DIST = \
	threadp.h \
	mutexp.h \
	condvarp.h \
	rwmutexp.h \
	storagep.h \
	workerp.h \
	wpoolp.h \
	schedp.h \
	fifop.h
#	barrierp.h

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

