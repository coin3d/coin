#! /usr/bin/make -f

.PHONY: all clean

templant = ../../scripts/templant
templates = ../../templates

BUILT_SOURCES = \
  SbVec2s.h \
  SbVec2f.h \
  SbVec2d.h \
  SbVec3f.h \
  SbVec3s.h \
  SbVec3d.h \
  SbVec4s.h \
  SbVec4f.h \
  SbVec4d.h \
  SbBox2s.h \
  SbBox2f.h \
  SbBox2d.h \
  SbBox3f.h \
  SbBox3s.h \
  SbBox3d.h \
  SbMatrix.h \
  SbPlane.h \
  SbLine.h \
  SbRotation.h

all: $(BUILT_SOURCES)

clean:
	rm -f $(BUILT_SOURCES)

SbBox2s.h: $(templant) $(templates)/SbBox-Header.tpl $(templates)/Copyright.tpl
	@if test -f $@; then :; else \
	  echo "//$$ INSERT TEMPLATE SbBox-Header(short, 2, s)" >$@; \
	fi
	@echo "templant -I$(templates) $@"; \
	$(templant) -I$(templates) $@;
	@touch $@;
	@rm -f $@.bak;

SbBox2f.h: $(templant) $(templates)/SbBox-Header.tpl $(templates)/Copyright.tpl
	@if test -f $@; then :; else \
	  echo "//$$ INSERT TEMPLATE SbBox-Header(float, 2, f)" >$@; \
	fi
	@echo "templant -I$(templates) $@"; \
	$(templant) -I$(templates) $@;
	@touch $@;
	@rm -f $@.bak;

SbBox2d.h: $(templant) $(templates)/SbBox-Header.tpl $(templates)/Copyright.tpl
	@if test -f $@; then :; else \
	  echo "//$$ INSERT TEMPLATE SbBox-Header(double, 2, d)" >$@; \
	fi
	@echo "templant -I$(templates) $@"; \
	$(templant) -I$(templates) $@;
	@touch $@;
	@rm -f $@.bak;

SbBox3s.h: $(templant) $(templates)/SbBox-Header.tpl $(templates)/Copyright.tpl
	@if test -f $@; then :; else \
	  echo "//$$ INSERT TEMPLATE SbBox-Header(short, 3, s)" >$@; \
	fi
	@echo "templant -I$(templates) $@"; \
	$(templant) -I$(templates) $@;
	@touch $@;
	@rm -f $@.bak;

SbBox3f.h: $(templant) $(templates)/SbBox-Header.tpl $(templates)/Copyright.tpl
	@if test -f $@; then :; else \
	  echo "//$$ INSERT TEMPLATE SbBox-Header(float, 3, f)" >$@; \
	fi
	@echo "templant -I$(templates) $@"; \
	$(templant) -I$(templates) $@;
	@touch $@;
	@rm -f $@.bak;

SbBox3d.h: $(templant) $(templates)/SbBox-Header.tpl $(templates)/Copyright.tpl
	@if test -f $@; then :; else \
	  echo "//$$ INSERT TEMPLATE SbBox-Header(double, 3, d)" >$@; \
	fi
	@echo "templant -I$(templates) $@"; \
	$(templant) -I$(templates) $@;
	@touch $@;
	@rm -f $@.bak;

SbVec2s.h: $(templant) $(templates)/SbVector-Header.tpl $(templates)/Copyright.tpl
	@if test -f $@; then :; else \
	  echo "//$$ INSERT TEMPLATE SbVector-Header(short, 2, s)" >$@; \
	fi
	@echo "templant -I$(templates) $@"; \
	$(templant) -I$(templates) $@;
	@touch $@;
	@rm -f $@.bak;

SbVec2f.h: $(templant) $(templates)/SbVector-Header.tpl $(templates)/Copyright.tpl
	@if test -f $@; then :; else \
	  echo "//$$ INSERT TEMPLATE SbVector-Header(float, 2, f)" >$@; \
	fi
	@echo "templant -I$(templates) $@"; \
	$(templant) -I$(templates) $@;
	@touch $@;
	@rm -f $@.bak;

SbVec2d.h: $(templant) $(templates)/SbVector-Header.tpl $(templates)/Copyright.tpl
	@if test -f $@; then :; else \
	  echo "//$$ INSERT TEMPLATE SbVector-Header(double, 2, d)" >$@; \
	fi
	@echo "templant -I$(templates) $@"; \
	$(templant) -I$(templates) $@;
	@touch $@;
	@rm -f $@.bak;

