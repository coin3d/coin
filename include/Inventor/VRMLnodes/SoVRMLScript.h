/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 2001 by Systems in Motion. All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public License
 *  version 2.1 as published by the Free Software Foundation. See the
 *  file LICENSE.LGPL at the root directory of the distribution for
 *  more details.
 *
 *  If you want to use Coin for applications not compatible with the
 *  LGPL, please contact SIM to acquire a Professional Edition license.
 *
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY
 *  http://www.sim.no support@sim.no Voice: +47 22114160 Fax: +47 22207097
 *
\**************************************************************************/

#ifndef COIN_SOVRMLSCRIPT_H
#define COIN_SOVRMLSCRIPT_H

#include <Inventor/nodes/SoSubNode.h>
#include <Inventor/fields/SoSFBool.h>
#include <Inventor/fields/SoMFString.h>

class COIN_DLL_API SoVRMLScript : public SoNode
{
  typedef SoNode inherited;

public:
  static void initClass(void);
  SoVRMLScript(void);

  static SoType getClassTypeId(void);
  virtual SoType getTypeId(void) const;

  SoMFString url;
  SoSFBool directOutput;
  SoSFBool mustEvaluate;

  virtual void doAction( SoAction * action );
  virtual void callback( SoCallbackAction * action );
  virtual void GLRender( SoGLRenderAction * action );
  virtual void getBoundingBox( SoGetBoundingBoxAction * action );
  virtual void pick( SoPickAction * action );
  virtual void handleEvent( SoHandleEventAction * action );
  virtual void write( SoWriteAction * action );

protected:
  virtual ~SoVRMLScript();
  virtual void copyContents( const SoFieldContainer * from, SbBool copyConn );

private:
  static SoType classTypeId;
  static void * createInstance(void);
  SoFieldData * fielddata;
  virtual const SoFieldData * getFieldData(void) const;

private:
  virtual SbBool readInstance(SoInput * in, unsigned short flags);
  
  void initFieldData(void);
  class SoVRMLScriptP * pimpl;
}; // class SoVRMLScript

#endif // ! COIN_SOVRMLSCRIPT_H
