#! /usr/bin/make -f

.PHONY: all

BUILT_SOURCES = \
  vec2s.c vec2f.c vec2d.c \
  vec3s.c vec3f.c vec3d.c \
  vec4s.c vec4f.c vec4d.c \
  box2s.c box2f.c box2d.c \
  box3s.c box3f.c box3d.c \
  mat4f.c \
  line3f.c \
  plane3f.c \
  rot4f.c \
  $(BUILT_DOCS)

BUILT_DOCS = \
  SbVec2s.dox SbVec2f.dox SbVec2d.dox \
  SbVec3s.dox SbVec3f.dox SbVec3d.dox \
  SbVec4s.dox SbVec4f.dox SbVec4d.dox \
  SbBox2s.dox SbBox2f.dox SbBox2d.dox \
  SbBox3s.dox SbBox3f.dox SbBox3d.dox \
  SbMatrix.dox \
  SbLine.dox \
  SbPlane.dox \
  SbRotation.dox \

templant = ../../scripts/templant
templates = ../../templates

all: $(BUILT_SOURCES)

clean:
	rm -f $(BUILT_SOURCES)

box2s.c: $(templant) $(templates)/cc_box_source.tpl $(templates)/Copyright.tpl
	@if test -f $@; then :; else \
	  echo "/*$$ INSERT TEMPLATE cc_box_source(short, 2, s) */" >$@; \
	fi
	@echo "templant -I$(templates) $@"; \
	$(templant) -I$(templates) $@
	@touch $@
	@rm -f $@.bak

box2f.c: $(templant) $(templates)/cc_box_source.tpl $(templates)/Copyright.tpl
	@if test -f $@; then :; else \
	  echo "/*$$ INSERT TEMPLATE cc_box_source(float, 2, f) */" >$@; \
	fi
	@echo "templant -I$(templates) $@"; \
	$(templant) -I$(templates) $@
	@touch $@
	@rm -f $@.bak

box2d.c: $(templant) $(templates)/cc_box_source.tpl $(templates)/Copyright.tpl
	@if test -f $@; then :; else \
	  echo "/*$$ INSERT TEMPLATE cc_box_source(double, 2, d) */" >$@; \
	fi
	@echo "templant -I$(templates) $@"; \
	$(templant) -I$(templates) $@
	@touch $@
	@rm -f $@.bak

box3s.c: $(templant) $(templates)/cc_box_source.tpl $(templates)/Copyright.tpl
	@if test -f $@; then :; else \
	  echo "/*$$ INSERT TEMPLATE cc_box_source(short, 3, s) */" >$@; \
	fi
	@echo "templant -I$(templates) $@"; \
	$(templant) -I$(templates) $@
	@touch $@
	@rm -f $@.bak

box3f.c: $(templant) $(templates)/cc_box_source.tpl $(templates)/Copyright.tpl
	@if test -f $@; then :; else \
	  echo "/*$$ INSERT TEMPLATE cc_box_source(float, 3, f) */" >$@; \
	fi
	@echo "templant -I$(templates) $@"; \
	$(templant) -I$(templates) $@
	@touch $@
	@rm -f $@.bak

box3d.c: $(templant) $(templates)/cc_box_source.tpl $(templates)/Copyright.tpl
	@if test -f $@; then :; else \
	  echo "/*$$ INSERT TEMPLATE cc_box_source(double, 3, d) */" >$@; \
	fi
	@echo "templant -I$(templates) $@"; \
	$(templant) -I$(templates) $@
	@touch $@
	@rm -f $@.bak

vec2s.c: $(templant) $(templates)/cc_vector_source.tpl $(templates)/Copyright.tpl
	@if test -f $@; then :; else \
	  echo "/*$$ INSERT TEMPLATE cc_vector_source(short, 2, s) */" >$@; \
	fi
	@echo "templant -I$(templates) $@"; \
	$(templant) -I$(templates) $@
	@touch $@
	@rm -f $@.bak

vec2f.c: $(templant) $(templates)/cc_vector_source.tpl $(templates)/Copyright.tpl
	@if test -f $@; then :; else \
	  echo "/*$$ INSERT TEMPLATE cc_vector_source(float, 2, f) */" >$@; \
	fi
	@echo "templant -I$(templates) $@"; \
	$(templant) -I$(templates) $@
	@touch $@
	@rm -f $@.bak

