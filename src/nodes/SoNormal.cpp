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
  \class SoNormal SoNormal.h Inventor/nodes/SoNormal.h
  \brief The SoNormal class ...
  \ingroup nodes

  FIXME: write class doc
*/

#include <Inventor/nodes/SoNormal.h>



#if !defined(COIN_EXCLUDE_SOGLRENDERACTION)
#include <Inventor/actions/SoGLRenderAction.h>
#endif // !COIN_EXCLUDE_SOGLRENDERACTION
#if !defined(COIN_EXCLUDE_SOPICKACTION)
#include <Inventor/actions/SoPickAction.h>
#endif // !COIN_EXCLUDE_SOPICKACTION

#if !defined(COIN_EXCLUDE_SONORMALELEMENT)
#include <Inventor/elements/SoNormalElement.h>
#endif // !COIN_EXCLUDE_SONORMALELEMENT

#include <Inventor/actions/SoCallbackAction.h>
#include <Inventor/actions/SoGetPrimitiveCountAction.h>

/*!
  \var SoMFVec3f SoNormal::vector
  FIXME: write documentation for field
*/

// *************************************************************************

SO_NODE_SOURCE(SoNormal);

/*!
  Constructor.
*/
SoNormal::SoNormal()
{
  SO_NODE_INTERNAL_CONSTRUCTOR(SoNormal);

  SO_NODE_ADD_FIELD(vector, (NULL));
}

/*!
  Destructor.
*/
SoNormal::~SoNormal()
{
}

/*!
  Does initialization common for all objects of the
  SoNormal class. This includes setting up the
  type system, among other things.
*/
void
SoNormal::initClass(void)
{
  SO_NODE_INTERNAL_INIT_CLASS(SoNormal);

#if !defined(COIN_EXCLUDE_SOGLRENDERACTION)
  SO_ENABLE(SoGLRenderAction, SoNormalElement);
#endif // !COIN_EXCLUDE_SOGLRENDERACTION

#if !defined(COIN_EXCLUDE_SOPICKACTION)
  SO_ENABLE(SoPickAction, SoNormalElement);
#endif // !COIN_EXCLUDE_SOPICKACTION

  SO_ENABLE(SoCallbackAction, SoNormalElement);
  SO_ENABLE(SoGetPrimitiveCountAction, SoNormalElement);
}

// FIXME: enable this for 10%++ performance gain (no need to enable
//        GL_NORMALIZE).
// SbVec3f*
// SoNormal::getNormalizedVectors()
// {
//   if (mVectorType == UNITVECTORS) return (SbVec3f*)vector.getValues(0); // FIXME: temp cast hack to compile. 19980909 mortene.
//   if(mNormalizedVectors == NULL) {
//     int i,n;
//     const SbVec3f *vec;
//     n = vector.getNum();
//     mNormalizedVectors = new SbVec3f[n];
//     vec = vector.getValues(0);
//     for (i = 0; i < n; i++) {
//       mNormalizedVectors[i] = *vec++;
//       mNormalizedVectors[i].normalize();
//     }
//   }
//   return mNormalizedVectors;
// }


#if !defined(COIN_EXCLUDE_SOGLRENDERACTION)
/*!
  FIXME: write function documentation
*/
void
SoNormal::GLRender(SoGLRenderAction * action)
{
  //
  // TODO: code to test if all normals are unit length,
  //       and store this in some cached variable.
  //       should be passed on to SoGLNormalizeElement
  //       to optimize rendering (pederb)
  //
  SoNormal::doAction(action);

}
#endif // !COIN_EXCLUDE_SOGLRENDERACTION


#if !defined(COIN_EXCLUDE_SOACTION)
/*!
  FIXME: write doc
 */
void
SoNormal::doAction(SoAction *action)
{
  SoNormalElement::set(action->getState(), this,
                       vector.getNum(), vector.getValues(0));
}

#endif // !COIN_EXCLUDE_SOACTION

#if !defined(COIN_EXCLUDE_SOCALLBACKACTION)
/*!
  FIXME: write doc
 */
void
SoNormal::callback(SoCallbackAction *action)
{
  SoNormal::doAction(action);
}
#endif // !COIN_EXCLUDE_SOCALLBACKACTION

#if !defined(COIN_EXCLUDE_SOPICKACTION)
/*!
  FIXME: write doc
 */
void
SoNormal::pick(SoPickAction *action)
{
  SoNormal::doAction(action);
}
#endif // !COIN_EXCLUDE_SOPICKACTION

#if !defined(COIN_EXCLUDE_SOGETPRIMITIVECOUNTACTION)
/*!
  FIXME: write doc
 */
void
SoNormal::getPrimitiveCount(SoGetPrimitiveCountAction * /* action */)
{
  COIN_STUB();
}
#endif // !COIN_EXCLUDE_SOGETPRIMITIVECOUNTACTION
