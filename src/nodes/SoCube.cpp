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
  \brief The SoCube class is for rendering cubes.
  \ingroup nodes

  Strictly speaking, as you can have different width, height and depth
  values for the "cube", instances of this class renders \e boxes.
*/

#include <Inventor/nodes/SoCube.h>
#include <Inventor/nodes/SoSubNodeP.h>

#include <Inventor/SoPickedPoint.h>
#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/actions/SoGetPrimitiveCountAction.h>
#include <Inventor/actions/SoRayPickAction.h>
#include <Inventor/bundles/SoMaterialBundle.h>
#include <Inventor/details/SoCubeDetail.h>
#include <Inventor/elements/SoDrawStyleElement.h>
#include <Inventor/elements/SoGLNormalizeElement.h>
#include <Inventor/elements/SoGLTextureEnabledElement.h>
#include <Inventor/elements/SoLightModelElement.h>
#include <Inventor/elements/SoMaterialBindingElement.h>
#include <Inventor/elements/SoTextureCoordinateElement.h>
#include <Inventor/misc/SoGL.h>
#include <Inventor/misc/SoGenerate.h>
#include <Inventor/misc/SoState.h>


/*!
  \var SoSFFloat SoCube::width
  X axis dimension of cube, defaults to 2.0.
*/
/*!
  \var SoSFFloat SoCube::height
  Y axis dimension of cube, defaults to 2.0.
*/
/*!
  \var SoSFFloat SoCube::depth
  Z axis dimension of cube, defaults to 2.0.
*/


// *************************************************************************

SO_NODE_SOURCE(SoCube);

/*!
  Constructor.
*/
SoCube::SoCube(void)
{
  SO_NODE_INTERNAL_CONSTRUCTOR(SoCube);

  SO_NODE_ADD_FIELD(width, (2.0f));
  SO_NODE_ADD_FIELD(height, (2.0f));
  SO_NODE_ADD_FIELD(depth, (2.0f));
}

/*!
  Destructor.
*/
SoCube::~SoCube()
{
}

// Doc in parent.
void
SoCube::initClass(void)
{
  SO_NODE_INTERNAL_INIT_CLASS(SoCube);
}

// Doc in parent.
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

  SbBool sendNormals =
    (SoLightModelElement::get(state) !=
     SoLightModelElement::BASE_COLOR);

  SoMaterialBundle mb(action);
  mb.sendFirst();

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

// Doc in parent.
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

// Doc in parent.
SbBool
SoCube::willUpdateNormalizeElement(SoState *) const
{
  return TRUE;
}

// Doc in parent.
void
SoCube::computeBBox(SoAction * action, SbBox3f & box, SbVec3f & center)
{
  center.setValue(0.0f, 0.0f, 0.0f);
  float w, h, d;
  this->getHalfSize(w, h, d);

  // Allow negative values.
  if (w < 0.0f) w = -w;
  if (h < 0.0f) h = -h;
  if (d < 0.0f) d = -d;

  box.setBounds(-w, -h, -d, w, h, d);
}

// Doc in parent.
void
SoCube::rayPick(SoRayPickAction * action)
{
  if (!shouldRayPick(action)) return;

  static int translation[6] = {2, 3, 5, 4, 1, 0}; // translate into detail part-num
  action->setObjectSpace();
  const SbLine & line = action->getLine();
  float size[3];
  this->getHalfSize(size[0], size[1], size[2]);
  int cnt = 0;
  // test intersection with all six planes
  for (int i = 0; i < 3; i++) {
    for (float j = -1.0f; j <= 1.0f; j += 2.0f) {
      SbVec3f norm(0, 0, 0);
      norm[i] = j;
      SbVec3f isect;

      SbPlane plane(norm, size[i]);
      if (plane.intersect(line, isect)) {
        int i1 = (i+1) % 3;
        int i2 = (i+2) % 3;

        if (isect[i1] >= -size[i1] && isect[i1] <= size[i1] &&
            isect[i2] >= -size[i2] && isect[i2] <= size[i2] &&
            action->isBetweenPlanes(isect)) {
          SoPickedPoint * pp = action->addIntersection(isect);
          if (pp) {
            SoCubeDetail * detail = new SoCubeDetail();
            detail->setPart(translation[cnt]);
            pp->setDetail(detail, this);
          }
        }
      }
      cnt++;
    }
  }
}

// Convenience function for finding half the size of the box.
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

// Doc from parent.
void
SoCube::getPrimitiveCount(SoGetPrimitiveCountAction * action)
{
  if (!this->shouldPrimitiveCount(action)) return;

  action->addNumTriangles(12);
}
