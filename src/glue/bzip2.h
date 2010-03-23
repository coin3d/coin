#ifndef COIN_GLUE_BZIP2_H
#define COIN_GLUE_BZIP2_H

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
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#if 0 /* to get proper auto-indentation in emacs */
}
#endif /* emacs indentation */

int cc_bzglue_available(void);

const char * cc_bzglue_BZ2_bzlibVersion(void);
void * cc_bzglue_BZ2_bzReadOpen(int * bzerror,
                                FILE * f,
                                int verbosity,
                                int bzsmall,
                                void * unused,
                                int nunused);
void cc_bzglue_BZ2_bzReadClose(int * bzerror, 
                               void * bzfile); 
int cc_bzglue_BZ2_bzRead(int * bzerror, 
                         void * bzfile, 
                         void * buf, 
                         int len);
void * cc_bzglue_BZ2_bzWriteOpen(int * bzerror,      
                                 FILE * fp, 
                                 int blocksize100k, 
                                 int verbosity, 
                                 int workfactor); 
void cc_bzglue_BZ2_bzWriteClose(int * bzerror, 
                                void * bzfile, 
                                int abandon, 
                                unsigned int * nbytesin, 
                                unsigned int * nbytesout);
void cc_bzglue_BZ2_bzWrite(int * bzerror, 
                           void * bzfile, 
                           void * buf, 
                           int len);
#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* !COIN_GLUE_BZIP2_H */