vec2d.c: $(templant) $(templates)/cc_vector_source.tpl $(templates)/Copyright.tpl
	@if test -f $@; then :; else \
	  echo "/*$$ INSERT TEMPLATE cc_vector_source(double, 2, d) */" >$@; \
	fi
	@echo "templant -I$(templates) $@"; \
	$(templant) -I$(templates) $@
	@touch $@
	@rm -f $@.bak

vec3s.c: $(templant) $(templates)/cc_vector_source.tpl $(templates)/Copyright.tpl
	@if test -f $@; then :; else \
	  echo "/*$$ INSERT TEMPLATE cc_vector_source(short, 3, s) */" >$@; \
	fi
	@echo "templant -I$(templates) $@"; \
	$(templant) -I$(templates) $@
	@touch $@
	@rm -f $@.bak

vec3f.c: $(templant) $(templates)/cc_vector_source.tpl $(templates)/Copyright.tpl
	@if test -f $@; then :; else \
	  echo "/*$$ INSERT TEMPLATE cc_vector_source(float, 3, f) */" >$@; \
	fi
	@echo "templant -I$(templates) $@"; \
	$(templant) -I$(templates) $@
	@touch $@
	@rm -f $@.bak

vec3d.c: $(templant) $(templates)/cc_vector_source.tpl $(templates)/Copyright.tpl
	@if test -f $@; then :; else \
	  echo "/*$$ INSERT TEMPLATE cc_vector_source(double, 3, d) */" >$@; \
	fi
	@echo "templant -I$(templates) $@"; \
	$(templant) -I$(templates) $@
	@touch $@
	@rm -f $@.bak

vec4s.c: $(templant) $(templates)/cc_vector_source.tpl $(templates)/Copyright.tpl
	@if test -f $@; then :; else \
	  echo "/*$$ INSERT TEMPLATE cc_vector_source(short, 4, s) */" >$@; \
	fi
	@echo "templant -I$(templates) $@"; \
	$(templant) -I$(templates) $@
	@touch $@
	@rm -f $@.bak

vec4f.c: $(templant) $(templates)/cc_vector_source.tpl $(templates)/Copyright.tpl
	@if test -f $@; then :; else \
	  echo "/*$$ INSERT TEMPLATE cc_vector_source(float, 4, f) */" >$@; \
	fi
	@echo "templant -I$(templates) $@"; \
	$(templant) -I$(templates) $@
	@touch $@
	@rm -f $@.bak

vec4d.c: $(templant) $(templates)/cc_vector_source.tpl $(templates)/Copyright.tpl
	@if test -f $@; then :; else \
	  echo "/*$$ INSERT TEMPLATE cc_vector_source(double, 4, d) */" >$@; \
	fi
	@echo "templant -I$(templates) $@"; \
	$(templant) -I$(templates) $@
	@touch $@
	@rm -f $@.bak

mat4f.c: $(templant) $(templates)/cc_matrix_source.tpl $(templates)/Copyright.tpl
	@if test -f $@; then :; else \
	  echo "/*$$ INSERT TEMPLATE cc_matrix_source(float, 4, f) */" >$@; \
	fi
	@echo "templant -I$(templates) $@"; \
	$(templant) -I$(templates) $@
	@touch $@
	@rm -f $@.bak

line3f.c: $(templant) $(templates)/cc_line_source.tpl $(templates)/Copyright.tpl
	@if test -f $@; then :; else \
	  echo "/*$$ INSERT TEMPLATE cc_line_source(float, 3, f) */" >$@; \
	fi
	@echo "templant -I$(templates) $@"; \
	$(templant) -I$(templates) $@
	@touch $@
	@rm -f $@.bak

plane3f.c: $(templant) $(templates)/cc_plane_source.tpl $(templates)/Copyright.tpl
	@if test -f $@; then :; else \
	  echo "/*$$ INSERT TEMPLATE cc_plane_source(float, 3, f) */" >$@; \
	fi
	@echo "templant -I$(templates) $@"; \
	$(templant) -I$(templates) $@
	@touch $@
	@rm -f $@.bak

