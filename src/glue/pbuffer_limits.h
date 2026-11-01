#ifndef COIN_GLUE_PBUFFER_LIMITS_H
#define COIN_GLUE_PBUFFER_LIMITS_H

/**************************************************************************\
 * Copyright (c) Kongsberg Oil & Gas Technologies AS
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 * Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 *
 * Redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the distribution.
 *
 * Neither the name of the copyright holder nor the names of its
 * contributors may be used to endorse or promote products derived from
 * this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
\**************************************************************************/


/* Private, context-independent part of the offscreen size-limit query. */
static inline void
coin_glglue_clamp_pbuffer_dimensions(unsigned int & width,
                                    unsigned int & height,
                                    const unsigned int limits[3])
{
  width = width < limits[0] ? width : limits[0];
  height = height < limits[1] ? height : limits[1];

  /* Some GLX implementations report zero pixels despite supporting usable
     pbuffers (for example XQuartz). Zero is not a specified unlimited value;
     disregard this unusable pixel-count hint, keeping the dimension limits.
     The caller still applies its conservative tile-size bound, and allocation
     failures are handled by SoOffscreenRenderer's smaller-buffer retries. */
  if (limits[2] == 0) return;

  int modulo = 0;
  while ((width * height) > limits[2]) {
    if (modulo % 2) { width /= 2; }
    else { height /= 2; }
    modulo++;
  }
}

#endif /* COIN_GLUE_PBUFFER_LIMITS_H */
