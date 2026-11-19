/**************************************************************************\
 * Copyright (c) Kongsberg Oil & Gas Technologies AS
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 * 
 * Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 * 
 * Redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the distribution.
 * 
 * Neither the name of the copyright holder nor the names of its
 * contributors may be used to endorse or promote products derived from
 * this software without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
\**************************************************************************/

/*!
  \class SoAnnotation SoAnnotation.h Inventor/nodes/SoAnnotation.h
  \brief The SoAnnotation node draws all its child geometry on top of other geometry.

  \ingroup coin_nodes

  This group-type node uses delayed rendering in combination with
  z-buffer disabling to let its children transparently render their
  geometry on top of the other geometry in the scene.

  Since the z-buffer needs to be disabled, the children's geometry
  will not be rendered back-to-front sorted, but rather in the order
  they are present in the scene graph.

  <b>FILE FORMAT/DEFAULTS:</b>
  \code
    Annotation {
        renderCaching AUTO
        boundingBoxCaching AUTO
        renderCulling AUTO
        pickCulling AUTO
    }
    \endcode
*/

#include <Inventor/nodes/SoAnnotation.h>

#include <Inventor/elements/SoCacheElement.h>
#include <Inventor/elements/SoDepthBufferElement.h>
#include <Inventor/elements/SoGLDepthBufferElement.h>
#include <Inventor/misc/SoState.h>
#include <Inventor/system/gl.h>
#include <Inventor/actions/SoGLRenderAction.h>

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif // HAVE_CONFIG_H
#include "nodes/SoSubNodeP.h"


namespace {

// Keep the annotation default in the element, so child depth settings and
// cached rendering use the same state as OpenGL.
class SoAnnotationDepthScope {
public:
  SoAnnotationDepthScope(SoState * state)
    : state(state), depth(state->getDepth()),
      depthtest(glIsEnabled(GL_DEPTH_TEST) ? TRUE : FALSE)
  {
    this->state->push();
  }

  ~SoAnnotationDepthScope()
  {
    // A throwing child can bypass the inherited separator's pop as well.
    // Restore this local state scope; recovering the action's paths and
    // render caches after an exception is a separate responsibility.
    while (this->state->getDepth() > this->depth) this->state->pop();
    this->depthtest ? glEnable(GL_DEPTH_TEST) : glDisable(GL_DEPTH_TEST);
  }

  void disableTest(void)
  {
    SoDepthBufferElement::set(this->state, FALSE,
                             SoDepthBufferElement::getWriteEnable(this->state),
                             SoDepthBufferElement::getFunction(this->state),
                             SoDepthBufferElement::getRange(this->state));
    // The incoming native test may differ from the element, e.g. when
    // an application has changed OpenGL state directly.
    glDisable(GL_DEPTH_TEST);
  }

private:
  SoAnnotationDepthScope(const SoAnnotationDepthScope &);
  SoAnnotationDepthScope & operator=(const SoAnnotationDepthScope &);

  SoState * state;
  int depth;
  SbBool depthtest;
};

} // namespace


SO_NODE_SOURCE(SoAnnotation);


/*!
  Constructor.
*/
SoAnnotation::SoAnnotation()
{
  SO_NODE_INTERNAL_CONSTRUCTOR(SoAnnotation);
}

/*!
  Destructor.
*/
SoAnnotation::~SoAnnotation()
{
}

// Doc in superclass.
/*!
  \copybrief SoBase::initClass(void)
*/
void
SoAnnotation::initClass(void)
{
  SO_NODE_INTERNAL_INIT_CLASS(SoAnnotation, SO_FROM_INVENTOR_1);

  SO_ENABLE(SoGLRenderAction, SoGLDepthBufferElement);
}

// Doc in superclass.
void
SoAnnotation::GLRender(SoGLRenderAction * action)
{
  switch (action->getCurPathCode()) {
  case SoAction::NO_PATH:
  case SoAction::BELOW_PATH:
    this->GLRenderBelowPath(action);
    break;
  case SoAction::OFF_PATH:
    // do nothing. Separator will reset state.
    break;
  case SoAction::IN_PATH:
    this->GLRenderInPath(action);
    break;
  }
}

// Doc in superclass.
void
SoAnnotation::GLRenderBelowPath(SoGLRenderAction * action)
{
  if (action->isRenderingDelayedPaths()) {
    SoAnnotationDepthScope scope(action->getState());
    scope.disableTest();
    inherited::GLRenderBelowPath(action);
  }
  else {
    SoCacheElement::invalidate(action->getState());
    action->addDelayedPath(action->getCurPath()->copy());
  }
}

// Doc in superclass.
void
SoAnnotation::GLRenderInPath(SoGLRenderAction * action)
{
  if (action->isRenderingDelayedPaths()) {
    SoAnnotationDepthScope scope(action->getState());
    scope.disableTest();
    inherited::GLRenderInPath(action);
  }
  else {
    SoCacheElement::invalidate(action->getState());
    action->addDelayedPath(action->getCurPath()->copy());
  }
}

// Doc in superclass.
void
SoAnnotation::GLRenderOffPath(SoGLRenderAction *)
{
  // should never render, this is a separator node
}

#ifdef COIN_TEST_SUITE

BOOST_AUTO_TEST_CASE(initialized)
{
  SoAnnotation * node = new SoAnnotation;
  assert(node);
  node->ref();
  BOOST_CHECK_MESSAGE(node->getTypeId() != SoType::badType(),
                      "missing class initialization");
  node->unref();
}

#endif // COIN_TEST_SUITE
