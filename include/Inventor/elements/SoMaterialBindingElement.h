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

#ifndef _SO_MATERIAL_BINDING_ELEMENT_H_
#define _SO_MATERIAL_BINDING_ELEMENT_H_

#include <Inventor/confdep.h>
#if defined(COIN_EXCLUDE_SOMATERIALBINDINGELEMENT)
#error "Configuration settings disable this class!"
#endif // COIN_EXCLUDE_SOMATERIALBINDINGELEMENT

#include <Inventor/elements/SoInt32Element.h>

class SoMaterialBindingElement : public SoInt32Element {
  typedef SoInt32Element inherited;

//$ BEGIN TEMPLATE ElementHeader( SoMaterialBindingElement )
public:
  static SoType classTypeId;
  static SoType getClassTypeId(void);
  static void * createInstance(void);
public:
  static int getClassStackIndex(void);
  virtual ~SoMaterialBindingElement(void);

  static void initClass(void);
  static void cleanClass(void);

protected:
  SoMaterialBindingElement(void);
  static int classStackIndex;
//$ END TEMPLATE ElementHeader

public:
// constants: (numbers are adjusted for Inventor 2.0 binary format files)
  enum Binding {
    OVERALL = 2,
    PER_PART = 3,
    PER_PART_INDEXED = 4,
    PER_FACE = 5,
    PER_FACE_INDEXED = 6,
    PER_VERTEX = 7,
    PER_VERTEX_INDEXED = 8,
    DEFAULT = OVERALL,
    NONE = OVERALL
  }; // enum Binding
    
  virtual void init(SoState * state);

  static  void set(SoState * const state, SoNode * const node,
		   const Binding binding);
  static  void set(SoState * const state, const Binding binding);
  static  Binding get(SoState * const state);
  static  Binding getDefault();

}; // class SoMaterialBindingElement

#endif // ! _SO_MATERIAL_BINDING_ELEMENT_H_
