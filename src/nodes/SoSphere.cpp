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
  \class SoSphere SoSphere.h Inventor/nodes/SoSphere.h
  \brief The SoSphere class is a sphere shape node.
  \ingroup nodes

  FIXME: write class doc
*/

#include <Inventor/nodes/SoSphere.h>
#include <Inventor/nodes/SoSubNodeP.h>
#include <Inventor/SbSphere.h>

#include <Inventor/bundles/SoMaterialBundle.h>
#include <Inventor/misc/SoState.h>
#include <Inventor/elements/SoGLTextureEnabledElement.h>

#include <Inventor/elements/SoGLShadeModelElement.h>
#include <Inventor/elements/SoGLNormalizeElement.h>
#include <Inventor/elements/SoLightModelElement.h>

#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/misc/SoGL.h>

#include <Inventor/actions/SoRayPickAction.h>



#include <Inventor/actions/SoGetPrimitiveCountAction.h>
#include <Inventor/misc/SoGenerate.h>

/*!
  \var SoSFFloat SoSphere::radius
  FIXME: write documentation for field
*/

#define SPHERE_NUM_SLICES 30.0f
#define SPHERE_NUM_STACKS 30.0f

// *************************************************************************

SO_NODE_SOURCE(SoSphere);

/*!
  Constructor.
*/
SoSphere::SoSphere()
{
  SO_NODE_INTERNAL_CONSTRUCTOR(SoSphere);

  SO_NODE_ADD_FIELD(radius, (1.0f));
}

/*!
  Destructor.
*/
SoSphere::~SoSphere()
{
}

// doc from parent
void
SoSphere::initClass(void)
{
  SO_NODE_INTERNAL_INIT_CLASS(SoSphere);
}

// doc from parent
void
SoSphere::GLRender(SoGLRenderAction * action)
{
  if (!shouldGLRender(action)) return;

  SoState *state = action->getState();

  SoMaterialBundle mb(action);
  mb.sendFirst();

  SbBool doTextures = SoGLTextureEnabledElement::get(state);

  SbBool sendNormals =
    (SoLightModelElement::get(state) !=
     SoLightModelElement::BASE_COLOR);

  float complexity = this->getComplexityValue(action);

  const SoGLShadeModelElement * sm = (SoGLShadeModelElement *)
    state->getConstElement(SoGLShadeModelElement::getClassStackIndex());
  sm->forceSend(FALSE);

  if (sendNormals) {
    const SoGLNormalizeElement * ne = (SoGLNormalizeElement *)
      state->getConstElement(SoGLNormalizeElement::getClassStackIndex());
    ne->forceSend(TRUE);
  }

  unsigned int flags = 0;
  if (sendNormals) flags |= SOGL_NEED_NORMALS;
  if (doTextures) flags |= SOGL_NEED_TEXCOORDS;

  sogl_render_sphere(this->radius.getValue(),
                     (int)(SPHERE_NUM_SLICES * complexity),
                     (int)(SPHERE_NUM_STACKS * complexity),
                     &mb,
                     flags);
}

// doc from parent
SbBool
SoSphere::willSetShadeModel(void) const
{
  return TRUE;
}

// doc from parent
SbBool
SoSphere::willUpdateNormalizeElement(SoState *) const
{
  return TRUE;
}

// doc from parent
void
SoSphere::computeBBox(SoAction * /* action */, SbBox3f & box, SbVec3f & center)
{
  float r = this->radius.getValue();

  // Allow negative values.
  if (r < 0.0f) r = -r;

  box.setBounds(SbVec3f(-r, -r, -r), SbVec3f(r, r, r));
  center.setValue(0.0f, 0.0f, 0.0f);
}

// doc from parent
void
SoSphere::rayPick(SoRayPickAction *action)
{
  if (!shouldRayPick(action)) return;

  action->setObjectSpace();
  const SbLine &line = action->getLine();
  SbSphere sphere(SbVec3f(0.0f, 0.0f, 0.0f), radius.getValue());
  SbVec3f enter, exit;
  if (sphere.intersect(line, enter, exit)) {
    if (action->isBetweenPlanes(enter)) action->addIntersection(enter);
    if (action->isBetweenPlanes(exit)) action->addIntersection(exit);
  }
}

// doc from parent
void
SoSphere::getPrimitiveCount(SoGetPrimitiveCountAction *action)
{
  if (!this->shouldPrimitiveCount(action)) return;

  float complexity = this->getComplexityValue(action);
  action->addNumTriangles((int)(complexity*2.0f*SPHERE_NUM_SLICES*(SPHERE_NUM_STACKS-1)));
}

// doc from parent
void
SoSphere::generatePrimitives(SoAction *action)
{
  float complexity = this->getComplexityValue(action);

  sogen_generate_sphere(this->radius.getValue(),
                        (int)(SPHERE_NUM_SLICES * complexity),
                        (int)(SPHERE_NUM_STACKS * complexity),
                        this,
                        action);

}
