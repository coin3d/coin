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
  vec4d.h \
  box2s.h \
  box2f.h \
  box2d.h \
  box3s.h \
  box3f.h \
  box3d.h \
  rot4f.h \
  mat4f.h \
  plane3f.h \
  line3f.h

templant = ../../../../scripts/templant
templates = ../../../../templates

all: $(BUILT_SOURCES)

clean:
	rm -f $(BUILT_SOURCES)

box2s.h: $(templant) $(templates)/cc_box_header.tpl $(templates)/Copyright.tpl
	@if test -f $@; then :; else \
	  echo "/*$$ INSERT TEMPLATE cc_box_header(short, 2, s) */" >$@; \
	fi
	@echo "templant -I$(templates) $@"; \
	$(templant) -I$(templates) $@
	@touch $@
	@rm -f $@.bak

box2f.h: $(templant) $(templates)/cc_box_header.tpl $(templates)/Copyright.tpl
	@if test -f $@; then :; else \
	  echo "/*$$ INSERT TEMPLATE cc_box_header(float, 2, f) */" >$@; \
	fi
	@echo "templant -I$(templates) $@"; \
	$(templant) -I$(templates) $@
	@touch $@
	@rm -f $@.bak

box2d.h: $(templant) $(templates)/cc_box_header.tpl $(templates)/Copyright.tpl
	@if test -f $@; then :; else \
	  echo "/*$$ INSERT TEMPLATE cc_box_header(double, 2, d) */" >$@; \
	fi
	@echo "templant -I$(templates) $@"; \
	$(templant) -I$(templates) $@
	@touch $@
	@rm -f $@.bak

box3s.h: $(templant) $(templates)/cc_box_header.tpl $(templates)/Copyright.tpl
	@if test -f $@; then :; else \
	  echo "/*$$ INSERT TEMPLATE cc_box_header(short, 3, s) */" >$@; \
	fi
	@echo "templant -I$(templates) $@"; \
	$(templant) -I$(templates) $@
	@touch $@
	@rm -f $@.bak

box3f.h: $(templant) $(templates)/cc_box_header.tpl $(templates)/Copyright.tpl
	@if test -f $@; then :; else \
	  echo "/*$$ INSERT TEMPLATE cc_box_header(float, 3, f) */" >$@; \
	fi
	@echo "templant -I$(templates) $@"; \
	$(templant) -I$(templates) $@
	@touch $@
	@rm -f $@.bak

box3d.h: $(templant) $(templates)/cc_box_header.tpl $(templates)/Copyright.tpl
	@if test -f $@; then :; else \
	  echo "/*$$ INSERT TEMPLATE cc_box_header(double, 3, d) */" >$@; \
	fi
	@echo "templant -I$(templates) $@"; \
	$(templant) -I$(templates) $@
	@touch $@
	@rm -f $@.bak

vec2s.h: $(templant) $(templates)/cc_vector_header.tpl $(templates)/Copyright.tpl
	@if test -f $@; then :; else \
	  echo "/*$$ INSERT TEMPLATE cc_vector_header(short, 2, s) */" >$@; \
	fi
	@echo "templant -I$(templates) $@"; \
	$(templant) -I$(templates) $@
	@touch $@
	@rm -f $@.bak

vec2f.h: $(templant) $(templates)/cc_vector_header.tpl $(templates)/Copyright.tpl
	@if test -f $@; then :; else \
	  echo "/*$$ INSERT TEMPLATE cc_vector_header(float, 2, f) */" >$@; \
	fi
	@echo "templant -I$(templates) $@"; \
	$(templant) -I$(templates) $@
	@touch $@
	@rm -f $@.bak

vec2d.h: $(templant) $(templates)/cc_vector_header.tpl $(templates)/Copyright.tpl
	@if test -f $@; then :; else \
	  echo "/*$$ INSERT TEMPLATE cc_vector_header(double, 2, d) */" >$@; \
	fi
	@echo "templant -I$(templates) $@"; \
	$(templant) -I$(templates) $@
	@touch $@
	@rm -f $@.bak

