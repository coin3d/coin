/**************************************************************************\
 *
 *  Copyright (C) 1998-2000 by Systems in Motion.  All rights reserved.
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

/*!
  \class SoCube SoCube.h Inventor/nodes/SoCube.h
  \brief The SoCube class ...
  \ingroup nodes

  FIXME: write class doc
*/

#include <Inventor/nodes/SoCube.h>


#include <Inventor/misc/SoState.h>
#include <Inventor/bundles/SoMaterialBundle.h>
#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/misc/SoGL.h>
#include <Inventor/actions/SoRayPickAction.h>
#include <Inventor/SoPickedPoint.h>
#include <Inventor/details/SoCubeDetail.h>
#include <Inventor/elements/SoMaterialBindingElement.h>
#include <Inventor/elements/SoLightModelElement.h>
#include <Inventor/elements/SoTextureCoordinateElement.h>
#include <Inventor/elements/SoGLTextureEnabledElement.h>
#include <Inventor/elements/SoGLShapeHintsElement.h>
#include <Inventor/elements/SoGLShadeModelElement.h>
#include <Inventor/elements/SoGLNormalizeElement.h>
#include <Inventor/actions/SoGetPrimitiveCountAction.h>
#include <Inventor/elements/SoDrawStyleElement.h>
#include <Inventor/misc/SoGenerate.h>

#include <assert.h>


/*!
  \var SoSFFloat SoCube::width
  FIXME: write documentation for field
*/
/*!
  \var SoSFFloat SoCube::height
  FIXME: write documentation for field
*/
/*!
  \var SoSFFloat SoCube::depth
  FIXME: write documentation for field
*/


// *************************************************************************

SO_NODE_SOURCE(SoCube);

/*!
  Constructor.
*/
SoCube::SoCube()
{
  SO_NODE_INTERNAL_CONSTRUCTOR(SoCube);

  SO_NODE_ADD_FIELD(width,(2.0f));
  SO_NODE_ADD_FIELD(height,(2.0f));
  SO_NODE_ADD_FIELD(depth,(2.0f));
}

/*!
  Destructor.
*/
SoCube::~SoCube()
{
}

/*!
  Does initialization common for all objects of the
  SoCube class. This includes setting up the
  type system, among other things.
*/
void
SoCube::initClass(void)
{
  SO_NODE_INTERNAL_INIT_CLASS(SoCube);
}

/*!
  FIXME: write function documentation
*/
void
SoCube::GLRender(SoGLRenderAction * action)
{
  if (!this->shouldGLRender(action)) return;
  SoState * state = action->getState();

  SoMaterialBindingElement::Binding binding =
    SoMaterialBindingElement::get(state);

  SbBool materialPerPart =
    (binding == SoMaterialBindingElement::PER_PART ||
     binding == SoMaterialBindingElement::PER_PART_INDEXED);

  SbBool doTextures = SoGLTextureEnabledElement::get(state);

  SbBool useTexFunc =
    (SoTextureCoordinateElement::getType(state) ==
     SoTextureCoordinateElement::FUNCTION);

  const SoTextureCoordinateElement * tce;
  if (useTexFunc)
    tce = SoTextureCoordinateElement::getInstance(state);

  SbBool sendNormals =
    (SoLightModelElement::get(state) !=
     SoLightModelElement::BASE_COLOR);

  SoMaterialBundle mb(action);
  mb.sendFirst();

  const SoGLShapeHintsElement * sh = (SoGLShapeHintsElement *)
    action->getState()->getConstElement(SoGLShapeHintsElement::getClassStackIndex());
  sh->forceSend(TRUE,
                SoDrawStyleElement::get(state) == SoDrawStyleElement::FILLED ? TRUE : FALSE);

  const SoGLShadeModelElement * sm = (SoGLShadeModelElement *)
    state->getConstElement(SoGLShadeModelElement::getClassStackIndex());
  sm->forceSend(TRUE);

  if (sendNormals) {
    const SoGLNormalizeElement * ne = (SoGLNormalizeElement *)
      state->getConstElement(SoGLNormalizeElement::getClassStackIndex());
    ne->forceSend(TRUE);
  }

  unsigned int flags = 0;
  if (materialPerPart) flags |= SOGL_MATERIAL_PER_PART;
  if (doTextures) flags |= SOGL_NEED_TEXCOORDS;
  if (sendNormals) flags |= SOGL_NEED_NORMALS;

  sogl_render_cube(width.getValue(),
                   height.getValue(),
                   depth.getValue(),
                   &mb,
                   flags);
}

