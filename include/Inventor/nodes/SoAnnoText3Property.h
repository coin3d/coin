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
 *  http://www.sim.no/ sales@sim.no Voice: +47 73540378 Fax: +47 73943861
 *
\**************************************************************************/

#ifndef __SOANNOTEXT3PROPERTY_H__
#define __SOANNOTEXT3PROPERTY_H__

#include <Inventor/nodes/SoNode.h>

#if defined(COIN_EXCLUDE_SOANNOTEXT3PROPERTY)
#error Configuration settings disrespected -- do not include this file!
#endif // COIN_EXCLUDE_SOANNOTEXT3PROPERTY

#include <Inventor/fields/SoSFEnum.h>
#include <Inventor/fields/SoSFBool.h>

// *************************************************************************

class SoAnnoText3Property : public SoNode {
    typedef SoNode inherited;

//$ BEGIN TEMPLATE NodeHeader( SoAnnoText3Property )
private:
  static SoType classTypeId;

public:
  static SoType getClassTypeId(void);
  virtual SoType getTypeId(void) const;
  static void * createInstance(void);
public:
  static void initClass(void);
  static void cleanClass(void);

  SoAnnoText3Property(void);
protected:
  virtual ~SoAnnoText3Property();
//$ END TEMPLATE NodeHeader

public:
  enum RenderPrintType {
    RENDER3D_PRINT_VECTOR, RENDER3D_PRINT_RASTER, RENDER2D_PRINT_RASTER
  };

  enum FontSizeHint {
    ANNOTATION, FIT_VECTOR_TEXT
  };

  SoSFEnum renderPrintType;
  SoSFBool isCharOrientedRasterPrint;
  SoSFEnum fontSizeHint;

#if !defined(COIN_EXCLUDE_SOACTION)
  virtual void doAction(SoAction * action);
#endif // !COIN_EXCLUDE_SOACTION
#if !defined(COIN_EXCLUDE_SOGLRENDERACTION)
  virtual void GLRender(SoGLRenderAction * action);
#endif // !COIN_EXCLUDE_SOGLRENDERACTION
#if !defined(COIN_EXCLUDE_SOCALLBACKACTION)
  virtual void callback(SoCallbackAction * action);
#endif // !COIN_EXCLUDE_SOCALLBACKACTION
#if !defined(COIN_EXCLUDE_SOPICKACTION)
  virtual void pick(SoPickAction * action);
#endif // !COIN_EXCLUDE_SOPICKACTION
#if !defined(COIN_EXCLUDE_SOGETBOUNDINGBOXACTION)
  virtual void getBoundingBox(SoGetBoundingBoxAction * action);
#endif // !COIN_EXCLUDE_SOGETBOUNDINGBOXACTION
#if !defined(COIN_EXCLUDE_SOGETPRIMITIVECOUNTACTION)
  virtual void getPrimitiveCount(SoGetPrimitiveCountAction * action);
#endif // !COIN_EXCLUDE_SOGETPRIMITIVECOUNTACTION
};

#endif // !__SOANNOTEXT3PROPERTY_H__
