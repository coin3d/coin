#ifndef COIN_GLUE_ZLIB_H
#define COIN_GLUE_ZLIB_H

/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) by Kongsberg Oil & Gas Technologies.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  ("GPL") version 2 as published by the Free Software Foundation.
 *  See the file LICENSE.GPL at the root directory of this source
 *  distribution for additional information about the GNU GPL.
 *
 *  For using Coin with software that can not be combined with the GNU
 *  GPL, and for taking advantage of the additional benefits of our
 *  support services, please contact Kongsberg Oil & Gas Technologies
 *  about acquiring a Coin Professional Edition License.
 *
 *  See http://www.coin3d.org/ for more information.
 *
 *  Kongsberg Oil & Gas Technologies, Bygdoy Alle 5, 0257 Oslo, NORWAY.
 *  http://www.sim.no/  sales@sim.no  coin-support@coin3d.org
 *
\**************************************************************************/

#ifndef COIN_INTERNAL
#error this is a private header file
#endif

#include <Inventor/C/basic.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#if 0 /* to get proper auto-indentation in emacs */
}
#endif /* emacs indentation */

int cc_zlibglue_available(void);
int cc_zlibglue_deflateInit2(void * stream,
                             int level,
                             int method,
                             int windowbits,
                             int memlevel,
                             int strategy);

int cc_zlibglue_inflateInit2(void * stream,
                             int windowbits);

int cc_zlibglue_deflateEnd(void * stream);
int cc_zlibglue_inflateEnd(void * stream);
int cc_zlibglue_inflate(void * stream, int flush);
int cc_zlibglue_inflateReset(void * stream);
int cc_zlibglue_deflateParams(void * stream, int level, int strategy);
int cc_zlibglue_deflate(void * stream, int flush);

void * cc_zlibglue_gzopen(const char * path, const char * mode);
void * cc_zlibglue_gzdopen(int fd, const char * mode);
int cc_zlibglue_gzsetparams(void * fp, int level, int strategy);
int cc_zlibglue_gzread(void * fp, void * buf, unsigned int len);
int cc_zlibglue_gzwrite(void * fp, const void * buf, unsigned int len);
off_t cc_zlibglue_gzseek(void * fp, off_t offset, int whence);
int cc_zlibglue_gzrewind(void * fp);
off_t cc_zlibglue_gztell(void * fp);
int cc_zlibglue_gzeof(void * fp);
int cc_zlibglue_gzclose(void * fp);
int cc_zlibglue_crc32(unsigned long crc, const char * buf, unsigned int len);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* !COIN_GLUE_ZLIB_H */
