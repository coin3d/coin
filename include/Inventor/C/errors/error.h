#ifndef CC_ERROR_H
#define CC_ERROR_H

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

#include <Inventor/C/base/string.h>

#include <stdarg.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* ********************************************************************** */

struct cc_error {
  cc_string debugstring;
};

typedef  struct cc_error  cc_error;

typedef void cc_error_cb(const cc_error * err, void * data);

/* ********************************************************************** */

void cc_error_construct(cc_error * me);
void cc_error_clean(cc_error * me);

  /*   const SbString & getDebugString(void) const; */
const cc_string * cc_error_get_debug_string(const cc_error * me);

  /* protected: */
  /*   void setDebugString(const char * const str); */

void cc_error_set_debug_string(cc_error * me, const char * str);

  /* protected: */
  /*   void appendToDebugString(const char * const str); */

void cc_error_append_to_debug_string(cc_error * me, const char * str);

  /* protected: */
  /*   void handleError(void); */

void cc_error_handle(cc_error * me);

  /* protected: */
  /*   virtual SoErrorCB * getHandler(void * & data) const; */
cc_error_cb * cc_error_get_handler(void ** data);

/* ********************************************************************** */

/* class COIN_DLL_API SoError { */
/* public: */
/*   static void setHandlerCallback(SoErrorCB * const func, void * const data); */
/*   static SoErrorCB * getHandlerCallback(void); */
/*   static void * getHandlerData(void); */

/*   static SoType getClassTypeId(void); */
/*   virtual SoType getTypeId(void) const; */
/*   SbBool isOfType(const SoType type) const; */

/*   static void post(const char * const format, ...); */

/*   static SbString getString(const SoNode * const node); */
/*   static SbString getString(const SoPath * const path); */
/*   static SbString getString(const SoEngine * const engine); */

/*   static void initClass(void); */
/*   static void initClasses(void); */

/* private: */
/*   static void generateBaseString(SbString & str, const SoBase * const base, */
/*                                  const char * const what); */

/*   static SoType classTypeId; */

/* ********************************************************************** */

#ifdef __cplusplus
} /* extern "C" */
#endif /* __cplusplus */

#endif /* ! CC_ERROR_H */