vec3s.h: $(templant) $(templates)/cc_vector_header.tpl $(templates)/Copyright.tpl
	@if test -f $@; then :; else \
	  echo "/*$$ INSERT TEMPLATE cc_vector_header(short, 3, s) */" >$@; \
	fi
	@echo "templant -I$(templates) $@"; \
	$(templant) -I$(templates) $@
	@touch $@
	@rm -f $@.bak

vec3f.h: $(templant) $(templates)/cc_vector_header.tpl $(templates)/Copyright.tpl
	@if test -f $@; then :; else \
	  echo "/*$$ INSERT TEMPLATE cc_vector_header(float, 3, f) */" >$@; \
	fi
	@echo "templant -I$(templates) $@"; \
	$(templant) -I$(templates) $@
	@touch $@
	@rm -f $@.bak

vec3d.h: $(templant) $(templates)/cc_vector_header.tpl $(templates)/Copyright.tpl
	@if test -f $@; then :; else \
	  echo "/*$$ INSERT TEMPLATE cc_vector_header(double, 3, d) */" >$@; \
	fi
	@echo "templant -I$(templates) $@"; \
	$(templant) -I$(templates) $@
	@touch $@
	@rm -f $@.bak

vec4s.h: $(templant) $(templates)/cc_vector_header.tpl $(templates)/Copyright.tpl
	@if test -f $@; then :; else \
	  echo "/*$$ INSERT TEMPLATE cc_vector_header(short, 4, s) */" >$@; \
	fi
	@echo "templant -I$(templates) $@"; \
	$(templant) -I$(templates) $@
	@touch $@
	@rm -f $@.bak

vec4f.h: $(templant) $(templates)/cc_vector_header.tpl $(templates)/Copyright.tpl
	@if test -f $@; then :; else \
	  echo "/*$$ INSERT TEMPLATE cc_vector_header(float, 4, f) */" >$@; \
	fi
	@echo "templant -I$(templates) $@"; \
	$(templant) -I$(templates) $@
	@touch $@
	@rm -f $@.bak

vec4d.h: $(templant) $(templates)/cc_vector_header.tpl $(templates)/Copyright.tpl
	@if test -f $@; then :; else \
	  echo "/*$$ INSERT TEMPLATE cc_vector_header(double, 4, d) */" >$@; \
	fi
	@echo "templant -I$(templates) $@"; \
	$(templant) -I$(templates) $@
	@touch $@
	@rm -f $@.bak

rot4f.h: $(templant) $(templates)/cc_rotation_header.tpl $(templates)/Copyright.tpl
	@if test -f $@; then :; else \
	  echo "/*$$ INSERT TEMPLATE cc_rotation_header(float, 4, f) */" >$@; \
	fi
	@echo "templant -I$(templates) $@"; \
	$(templant) -I$(templates) $@
	@touch $@
	@rm -f $@.bak

mat4f.h: $(templant) $(templates)/cc_matrix_header.tpl $(templates)/Copyright.tpl
	@if test -f $@; then :; else \
	  echo "/*$$ INSERT TEMPLATE cc_matrix_header(float, 4, f) */" >$@; \
	fi
	@echo "templant -I$(templates) $@"; \
	$(templant) -I$(templates) $@
	@touch $@
	@rm -f $@.bak

plane3f.h: $(templant) $(templates)/cc_plane_header.tpl $(templates)/Copyright.tpl
	@if test -f $@; then :; else \
	  echo "/*$$ INSERT TEMPLATE cc_plane_header(float, 3, f) */" >$@; \
	fi
	@echo "templant -I$(templates) $@"; \
	$(templant) -I$(templates) $@
	@touch $@
	@rm -f $@.bak

line3f.h: $(templant) $(templates)/cc_line_header.tpl $(templates)/Copyright.tpl
	@if test -f $@; then :; else \
	  echo "/*$$ INSERT TEMPLATE cc_line_header(float, 3, f) */" >$@; \
	fi
	@echo "templant -I$(templates) $@"; \
	$(templant) -I$(templates) $@
	@touch $@
	@rm -f $@.bak

