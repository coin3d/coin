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
  SbVec4d.h

all: $(BUILT_SOURCES)

clean:
	rm -f $(BUILT_SOURCES)

SbVec2s.h: $(templant) $(templates)/SbVec-Header.tpl $(templates)/Copyright.tpl
	echo "//$$ INSERT TEMPLATE SbVec-Header(short, 2, s)" >$@;
	@echo "templant -I$(templates) $@"; \
	$(templant) -I$(templates) $@;
	@touch $@;
	@rm -f $@.bak;

SbVec2f.h: $(templant) $(templates)/SbVec-Header.tpl $(templates)/Copyright.tpl
	echo "//$$ INSERT TEMPLATE SbVec-Header(float, 2, f)" >$@;
	@echo "templant -I$(templates) $@"; \
	$(templant) -I$(templates) $@;
	@touch $@;
	@rm -f $@.bak;

SbVec2d.h: $(templant) $(templates)/SbVec-Header.tpl $(templates)/Copyright.tpl
	echo "//$$ INSERT TEMPLATE SbVec-Header(double, 2, d)" >$@;
	@echo "templant -I$(templates) $@"; \
	$(templant) -I$(templates) $@;
	@touch $@;
	@rm -f $@.bak;

SbVec3s.h: $(templant) $(templates)/SbVec-Header.tpl $(templates)/Copyright.tpl
	echo "//$$ INSERT TEMPLATE SbVec-Header(short, 3, s)" >$@;
	@echo "templant -I$(templates) $@"; \
	$(templant) -I$(templates) $@;
	@touch $@;
	@rm -f $@.bak;

SbVec3f.h: $(templant) $(templates)/SbVec-Header.tpl $(templates)/Copyright.tpl
	echo "//$$ INSERT TEMPLATE SbVec-Header(float, 3, f)" >$@;
	@echo "templant -I$(templates) $@"; \
	$(templant) -I$(templates) $@;
	@touch $@;
	@rm -f $@.bak;

SbVec3d.h: $(templant) $(templates)/SbVec-Header.tpl $(templates)/Copyright.tpl
	echo "//$$ INSERT TEMPLATE SbVec-Header(double, 3, d)" >$@;
	@echo "templant -I$(templates) $@"; \
	$(templant) -I$(templates) $@;
	@touch $@;
	@rm -f $@.bak;

SbVec4s.h: $(templant) $(templates)/SbVec-Header.tpl $(templates)/Copyright.tpl
	echo "//$$ INSERT TEMPLATE SbVec-Header(short, 4, s)" >$@;
	@echo "templant -I$(templates) $@"; \
	$(templant) -I$(templates) $@;
	@touch $@;
	@rm -f $@.bak;

SbVec4f.h: $(templant) $(templates)/SbVec-Header.tpl $(templates)/Copyright.tpl
	echo "//$$ INSERT TEMPLATE SbVec-Header(float, 4, f)" >$@;
	@echo "templant -I$(templates) $@"; \
	$(templant) -I$(templates) $@;
	@touch $@;
	@rm -f $@.bak;

SbVec4d.h: $(templant) $(templates)/SbVec-Header.tpl $(templates)/Copyright.tpl
	echo "//$$ INSERT TEMPLATE SbVec-Header(double, 4, d)" >$@;
	@echo "templant -I$(templates) $@"; \
	$(templant) -I$(templates) $@;
	@touch $@;
	@rm -f $@.bak;