rot4f.c: $(templant) $(templates)/cc_rotation_source.tpl $(templates)/Copyright.tpl
	@if test -f $@; then :; else \
	  echo "/*$$ INSERT TEMPLATE cc_rotation_source(float, 4, f) */" >$@; \
	fi
	@echo "templant -I$(templates) $@"; \
	$(templant) -I$(templates) $@
	@touch $@
	@rm -f $@.bak

## DOXYGEN FILES

SbBox2s.dox: $(templant) $(templates)/SbBox-Doxygen.tpl $(templates)/Copyright.tpl
	@if test -f $@; then :; else \
	  echo "/*$$ INSERT TEMPLATE SbBox-Doxygen(short, 2, s) */" >$@; \
	fi
	@echo "templant -I$(templates) $@"; \
	$(templant) -I$(templates) $@
	@touch $@
	@rm -f $@.bak

SbBox2f.dox: $(templant) $(templates)/SbBox-Doxygen.tpl $(templates)/Copyright.tpl
	@if test -f $@; then :; else \
	  echo "/*$$ INSERT TEMPLATE SbBox-Doxygen(float, 2, f) */" >$@; \
	fi
	@echo "templant -I$(templates) $@"; \
	$(templant) -I$(templates) $@
	@touch $@
	@rm -f $@.bak

SbBox2d.dox: $(templant) $(templates)/SbBox-Doxygen.tpl $(templates)/Copyright.tpl
	@if test -f $@; then :; else \
	  echo "/*$$ INSERT TEMPLATE SbBox-Doxygen(double, 2, d) */" >$@; \
	fi
	@echo "templant -I$(templates) $@"; \
	$(templant) -I$(templates) $@
	@touch $@
	@rm -f $@.bak

SbBox3s.dox: $(templant) $(templates)/SbBox-Doxygen.tpl $(templates)/Copyright.tpl
	@if test -f $@; then :; else \
	  echo "/*$$ INSERT TEMPLATE SbBox-Doxygen(short, 3, s) */" >$@; \
	fi
	@echo "templant -I$(templates) $@"; \
	$(templant) -I$(templates) $@
	@touch $@
	@rm -f $@.bak

SbBox3f.dox: $(templant) $(templates)/SbBox-Doxygen.tpl $(templates)/Copyright.tpl
	@if test -f $@; then :; else \
	  echo "/*$$ INSERT TEMPLATE SbBox-Doxygen(float, 3, f) */" >$@; \
	fi
	@echo "templant -I$(templates) $@"; \
	$(templant) -I$(templates) $@
	@touch $@
	@rm -f $@.bak

SbBox3d.dox: $(templant) $(templates)/SbBox-Doxygen.tpl $(templates)/Copyright.tpl
	@if test -f $@; then :; else \
	  echo "/*$$ INSERT TEMPLATE SbBox-Doxygen(double, 3, d) */" >$@; \
	fi
	@echo "templant -I$(templates) $@"; \
	$(templant) -I$(templates) $@
	@touch $@
	@rm -f $@.bak

SbVec2s.dox: $(templant) $(templates)/SbVector-Doxygen.tpl $(templates)/Copyright.tpl
	@if test -f $@; then :; else \
	  echo "/*$$ INSERT TEMPLATE SbVector-Doxygen(short, 2, s) */" >$@; \
	fi
	@echo "templant -I$(templates) $@"; \
	$(templant) -I$(templates) $@
	@touch $@
	@rm -f $@.bak

SbVec2f.dox: $(templant) $(templates)/SbVector-Doxygen.tpl $(templates)/Copyright.tpl
	@if test -f $@; then :; else \
	  echo "/*$$ INSERT TEMPLATE SbVector-Doxygen(float, 2, f) */" >$@; \
	fi
	@echo "templant -I$(templates) $@"; \
	$(templant) -I$(templates) $@
	@touch $@
	@rm -f $@.bak

SbVec2d.dox: $(templant) $(templates)/SbVector-Doxygen.tpl $(templates)/Copyright.tpl
	@if test -f $@; then :; else \
	  echo "/*$$ INSERT TEMPLATE SbVector-Doxygen(double, 2, d) */" >$@; \
	fi
	@echo "templant -I$(templates) $@"; \
	$(templant) -I$(templates) $@
	@touch $@
	@rm -f $@.bak