SbVec3s.h: $(templant) $(templates)/SbVector-Header.tpl $(templates)/Copyright.tpl
	@if test -f $@; then :; else \
	  echo "//$$ INSERT TEMPLATE SbVector-Header(short, 3, s)" >$@; \
	fi
	@echo "templant -I$(templates) $@"; \
	$(templant) -I$(templates) $@;
	@touch $@;
	@rm -f $@.bak;

SbVec3f.h: $(templant) $(templates)/SbVector-Header.tpl $(templates)/Copyright.tpl
	@if test -f $@; then :; else \
	  echo "//$$ INSERT TEMPLATE SbVector-Header(float, 3, f)" >$@; \
	fi
	@echo "templant -I$(templates) $@"; \
	$(templant) -I$(templates) $@;
	@touch $@;
	@rm -f $@.bak;

SbVec3d.h: $(templant) $(templates)/SbVector-Header.tpl $(templates)/Copyright.tpl
	@if test -f $@; then :; else \
	  echo "//$$ INSERT TEMPLATE SbVector-Header(double, 3, d)" >$@; \
	fi
	@echo "templant -I$(templates) $@"; \
	$(templant) -I$(templates) $@;
	@touch $@;
	@rm -f $@.bak;

SbVec4s.h: $(templant) $(templates)/SbVector-Header.tpl $(templates)/Copyright.tpl
	@if test -f $@; then :; else \
	  echo "//$$ INSERT TEMPLATE SbVector-Header(short, 4, s)" >$@; \
	fi
	@echo "templant -I$(templates) $@"; \
	$(templant) -I$(templates) $@;
	@touch $@;
	@rm -f $@.bak;

SbVec4f.h: $(templant) $(templates)/SbVector-Header.tpl $(templates)/Copyright.tpl
	@if test -f $@; then :; else \
	  echo "//$$ INSERT TEMPLATE SbVector-Header(float, 4, f)" >$@; \
	fi
	@echo "templant -I$(templates) $@"; \
	$(templant) -I$(templates) $@;
	@touch $@;
	@rm -f $@.bak;

SbVec4d.h: $(templant) $(templates)/SbVector-Header.tpl $(templates)/Copyright.tpl
	@if test -f $@; then :; else \
	  echo "//$$ INSERT TEMPLATE SbVector-Header(double, 4, d)" >$@; \
	fi
	@echo "templant -I$(templates) $@"; \
	$(templant) -I$(templates) $@;
	@touch $@;
	@rm -f $@.bak;

SbMatrix.h: $(templant) $(templates)/SbMatrix-Header.tpl $(templates)/Copyright.tpl
	@if test -f $@; then :; else \
	  echo "//$$ INSERT TEMPLATE SbMatrix-Header(float, 4, f)" >$@; \
	fi
	@echo "templant -I$(templates) $@"; \
	$(templant) -I$(templates) $@;
	@touch $@;
	@rm -f $@.bak;

SbPlane.h: $(templant) $(templates)/SbPlane-Header.tpl $(templates)/Copyright.tpl
	@if test -f $@; then :; else \
	  echo "//$$ INSERT TEMPLATE SbPlane-Header(float, 3, f)" >$@; \
	fi
	@echo "templant -I$(templates) $@"; \
	$(templant) -I$(templates) $@;
	@touch $@;
	@rm -f $@.bak;

SbLine.h: $(templant) $(templates)/SbLine-Header.tpl $(templates)/Copyright.tpl
	@if test -f $@; then :; else \
	  echo "//$$ INSERT TEMPLATE SbLine-Header(float, 3, f)" >$@; \
	fi
	@echo "templant -I$(templates) $@"; \
	$(templant) -I$(templates) $@;
	@touch $@;
	@rm -f $@.bak;

SbRotation.h: $(templant) $(templates)/SbRotation-Header.tpl $(templates)/Copyright.tpl
	@if test -f $@; then :; else \
	  echo "//$$ INSERT TEMPLATE SbRotation-Header(float, 4, f)" >$@; \
	fi
	@echo "templant -I$(templates) $@"; \
	$(templant) -I$(templates) $@;
	@touch $@;
	@rm -f $@.bak;

