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


/*!
  \class SoGenerateNormalsAction Inventor/actions/SoGenerateNormalsAction.h
  \brief The SoGenerateNormalsAction class is used for generating normals.
*/

#include <Inventor/actions/SoGenerateNormalsAction.h>
#include <Inventor/SoDB.h>
#include <Inventor/SoInput.h>
#include <Inventor/nodes/SoNormal.h>
#include <Inventor/nodes/SoIndexedFaceSet.h>
#include <Inventor/nodes/SoCoordinate3.h>
#include <Inventor/nodes/SoCoordinate.h>
#include <Inventor/nodes/SoGroup.h>
#include <Inventor/nodes/SoShapeHints.h>
#include <Inventor/nodes/SoNormalBinding.h>
#include <Inventor/misc/SoState.h>
#include <assert.h>

/*!
  The constructor.
*/

SoGenerateNormalsAction::SoGenerateNormalsAction()
{
  this->creaseAngle = -1.0f;
  this->flipNormals = FALSE;
}

/*!
  The destructor.
*/

SoGenerateNormalsAction::~SoGenerateNormalsAction()
{
}

/*!
  FIXME: write doc.
*/
  
void 
SoGenerateNormalsAction::apply(SoNode * const node)
{
  SoDB::traverse(node, traversegraph, this);

  if (node->format() == SoNode::VRML10) {
    // add a normal binding node for vrml10
    SoNormalBinding *nb = new SoNormalBinding;
    nb->value.value = SoNormalBinding::PER_VERTEX_INDEXED;
    nb->value.setDefault(FALSE);
    ((SoGroup*)node)->insertChild(nb, 0);
  }
}

/*!
  This method lets you override the crease angle found in the scene graph.
  A negative value means that the crease angle found in the scene graph should
  be used.
*/

void 
SoGenerateNormalsAction::setCreaseAngle(const float crease_angle)
{
  this->creaseAngle = crease_angle;
}

/*!
  This method lets you set wether normals should be flipped or not.
*/

void 
SoGenerateNormalsAction::setFlipNormals(const SbBool yesno)
{
  this->flipNormals = yesno;
}

/*!
  FIXME: write doc.
*/

void 
SoGenerateNormalsAction::traversegraph(SoNode *node, SoState *state, void *data)
{
  assert(node != NULL && state != NULL && data != NULL);
  SoGenerateNormalsAction *action = (SoGenerateNormalsAction*)data;
  
  SoType type = node->getTypeId();

  if (node->isOfType(SoNormalBinding::getClassTypeId()) ||
      (node->isOfType(SoNormal::getClassTypeId()) &&
       (node->format() == SoNode::VRML10))) {
    if (node->getNumParents() != 1) {
      // FIXME: safe to remove node from all parents?
      // should be correct, but will create redundant nodes in the scene graph.
      SoDebugError::postInfo("SoGenerateNormalsAction::traversegraph",
			     "Old Normal or NormalBinding nodes with more than one parent "
			     "will not be removed.\n");
    }
    else {
      SoGroup *currentgroup = node->getParent(0); 
      // find node index
      int i;
      int n = currentgroup->getNumChildren();
      for (i = 0; i < n; i++) {
	if (currentgroup->getChild(i) == node) break;
      }
      if (i == n) {
	SoDebugError::postInfo("SoGenerateNormalsAction::traversegraph",
			       "errr...could not remove Normal or NormalBinding node!\n");
      }
      else {
	currentgroup->removeChild(i);
      }
    }
  }
  else if (node->isOfType(SoIndexedFaceSet::getClassTypeId())) {
    SoIndexedFaceSet *fs = (SoIndexedFaceSet*)node;
    SoNormal *norm = action->createNormalNode(fs, state);
    if (norm == NULL) return; // something went wrong
    if (fs->format() == SoNode::VRML10) {
      if (node->getNumParents() != 1) {
	SoDebugError::postInfo("SoGenerateNormalsAction::traversegraph", "Sorry: FaceSets with more than one parent is not supported.\n");
	delete norm; // node is not ref'ed yet
	return;
      }
      SoGroup *currentgroup = node->getParent(0); 
      
      if (currentgroup == NULL) {
	SoDebugError::postInfo("SoGenerateNormalsAction::traversegraph",
			       "errr...no parent node for faceset!\n");
	delete norm; // node is not ref'ed yet
	return;
      }
      int i;
      int n = currentgroup->getNumChildren();
      for (i = 0; i < n; i++) {
	if (currentgroup->getChild(i) == node) break;
      }
      if (i == n) {
	SoDebugError::postInfo("SoGenerateNormalsAction::traversegraph",
			       "FaceSet not found in group node!\n");
	delete norm;
	return;
      }
      currentgroup->insertChild(norm, i);
    }
    else {
      if (fs->normal.value) fs->normal.value->unref();
      // FIXME: addParent for norm-noden????
      norm->ref();
      fs->normal.value = norm;
      fs->normalPerVertex.setValue(TRUE);
      fs->normal.setDefault(FALSE);
      fs->normalPerVertex.setDefault(FALSE);
    }
  }
}

/*!
  FIXME: write doc.
*/

SoNormal *
SoGenerateNormalsAction::createNormalNode(SoIndexedFaceSet * const fs, 
					  SoState * const state)
{
  SbBool ccw = TRUE;
  float creaseangle = 0.5f; // default for VRML 1.0

  SbVec3f *coords = NULL;
  if (fs->format() == SoNode::VRML10) {
    SoCoordinate3 *cnode = 
      (SoCoordinate3*)state->getTopElement(SoState::Coordinate3Index);
    if (cnode == NULL) {
      SoDebugError::postInfo("SoGenerateNormalsAction::createNormalNode",
			     "No coordinate node found for faceset!\n");
      return NULL;
    }
    coords = (SbVec3f*)cnode->point.values;
    
    SoShapeHints *hints = 
      (SoShapeHints*)state->getTopElement(SoState::ShapeHintsIndex);
    if (hints) {
      ccw = hints->vertexOrdering.value != SoShapeHints::CLOCKWISE;
      creaseangle = hints->creaseAngle.value;
    }
  }
  else { // vrml20
    SoCoordinate *cnode = (SoCoordinate*)fs->coord.value;
    if (cnode == NULL) {
      SoDebugError::postInfo("SoGenerateNormalsAction::createNormalNode",
			     "No coordinate node found for faceset!\n");
      return NULL;
    }
    coords = (SbVec3f*)cnode->point.values;
    ccw = fs->ccw.getValue();
    creaseangle = fs->creaseAngle.value;
  }
  
  if (this->flipNormals) ccw = !ccw;
  if (this->creaseAngle >= 0.0f) creaseangle = this->creaseAngle;

  // FIXME: ?? 19981007 mortene.
#ifdef COIN_DEBUG
  fprintf(stderr,"crease angle: %f\n", creaseangle);
#endif
  
  SoNormal *node = new SoNormal;
  fs->makeNormals(coords, node, creaseangle, ccw, TRUE);
  fs->normalIndex.setDefault(FALSE);
  node->vector.setDefault(FALSE);
  return node;
}

