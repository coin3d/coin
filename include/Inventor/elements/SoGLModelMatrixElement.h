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

#ifndef _SO_GL_MODEL_MATRIX_ELEMENT_H_
#define _SO_GL_MODEL_MATRIX_ELEMENT_H_

#include <Inventor/confdep.h>
#if defined(COIN_EXCLUDE_SOGLMODELMATRIXELEMENT)
#error "Configuration settings disrespected -- do not include this file!"
#endif // COIN_EXCLUDE_SOGLMODELMATRIXELEMENT

#include <Inventor/elements/SoModelMatrixElement.h>

class SoGLModelMatrixElement : public SoModelMatrixElement {
  typedef SoModelMatrixElement inherited;

//$ BEGIN TEMPLATE ElementHeader( SoGLModelMatrixElement )
public:
  static SoType classTypeId;
  static SoType getClassTypeId(void);
  static void * createInstance(void);
public:
  static int getClassStackIndex(void);
  virtual ~SoGLModelMatrixElement(void);

  static void initClass(void);
  static void cleanClass(void);

protected:
  SoGLModelMatrixElement(void);
  static int classStackIndex;
//$ END TEMPLATE ElementHeader

public:
  virtual void init(SoState * state);
  
  virtual void push(SoState * state);
  virtual void pop(SoState * state,
		   const SoElement * prevTopElement);

protected:
  virtual void makeEltIdentity();
  virtual void setElt(const SbMatrix & matrix);
  virtual void multElt(const SbMatrix & matrix);

  virtual void translateEltBy(const SbVec3f & translation);
  virtual void rotateEltBy(const SbRotation & translation);
  virtual void scaleEltBy(const SbVec3f & scaleFactor);
  virtual SbMatrix pushMatrixElt();
  virtual void popMatrixElt(const SbMatrix & matrix);
 
private:
  SoState * state;
  uint32_t viewEltNodeId;

}; // class SoGLModelMatrixElement

#endif // ! _SO_GL_MODEL_MATRIX_ELEMENT_H_
