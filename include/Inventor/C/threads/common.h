#ifndef CC_THREADCOMMON_H
#define CC_THREADCOMMON_H

/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2001 by Systems in Motion.  All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License version 2 as
 *  published by the Free Software Foundation.  See the file LICENSE.GPL
 *  at the root directory of this source distribution for more details.
 *
 *  If you desire to use Coin with software that is incompatible
 *  licensewise with the GPL, and / or you would like to take
 *  advantage of the additional benefits with regard to our support
 *  services, please contact Systems in Motion about acquiring a Coin
 *  Professional Edition License.
 *
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY
 *  www.sim.no, support@sim.no, Voice: +47 22114160, Fax: +47 22207097
 *
\**************************************************************************/

#include <Inventor/C/basic.h>  /* COIN_DLL_API */

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* ********************************************************************** */

  typedef  struct cc_thread cc_thread;
  typedef  struct cc_mutex cc_mutex;
  typedef  struct cc_rwmutex cc_rwmutex;
  typedef  struct cc_condvar cc_condvar;
  typedef  struct cc_barrier cc_barrier;
  typedef  struct cc_storage cc_storage;
  
  /* used by rwmutex - read_precedence is default */
  enum cc_precedence {
    CC_READ_PRECEDENCE,
    CC_WRITE_PRECEDENCE
  };

  enum cc_threads_implementation {
    CC_NO_THREADS = -1,
    CC_PTHREAD    = 0,
    CC_W32THREAD
  };

  enum cc_retval {
    CC_ERROR = 0,
    CC_OK = 1,
    CC_TIMEOUT,
    CC_BUSY
  };
  
  typedef enum cc_precedence cc_precedence;
  typedef enum cc_threads_implementation cc_threads_implementation;
  typedef enum cc_retval cc_retval;
  
  /* ********************************************************************** */
  
  COIN_DLL_API int cc_thread_implementation(void);
  
  /* ********************************************************************** */
  
#ifdef __cplusplus
} /* extern "C" */
#endif /* __cplusplus */

#endif /* ! CC_THREADCOMMON_H */