SbVec3s.dox: $(templant) $(templates)/SbVector-Doxygen.tpl $(templates)/Copyright.tpl
	@if test -f $@; then :; else \
	  echo "/*$$ INSERT TEMPLATE SbVector-Doxygen(short, 3, s) */" >$@; \
	fi
	@echo "templant -I$(templates) $@"; \
	$(templant) -I$(templates) $@
	@touch $@
	@rm -f $@.bak

SbVec3f.dox: $(templant) $(templates)/SbVector-Doxygen.tpl $(templates)/Copyright.tpl
	@if test -f $@; then :; else \
	  echo "/*$$ INSERT TEMPLATE SbVector-Doxygen(float, 3, f) */" >$@; \
	fi
	@echo "templant -I$(templates) $@"; \
	$(templant) -I$(templates) $@
	@touch $@
	@rm -f $@.bak

SbVec3d.dox: $(templant) $(templates)/SbVector-Doxygen.tpl $(templates)/Copyright.tpl
	@if test -f $@; then :; else \
	  echo "/*$$ INSERT TEMPLATE SbVector-Doxygen(double, 3, d) */" >$@; \
	fi
	@echo "templant -I$(templates) $@"; \
	$(templant) -I$(templates) $@
	@touch $@
	@rm -f $@.bak

SbVec4s.dox: $(templant) $(templates)/SbVector-Doxygen.tpl $(templates)/Copyright.tpl
	@if test -f $@; then :; else \
	  echo "/*$$ INSERT TEMPLATE SbVector-Doxygen(short, 4, s) */" >$@; \
	fi
	@echo "templant -I$(templates) $@"; \
	$(templant) -I$(templates) $@
	@touch $@
	@rm -f $@.bak

SbVec4f.dox: $(templant) $(templates)/SbVector-Doxygen.tpl $(templates)/Copyright.tpl
	@if test -f $@; then :; else \
	  echo "/*$$ INSERT TEMPLATE SbVector-Doxygen(float, 4, f) */" >$@; \
	fi
	@echo "templant -I$(templates) $@"; \
	$(templant) -I$(templates) $@
	@touch $@
	@rm -f $@.bak

SbVec4d.dox: $(templant) $(templates)/SbVector-Doxygen.tpl $(templates)/Copyright.tpl
	@if test -f $@; then :; else \
	  echo "/*$$ INSERT TEMPLATE SbVector-Doxygen(double, 4, d) */" >$@; \
	fi
	@echo "templant -I$(templates) $@"; \
	$(templant) -I$(templates) $@
	@touch $@
	@rm -f $@.bak

SbMatrix.dox: $(templant) $(templates)/SbMatrix-Doxygen.tpl $(templates)/Copyright.tpl
	@if test -f $@; then :; else \
	  echo "/*$$ INSERT TEMPLATE SbMatrix-Doxygen(float, 4, f) */" >$@; \
	fi
	@echo "templant -I$(templates) $@"; \
	$(templant) -I$(templates) $@
	@touch $@
	@rm -f $@.bak

SbLine.dox: $(templant) $(templates)/SbLine-Doxygen.tpl $(templates)/Copyright.tpl
	@if test -f $@; then :; else \
	  echo "/*$$ INSERT TEMPLATE SbLine-Doxygen(float, 3, f) */" >$@; \
	fi
	@echo "templant -I$(templates) $@"; \
	$(templant) -I$(templates) $@
	@touch $@
	@rm -f $@.bak

SbPlane.dox: $(templant) $(templates)/SbPlane-Doxygen.tpl $(templates)/Copyright.tpl
	@if test -f $@; then :; else \
	  echo "/*$$ INSERT TEMPLATE SbPlane-Doxygen(float, 3, f) */" >$@; \
	fi
	@echo "templant -I$(templates) $@"; \
	$(templant) -I$(templates) $@
	@touch $@
	@rm -f $@.bak

SbRotation.dox: $(templant) $(templates)/SbRotation-Doxygen.tpl $(templates)/Copyright.tpl
	@if test -f $@; then :; else \
	  echo "/*$$ INSERT TEMPLATE SbRotation-Doxygen(float, 4, f) */" >$@; \
	fi
	@echo "templant -I$(templates) $@"; \
	$(templant) -I$(templates) $@
	@touch $@
	@rm -f $@.bak

