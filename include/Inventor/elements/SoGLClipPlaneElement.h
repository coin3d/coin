/**************************************************************************\
 * 
 *  Copyright (C) 1998-1999 by Systems in Motion.  All rights reserved.
 *
 *  This file is part of the Coin library.
 *
 *  This file may be distributed under the terms of the Q Public License
 *  as defined by Troll Tech AS of Norway and appearing in the file
 *  LICENSE.QPL included in the packaging of this file.
 *
 *  If you want to use Coin in applications not covered by licenses
 *  compatible with the QPL, you can contact SIM to aquire a
 *  Professional Edition license for Coin.
 *
 *  Systems in Motion AS, Prof. Brochs gate 6, N-7030 Trondheim, NORWAY
 *  http://www.sim.no/ sales@sim.no Voice: +47 22114160 Fax: +47 67172912
 *
\**************************************************************************/

#ifndef _SO_GL_CLIP_PLANE_ELEMENT_H_
#define _SO_GL_CLIP_PLANE_ELEMENT_H_

#include <Inventor/confdep.h>
#if defined(COIN_EXCLUDE_SOGLCLIPPLANEELEMENT)
#error "Configuration settings disrespected -- do not include this file!"
#endif // COIN_EXCLUDE_SOGLCLIPPLANEELEMENT

#include <Inventor/elements/SoClipPlaneElement.h>

class SoGLClipPlaneElement : public SoClipPlaneElement {
  typedef SoClipPlaneElement inherited;

//$ BEGIN TEMPLATE ElementHeader( SoGLClipPlaneElement )
public:
  static SoType classTypeId;
  static SoType getClassTypeId(void);
  static void * createInstance(void);
public:
  static int getClassStackIndex(void);
  virtual ~SoGLClipPlaneElement(void);

  static void initClass(void);
  static void cleanClass(void);

protected:
  SoGLClipPlaneElement(void);
  static int classStackIndex;
//$ END TEMPLATE ElementHeader

public:
  virtual void init(SoState * state);
  virtual void pop(SoState * state,
		   const SoElement * prevTopElement );
  static  int getMaxGLPlanes(void);

protected:
  virtual void addToElt(const SbPlane & plane,
			const SbMatrix & modelMatrix);

private:
  static int maxGLPlanes;

}; // class SoGLClipPlaneElement

#endif // ! _SO_GL_CLIP_PLANE_ELEMENT_H_
