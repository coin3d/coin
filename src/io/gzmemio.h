#ifndef COIN_GZMEMIO_H
#define COIN_GZMEMIO_H

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

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif /* HAVE_CONFIG_H */

#include <Inventor/system/inttypes.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

  void * cc_gzm_open(const uint8_t * buffer, uint32_t len);
  int cc_gzm_read (void * file, void * buf, uint32_t len);
  int cc_gzm_getc(void * file);
  char * cc_gzm_gets(void * file, char * buf, int len);
  off_t cc_gzm_seek(void * file, off_t offset, int whence);
  int cc_gzm_rewind(void * file);
  off_t cc_gzm_tell(void * file);
  int cc_gzm_eof(void * file);
  int cc_gzm_close(void * file);
  int cc_gzm_sizeof_z_stream(void);
  

  /*
    the following functions are not implemented yet (writing to a
    buffer is not supported).

    int cc_gzm_write(void * file, void * buf, unsigned int len);
    int cc_gzm_setparams(void * file, int level, int strategy);
    int cc_gzm_putc(void * file, int c);
    int cc_gzm_puts(void * file, const char * s);
  */

#ifdef __cplusplus
} /* extern "C" */
#endif /* __cplusplus */

#endif /* COIN_GZMEMIO_H */
