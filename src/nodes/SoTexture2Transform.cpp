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

/*!
  \class SoTexture2Transform SoTexture2Transform.h Inventor/nodes/SoTexture2Transform.h
  \brief The SoTexture2Transform class ...
  \ingroup nodes

  FIXME: write class doc
*/

#include <Inventor/nodes/SoTexture2Transform.h>


#include <math.h>

#if !defined(COIN_EXCLUDE_SOGLRENDERACTION)
#include <Inventor/actions/SoGLRenderAction.h>
#endif // !COIN_EXCLUDE_SOGLRENDERACTION

#if !defined(COIN_EXCLUDE_SOGLTEXTUREMATRIXELEMENT)
#include <Inventor/elements/SoGLTextureMatrixElement.h>
#endif // !COIN_EXCLUDE_SOGLTEXTUREMATRIXELEMENT

/*!
  \var SoSFVec2f SoTexture2Transform::translation
  FIXME: write documentation for field
*/
/*!
  \var SoSFFloat SoTexture2Transform::rotation
  FIXME: write documentation for field
*/
/*!
  \var SoSFVec2f SoTexture2Transform::scaleFactor
  FIXME: write documentation for field
*/
/*!
  \var SoSFVec2f SoTexture2Transform::center
  FIXME: write documentation for field
*/


// *************************************************************************

SO_NODE_SOURCE(SoTexture2Transform);

/*!
  Constructor.
*/
SoTexture2Transform::SoTexture2Transform()
{
  SO_NODE_INTERNAL_CONSTRUCTOR(SoTexture2Transform);
  
  SO_NODE_ADD_FIELD(translation, (0.0f, 0.0f));
  SO_NODE_ADD_FIELD(rotation, (0.0f));
  SO_NODE_ADD_FIELD(scaleFactor, (1.0f, 1.0f));
  SO_NODE_ADD_FIELD(center, (0.0f, 0.0f));
}

/*!
  Destructor.
*/
SoTexture2Transform::~SoTexture2Transform()
{
}

/*!
  Does initialization common for all objects of the
  SoTexture2Transform class. This includes setting up the
  type system, among other things.
*/
void
SoTexture2Transform::initClass(void)
{
  SO_NODE_INTERNAL_INIT_CLASS(SoTexture2Transform);

#if !defined(COIN_EXCLUDE_SOGLRENDERACTION)
  SO_ENABLE(SoGLRenderAction, SoGLTextureMatrixElement);
#endif // !COIN_EXCLUDE_SOGLRENDERACTION
}


#if !defined(COIN_EXCLUDE_SOGLRENDERACTION)
/*!
  FIXME: write function documentation
*/
void 
SoTexture2Transform::GLRender(SoGLRenderAction * action)
{
  SbMatrix mat, tmp;
  SbVec2f c = center.isIgnored() ? 
    SbVec2f(0.0f, 0.0f) :
    center.getValue();

  mat.makeIdentity();
  mat[3][0] = -c[0];
  mat[3][1] = -c[1];

  SbVec2f scale = scaleFactor.getValue();
  if (!scaleFactor.isIgnored() && 
      scale != SbVec2f(1.0f, 1.0f)) {
    tmp.makeIdentity();
    tmp[0][0] = scale[0];
    tmp[1][1] = scale[1];
    mat.multLeft(tmp);
  }
  if (!rotation.isIgnored() && rotation.getValue() != 0.0f) {
    float cosa = (float)cos(rotation.getValue());
    float sina = (float)sin(rotation.getValue());
    tmp.makeIdentity();
    tmp[0][0] = cosa;
    tmp[1][0] = -sina;
    tmp[0][1] = sina;
    tmp[1][1] = cosa;
    mat.multLeft(tmp);
  }
  c += translation.getValue();
  if (!translation.isIgnored() && 
      c != SbVec2f(0.0f, 0.0f)) {
    tmp.makeIdentity();
    tmp[3][0] = c[0];
    tmp[3][1] = c[1];
    mat.multLeft(tmp);
  }
  
  SoTextureMatrixElement::mult(action->getState(), this,
			       mat);
}
#endif // ! COIN_EXCLUDE_SOGLRENDERACTION



#if !defined(COIN_EXCLUDE_SOACTION)
/*!
  FIXME: write doc
 */
void
SoTexture2Transform::doAction(SoAction * /* action */)
{
  assert(0 && "FIXME: not implemented");
}
#endif // !COIN_EXCLUDE_SOACTION

#if !defined(COIN_EXCLUDE_SOCALLBACKACTION)
/*!
  FIXME: write doc
 */
void
SoTexture2Transform::callback(SoCallbackAction * /* action */)
{
  assert(0 && "FIXME: not implemented");
}
#endif // !COIN_EXCLUDE_SOCALLBACKACTION

#if !defined(COIN_EXCLUDE_SOGETMATRIXACTION)
/*!
  FIXME: write doc
 */
void
SoTexture2Transform::getMatrix(SoGetMatrixAction * /* action */)
{
  assert(0 && "FIXME: not implemented");
}
#endif // !COIN_EXCLUDE_SOGETMATRIXACTION

#if !defined(COIN_EXCLUDE_SOPICKACTION)
/*!
  FIXME: write doc
 */
void
SoTexture2Transform::pick(SoPickAction * /* action */)
{
  assert(0 && "FIXME: not implemented");
}
#endif // !COIN_EXCLUDE_SOPICKACTION

void 
SoTexture2Transform::calcMatrix(SbMatrix &mat)
{
  SbMatrix tmp;
  SbVec2f c = center.isIgnored() ? 
    SbVec2f(0.0f, 0.0f) :
    center.getValue();

  mat.makeIdentity();
  mat[3][0] = -c[0];
  mat[3][1] = -c[1];
  
  SbVec2f scale = scaleFactor.getValue();
  if (!scaleFactor.isIgnored() && 
      scale != SbVec2f(1.0f, 1.0f)) {
    tmp.makeIdentity();
    tmp[0][0] = scale[0];
    tmp[1][1] = scale[1];
    mat.multRight(tmp);
  }
  if (!rotation.isIgnored() && rotation.getValue() != 0.0f) {
    float cosa = (float)cos(rotation.getValue());
    float sina = (float)sin(rotation.getValue());
    tmp.makeIdentity();
    tmp[0][0] = cosa;
    tmp[1][0] = -sina;
    tmp[0][1] = sina;
    tmp[1][1] = cosa;
    mat.multRight(tmp);
  }
  c += translation.getValue();
  if (!translation.isIgnored() && 
      c != SbVec2f(0.0f, 0.0f)) {
    tmp.makeIdentity();
    tmp[3][0] = c[0];
    tmp[3][1] = c[1];
    mat.multRight(tmp);
  }
}

