#! /usr/bin/make -f

.PHONY: all clean

BUILT_SOURCES = \
  vec2s.h \
  vec2f.h \
  vec2d.h \
  vec3s.h \
  vec3f.h \
  vec3d.h \
  vec4s.h \
  vec4f.h \
  vec4d.h

templant = ../../../../scripts/templant
templates = ../../../../templates

all: $(BUILT_SOURCES)

clean:
	rm -f $(BUILT_SOURCES)

vec2s.h: $(templant) $(templates)/cc_vec_header.tpl $(templates)/Copyright.tpl
	echo "/*$$ INSERT TEMPLATE cc_vec_header(short, 2, s) */" >$@
	@echo "templant -I$(templates) $@"; \
	$(templant) -I$(templates) $@
	@touch $@
	@rm -f $@.bak

vec2f.h: $(templant) $(templates)/cc_vec_header.tpl $(templates)/Copyright.tpl
	echo "/*$$ INSERT TEMPLATE cc_vec_header(float, 2, f) */" >$@
	@echo "templant -I$(templates) $@"; \
	$(templant) -I$(templates) $@
	@touch $@
	@rm -f $@.bak

vec2d.h: $(templant) $(templates)/cc_vec_header.tpl $(templates)/Copyright.tpl
	echo "/*$$ INSERT TEMPLATE cc_vec_header(double, 2, d) */" >$@
	@echo "templant -I$(templates) $@"; \
	$(templant) -I$(templates) $@
	@touch $@
	@rm -f $@.bak

vec3s.h: $(templant) $(templates)/cc_vec_header.tpl $(templates)/Copyright.tpl
	echo "/*$$ INSERT TEMPLATE cc_vec_header(short, 3, s) */" >$@
	@echo "templant -I$(templates) $@"; \
	$(templant) -I$(templates) $@
	@touch $@
	@rm -f $@.bak

vec3f.h: $(templant) $(templates)/cc_vec_header.tpl $(templates)/Copyright.tpl
	echo "/*$$ INSERT TEMPLATE cc_vec_header(float, 3, f) */" >$@
	@echo "templant -I$(templates) $@"; \
	$(templant) -I$(templates) $@
	@touch $@
	@rm -f $@.bak

vec3d.h: $(templant) $(templates)/cc_vec_header.tpl $(templates)/Copyright.tpl
	echo "/*$$ INSERT TEMPLATE cc_vec_header(double, 3, d) */" >$@
	@echo "templant -I$(templates) $@"; \
	$(templant) -I$(templates) $@
	@touch $@
	@rm -f $@.bak

vec4s.h: $(templant) $(templates)/cc_vec_header.tpl $(templates)/Copyright.tpl
	echo "/*$$ INSERT TEMPLATE cc_vec_header(short, 4, s) */" >$@
	@echo "templant -I$(templates) $@"; \
	$(templant) -I$(templates) $@
	@touch $@
	@rm -f $@.bak

vec4f.h: $(templant) $(templates)/cc_vec_header.tpl $(templates)/Copyright.tpl
	echo "/*$$ INSERT TEMPLATE cc_vec_header(float, 4, f) */" >$@
	@echo "templant -I$(templates) $@"; \
	$(templant) -I$(templates) $@
	@touch $@
	@rm -f $@.bak

vec4d.h: $(templant) $(templates)/cc_vec_header.tpl $(templates)/Copyright.tpl
	echo "/*$$ INSERT TEMPLATE cc_vec_header(double, 4, d) */" >$@
	@echo "templant -I$(templates) $@"; \
	$(templant) -I$(templates) $@
	@touch $@
	@rm -f $@.bak

