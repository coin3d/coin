#! /usr/bin/make -f

.PHONY: all

BUILT_SOURCES = \
  vec2s.c \
  vec2f.c \
  vec2d.c \
  vec3s.c \
  vec3f.c \
  vec3d.c \
  vec4s.c \
  vec4f.c \
  vec4d.c

templant = ../../scripts/templant
templates = ../../templates

all: $(BUILT_SOURCES)

clean:
	rm -f $(BUILT_SOURCES)

vec2s.c: $(templant) $(templates)/cc_vec_source.tpl $(templates)/Copyright.tpl
	echo "/*$$ INSERT TEMPLATE cc_vec_source(short, 2, s) */" >$@
	@echo "templant -I$(templates) $@"; \
	$(templant) -I$(templates) $@
	@touch $@
	@rm -f $@.bak

vec2f.c: $(templant) $(templates)/cc_vec_source.tpl $(templates)/Copyright.tpl
	echo "/*$$ INSERT TEMPLATE cc_vec_source(float, 2, f) */" >$@
	@echo "templant -I$(templates) $@"; \
	$(templant) -I$(templates) $@
	@touch $@
	@rm -f $@.bak

vec2d.c: $(templant) $(templates)/cc_vec_source.tpl $(templates)/Copyright.tpl
	echo "/*$$ INSERT TEMPLATE cc_vec_source(double, 2, d) */" >$@
	@echo "templant -I$(templates) $@"; \
	$(templant) -I$(templates) $@
	@touch $@
	@rm -f $@.bak

vec3s.c: $(templant) $(templates)/cc_vec_source.tpl $(templates)/Copyright.tpl
	echo "/*$$ INSERT TEMPLATE cc_vec_source(short, 3, s) */" >$@
	@echo "templant -I$(templates) $@"; \
	$(templant) -I$(templates) $@
	@touch $@
	@rm -f $@.bak

vec3f.c: $(templant) $(templates)/cc_vec_source.tpl $(templates)/Copyright.tpl
	echo "/*$$ INSERT TEMPLATE cc_vec_source(float, 3, f) */" >$@
	@echo "templant -I$(templates) $@"; \
	$(templant) -I$(templates) $@
	@touch $@
	@rm -f $@.bak

vec3d.c: $(templant) $(templates)/cc_vec_source.tpl $(templates)/Copyright.tpl
	echo "/*$$ INSERT TEMPLATE cc_vec_source(double, 3, d) */" >$@
	@echo "templant -I$(templates) $@"; \
	$(templant) -I$(templates) $@
	@touch $@
	@rm -f $@.bak

vec4s.c: $(templant) $(templates)/cc_vec_source.tpl $(templates)/Copyright.tpl
	echo "/*$$ INSERT TEMPLATE cc_vec_source(short, 4, s) */" >$@
	@echo "templant -I$(templates) $@"; \
	$(templant) -I$(templates) $@
	@touch $@
	@rm -f $@.bak

vec4f.c: $(templant) $(templates)/cc_vec_source.tpl $(templates)/Copyright.tpl
	echo "/*$$ INSERT TEMPLATE cc_vec_source(float, 4, f) */" >$@
	@echo "templant -I$(templates) $@"; \
	$(templant) -I$(templates) $@
	@touch $@
	@rm -f $@.bak

vec4d.c: $(templant) $(templates)/cc_vec_source.tpl $(templates)/Copyright.tpl
	echo "/*$$ INSERT TEMPLATE cc_vec_source(double, 4, d) */" >$@
	@echo "templant -I$(templates) $@"; \
	$(templant) -I$(templates) $@
	@touch $@
	@rm -f $@.bak