/*!
  FIXME: write function documentation
*/
void
SoCube::generatePrimitives(SoAction * action)
{
  SoMaterialBindingElement::Binding binding =
    SoMaterialBindingElement::get(action->getState());

  SbBool materialPerPart =
    (binding == SoMaterialBindingElement::PER_PART ||
     binding == SoMaterialBindingElement::PER_PART_INDEXED);

  unsigned int flags = 0;
  if (materialPerPart) flags |= SOGEN_MATERIAL_PER_PART;
  sogen_generate_cube(this->width.getValue(),
                      this->height.getValue(),
                      this->depth.getValue(),
                      flags,
                      this,
                      action);
}

/*!
  FIXME: write function documentation
*/
SbBool
SoCube::willSetShadeModel(void) const
{
  return TRUE;
}

/*!
  FIXME: write function documentation
*/
SbBool
SoCube::willSetShapeHints(void) const
{
  return TRUE;
}

//!
SbBool
SoCube::willUpdateNormalizeElement(SoState *) const
{
  return TRUE;
}

/*!
  FIXME: write function documentation
*/
void
SoCube::computeBBox(SoAction * /* action */, SbBox3f & box, SbVec3f & center)
{
  center.setValue(0.0f, 0.0f, 0.0f);
  float w,h,d;
  this->getHalfSize(w,h,d);
  box.setBounds(-w, -h, -d, w, h, d);
}

/*!
  FIXME: write function documentation
*/
void
SoCube::rayPick(SoRayPickAction *action)
{
  if (!shouldRayPick(action)) return;

  static int translation[6] = {2,3,5,4,1,0}; // translate into detail part-num
  action->setObjectSpace();
  const SbLine &line = action->getLine();
  float size[3];
  this->getHalfSize(size[0],size[1],size[2]);
#if 0
  fprintf(stderr,"obj line: %g %g %g, %g %g %g\n",
          line.getPosition()[0], line.getPosition()[1], line.getPosition()[2],
          line.getDirection()[0], line.getDirection()[1], line.getDirection()[2]);
#endif
  int cnt = 0;
  // test intersection with all six planes
  for (int i = 0; i < 3; i++) {
    for (float j = -1.0f; j <= 1.0f; j += 2.0f) {
      SbVec3f norm(0,0,0);
      norm[i] = j;
      SbVec3f isect;

      SbPlane plane(norm, size[i]);
      if (plane.intersect(line, isect)) {
        int i1 = (i+1) % 3;
        int i2 = (i+2) % 3;

        if (isect[i1] >= -size[i1] && isect[i1] <= size[i1] &&
            isect[i2] >= -size[i2] && isect[i2] <= size[i2] &&
            action->isBetweenPlanes(isect)) {
          SoPickedPoint *pp = action->addIntersection(isect);
          if (pp) {
            SoCubeDetail *detail = new SoCubeDetail();
            detail->setPart(translation[cnt]);
            pp->setDetail(detail, this);
          }
        }
      }
      cnt++;
    }
  }
}

/*!
  \internal
*/
void
SoCube::getHalfSize(float & w, float & h, float & d)
{
  w = (width.isIgnored() ? 1.0f :
       width.getValue() / 2.0f);
  h = (height.isIgnored() ? 1.0f :
       height.getValue() / 2.0f);
  d = (depth.isIgnored() ? 1.0f :
       depth.getValue() / 2.0f);
}

/*!
  FIXME: write doc
*/
void
SoCube::getPrimitiveCount(SoGetPrimitiveCountAction *action)
{
  if (!this->shouldPrimitiveCount(action)) return;

  action->addNumTriangles(12);
}
