/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2001 by Systems in Motion.  All rights reserved.
 *  
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  version 2 as published by the Free Software Foundation.  See the
 *  file LICENSE.GPL at the root directory of this source distribution
 *  for more details.
 *
 *  If you desire to use Coin with software that is incompatible
 *  licensewise with the GPL, and / or you would like to take
 *  advantage of the additional benefits with regard to our support
 *  services, please contact Systems in Motion about acquiring a Coin
 *  Professional Edition License.  See <URL:http://www.coin3d.org> for
 *  more information.
 *
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY
 *  <URL:http://www.sim.no>, <mailto:support@sim.no>
 *
\**************************************************************************/

/*!
  \class SoToVRML2Action SoToVRML2Action.h Inventor/actions/SoToVRML2Action.h
  \brief The SoToVRML2Action class builds a new scene graph, using only VRML97/VRML2 nodes.
  \ingroup actions

  This action is used for converting a scene graph of VRML1/Coin nodes
  to a new scene graph using only VRML97/VRML2 nodes.

  Due to the basic differences between VRML1/Coin and VRML2 (the
  latter does not really have a traversal state) the new scene graph
  will typically be somewhat larger. To minimize this effect the
  action tries to re-use nodes when possible.

  VRML1 nodes will be converted to its direct equivalent VRML2 node,
  while Coin nodes with no VRML2 equivalent are converted to
  IndexedFaceSet. If the DrawStyle is POINTS, all geometry will be
  built using PointSet; if it is LINES IndexedLineSet is used.

  Note: if VRML97 support is not present in the Coin library, this
  action does nothing and getVRML2SceneGraph always returns NULL.

  \sa SoToVRMLAction

  \since Coin 2.0
  \since TGS Inventor 2.5
*/

// FIXME: SoComplexity::BOUNDING_BOX are not supported. For
// DrawStyle::LINES quads are not handled correctly (will always draw
// triangles). SoArray and SoMultipleCopy are not supported.
// Reusing of appearance and geometry nodes is not implemented.
// 20020813 kristian.

#include <Inventor/SbName.h>
#include <Inventor/actions/SoToVRML2Action.h>
#include <Inventor/actions/SoCallbackAction.h>
#include <Inventor/actions/SoSearchAction.h>

#include <Inventor/nodes/SoNodes.h>
#include <Inventor/elements/SoCoordinateElement.h>
#include <Inventor/elements/SoNormalElement.h>
#include <Inventor/elements/SoTextureCoordinateElement.h>

#include <Inventor/errors/SoDebugError.h>

#include <Inventor/SoInput.h>
#include <Inventor/SoOutput.h>
#include <Inventor/SbBSPTree.h>
#include <Inventor/actions/SoGetPrimitiveCountAction.h>
#include <Inventor/SbViewportRegion.h>
#include <Inventor/nodes/SoTransform.h>
#include <Inventor/actions/SoWriteAction.h>
#include <Inventor/lists/SbList.h>
#include <Inventor/SoPrimitiveVertex.h>
#include <Inventor/lists/SoPathList.h>
#include <Inventor/lists/SoNodeList.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif // HAVE_CONFIG_H

SO_ACTION_SOURCE(SoToVRML2Action);

// *************************************************************************

// Overridden from parent class.
void
SoToVRML2Action::initClass(void)
{
  SO_ACTION_INIT_CLASS(SoToVRML2Action, SoToVRMLAction);
}

/*!
  \fn SoToVRML2Action::SoToVRML2Action(void)
  Constructor.
*/

/*!
  \fn SoToVRML2Action::~SoToVRML2Action(void)
  The destructor.
*/

/*!
  \fn SoNode * SoToVRML2Action::getVRML2SceneGraph(void) const

  Return a pointer to the root node of the generated scenegraph of
  only VRML2 / VRML97 nodes.

  Will return \c NULL if VRML97 support was not compiled into the
  library.
*/

/*!
  \fn void SoToVRML2Action::reuseAppearanceNodes(SbBool appearance)

  Set the flag deciding if appearance nodes should be reused if possible.
  The default is FALSE.
*/

/*!
  \fn SbBool SoToVRML2Action::doReuseAppearanceNodes(void) const

  Get the flag deciding if appearance nodes should be reused if possible.
  The default is FALSE.
*/

/*!
  \fn void SoToVRML2Action::reusePropertyNodes(SbBool property)

  Set the flag deciding if property nodes should be reused if possible.
  The default is FALSE.
*/

/*!
  \fn SbBool SoToVRML2Action::doReusePropertyNodes(void) const

  Get the flag deciding if property nodes should be reused if possible.
  The default is FALSE.
*/

/*!
  \fn void SoToVRML2Action::reuseGeometryNodes(SbBool geometry)

  Set the flag deciding if geometry nodes should be reused if possible.
  The default is FALSE.
*/

/*!
  \fn SbBool SoToVRML2Action::doReuseGeometryNodes(void) const

  Get the flag deciding if geometry nodes should be reused if possible.
  The default is FALSE.
*/


// *************************************************************************

#ifndef HAVE_VRML97
SoToVRML2Action::SoToVRML2Action(void)
{
  SO_ACTION_CONSTRUCTOR(SoToVRML2Action);
}

SoToVRML2Action::~SoToVRML2Action() { }
void SoToVRML2Action::apply(SoNode * node) { }
void SoToVRML2Action::apply(SoPath * path) { }
void SoToVRML2Action::apply(const SoPathList & pathlist, SbBool obeysrules) { }
SoNode * SoToVRML2Action::getVRML2SceneGraph(void) const { return NULL; }
void SoToVRML2Action::beginTraversal(SoNode * node) { }
void SoToVRML2Action::reuseAppearanceNodes(SbBool appearance) { }
SbBool SoToVRML2Action::doReuseAppearanceNodes(void) const { return FALSE; }
void SoToVRML2Action::reusePropertyNodes(SbBool property) { }
SbBool SoToVRML2Action::doReusePropertyNodes(void) const { return FALSE; }
void SoToVRML2Action::reuseGeometryNodes(SbBool geometry) { }
SbBool SoToVRML2Action::doReuseGeometryNodes(void) const { return FALSE; }
#else // HAVE_VRML97

#include <Inventor/VRMLnodes/SoVRMLNodes.h>
#include <Inventor/VRMLnodes/SoVRML.h>

#ifndef DOXYGEN_SKIP_THIS

class SoToVRML2ActionP {
public:
  SoToVRML2ActionP(SoToVRML2Action * master)
  {
    this->master = master;
    this->nodefuse = FALSE; // for optimizing bad scene graphs
    this->reuseAppearanceNodes = FALSE;
    this->reusePropertyNodes = FALSE;
    this->reuseGeometryNodes = FALSE;
    this->vrml2path = NULL;
    this->vrml2root = NULL;
  }

  void init(void)
  {
    this->bsptree = NULL;
    this->bsptreetex = NULL;
    this->bsptreenormal = NULL;
    this->coordidx = NULL;
    this->normalidx = NULL;
    this->texidx = NULL;
    this->coloridx = NULL;

    recentTex2 = NULL;
    do_post_primitives = FALSE;
      
    this->vrmlcoords = new SbList <SoVRMLCoordinate *>;
    this->vrmlnormals = new SbList <SoVRMLNormal *>;
    this->vrmlcolors = new SbList <SoVRMLColor *>;
    this->vrmltexcoords = new SbList <SoVRMLTextureCoordinate *>;

    if (this->vrml2path) {
      this->vrml2path->unref();
    }
    this->vrml2path = (SoFullPath*) new SoPath;
    this->vrml2path->ref();
    
    if (this->vrml2root) {
      this->vrml2root->unref();
    }
    this->vrml2root = new SoVRMLGroup;
    this->vrml2root->ref();
    this->vrml2path->setHead(this->vrml2root);
  }

  SoToVRML2Action * master;
  SbBool nodefuse;
  SbBool reuseAppearanceNodes;
  SbBool reusePropertyNodes;
  SbBool reuseGeometryNodes;
  SbDict dict;
  SoCallbackAction cbaction;
  SoSearchAction searchaction;

  SbBSPTree * bsptree;
  SbBSPTree * bsptreetex;
  SbBSPTree * bsptreenormal;
  SbList <int32_t> * coordidx;
  SbList <int32_t> * normalidx;
  SbList <int32_t> * texidx;
  SbList <int32_t> * coloridx;

  SoTexture2 * recentTex2;
  SbBool do_post_primitives;

  static SoCallbackAction::Response unsupported_cb(void *, SoCallbackAction *, const SoNode *);
  
  SoFullPath * vrml2path;
  SoVRMLGroup * vrml2root;
  SbList <SoVRMLCoordinate *> * vrmlcoords;
  SbList <SoVRMLNormal *> * vrmlnormals;
  SbList <SoVRMLColor *> * vrmlcolors;
  SbList <SoVRMLTextureCoordinate *> * vrmltexcoords;

  SoNode * search_for_recent_node(SoAction * action, const SoType & type);
  SoGroup * get_current_tail(void);
  SoVRMLCoordinate * get_or_create_coordinate(const SbVec4f *, int32_t num);
  SoVRMLCoordinate * get_or_create_coordinate(const SbVec3f *, int32_t num);
  SoVRMLNormal * get_or_create_normal(const SbVec3f *, int32_t num);
  SoVRMLColor * get_or_create_color(const uint32_t * packedColor, int32_t num);
  SoVRMLColor * get_or_create_color(const SbColor *, int32_t num);
  SoVRMLTextureCoordinate * get_or_create_texcoordinate(const SbVec2f *, int32_t num);
  void insert_shape(SoCallbackAction * action, SoVRMLGeometry * geom);    

  // Shape nodes
  static SoCallbackAction::Response soasciitext_cb(void *, SoCallbackAction *, const SoNode *);
  static SoCallbackAction::Response socone_cb(void *, SoCallbackAction *, const SoNode *);
  static SoCallbackAction::Response socube_cb(void *, SoCallbackAction *, const SoNode *);
  static SoCallbackAction::Response socylinder_cb(void *, SoCallbackAction *, const SoNode *);
  static SoCallbackAction::Response soifs_cb(void *, SoCallbackAction *, const SoNode *);
  static SoCallbackAction::Response soils_cb(void *, SoCallbackAction *, const SoNode *);
  static SoCallbackAction::Response solineset_cb(void *, SoCallbackAction *, const SoNode *);
  static SoCallbackAction::Response sopointset_cb(void *, SoCallbackAction *, const SoNode *);
  static SoCallbackAction::Response sosphere_cb(void *, SoCallbackAction *, const SoNode *);

  // Property nodes
  static SoCallbackAction::Response soinfo_cb(void *, SoCallbackAction *, const SoNode *);
  static SoCallbackAction::Response somattrans_cb(void *, SoCallbackAction *, const SoNode *);
  static SoCallbackAction::Response sorotation_cb(void *, SoCallbackAction *, const SoNode *);
  static SoCallbackAction::Response sorotationxyz_cb(void *, SoCallbackAction *, const SoNode *);
  static SoCallbackAction::Response soscale_cb(void *, SoCallbackAction *, const SoNode *);
  static SoCallbackAction::Response sotransform_cb(void *, SoCallbackAction *, const SoNode *);
  static SoCallbackAction::Response sotranslation_cb(void *, SoCallbackAction *, const SoNode *);

  // Group nodes
  static SoCallbackAction::Response push_sep_cb(void *, SoCallbackAction *, const SoNode *);
  static SoCallbackAction::Response pop_sep_cb(void *, SoCallbackAction *, const SoNode *);
  static SoCallbackAction::Response push_switch_cb(void *, SoCallbackAction *, const SoNode *);
  static SoCallbackAction::Response pop_switch_cb(void *, SoCallbackAction *, const SoNode *);
  static SoCallbackAction::Response push_lod_cb(void *, SoCallbackAction *, const SoNode *);

  // Other nodes
  static SoCallbackAction::Response sopercam_cb(void *, SoCallbackAction *, const SoNode *);
  static SoCallbackAction::Response sodirlight_cb(void *, SoCallbackAction *, const SoNode *);
  static SoCallbackAction::Response sowwwinl_cb(void *, SoCallbackAction *, const SoNode *);

  // Convert nodes to SoVRMLIndexedFaceSet via triangle cb
  static SoCallbackAction::Response sotoifs_cb(void *, SoCallbackAction *, const SoNode *);
  static SoCallbackAction::Response post_primitives_cb(void *, SoCallbackAction *, const SoNode *);
  static void triangle_cb(void * userdata, SoCallbackAction * action,
                          const SoPrimitiveVertex * v1,
                          const SoPrimitiveVertex * v2,
                          const SoPrimitiveVertex * v3);

};

#endif // DOXYGEN_SKIP_THIS


#define PRIVATE(p) (p->pimpl)
#define PUBLIC(p) (p->master)
#define THISP(p) ((SoToVRML2ActionP*)p)

// *************************************************************************

SoToVRML2Action::SoToVRML2Action(void)
{
  SO_ACTION_CONSTRUCTOR(SoToVRML2Action);

  PRIVATE(this) = new SoToVRML2ActionP(this);

#define ADD_PRE_CB(_node_, _cb_) \
  PRIVATE(this)->cbaction.addPreCallback(_node_::getClassTypeId(), SoToVRML2ActionP::_cb_, PRIVATE(this))
#define ADD_POST_CB(_node_, _cb_) \
  PRIVATE(this)->cbaction.addPostCallback(_node_::getClassTypeId(), SoToVRML2ActionP::_cb_, PRIVATE(this))
#define ADD_UNSUPPORTED(_node_) \
  PRIVATE(this)->cbaction.addPreCallback(_node_::getClassTypeId(), SoToVRML2ActionP::unsupported_cb, PRIVATE(this))
#define ADD_TRIANGLE_CB(_node_) \
  PRIVATE(this)->cbaction.addTriangleCallback(_node_::getClassTypeId(), SoToVRML2ActionP::triangle_cb, PRIVATE(this))
#define ADD_SHAPE_CB(_node_, _cb_) \
  ADD_PRE_CB(_node_, _cb_); ADD_TRIANGLE_CB(_node_); ADD_POST_CB(_node_, post_primitives_cb);
#define ADD_SO_TO_IFS(_node_) \
  ADD_PRE_CB(_node_, sotoifs_cb); ADD_TRIANGLE_CB(_node_); ADD_POST_CB(_node_, post_primitives_cb);

  // Shape nodes
  ADD_SHAPE_CB(SoAsciiText, soasciitext_cb);
  ADD_SHAPE_CB(SoCone, socone_cb);
  ADD_SHAPE_CB(SoCube, socube_cb);
  ADD_SHAPE_CB(SoCylinder, socylinder_cb);
  ADD_SHAPE_CB(SoIndexedFaceSet, soifs_cb);
  ADD_SHAPE_CB(SoIndexedLineSet, soils_cb);
  ADD_SHAPE_CB(SoPointSet, sopointset_cb);
  ADD_SHAPE_CB(SoSphere, sosphere_cb);

  // Property nodes
  ADD_PRE_CB(SoInfo, soinfo_cb);
  ADD_PRE_CB(SoMatrixTransform, somattrans_cb);
  ADD_PRE_CB(SoRotation, sorotation_cb);
  ADD_PRE_CB(SoRotationXYZ, sorotationxyz_cb);
  ADD_PRE_CB(SoScale, soscale_cb);
  ADD_PRE_CB(SoTransform, sotransform_cb);
  ADD_PRE_CB(SoTranslation, sotranslation_cb);

  // Group nodes
  ADD_PRE_CB(SoSeparator, push_sep_cb);
  ADD_POST_CB(SoSeparator, pop_sep_cb);
  ADD_PRE_CB(SoSwitch, push_switch_cb);
  ADD_POST_CB(SoSwitch, pop_switch_cb);
  ADD_PRE_CB(SoLOD, push_lod_cb);
  ADD_UNSUPPORTED(SoWWWAnchor); // Convert to SoVRMLAnchor

  // Other nodes
  ADD_UNSUPPORTED(SoOrthographicCamera);
  ADD_PRE_CB(SoPerspectiveCamera, sopercam_cb);
  ADD_PRE_CB(SoDirectionalLight, sodirlight_cb);
  ADD_UNSUPPORTED(SoPointLight);
  ADD_UNSUPPORTED(SoSpotLight);
  ADD_PRE_CB(SoWWWInline, sowwwinl_cb);

  // Coin nodes
  ADD_SHAPE_CB(SoLineSet, solineset_cb);
  ADD_SO_TO_IFS(SoIndexedTriangleStripSet);
  ADD_SO_TO_IFS(SoFaceSet);
  ADD_SO_TO_IFS(SoQuadMesh);
  ADD_SO_TO_IFS(SoTriangleStripSet);

  ADD_SO_TO_IFS(SoNurbsCurve);
  ADD_SO_TO_IFS(SoNurbsSurface);
  ADD_SO_TO_IFS(SoIndexedNurbsCurve);
  ADD_SO_TO_IFS(SoIndexedNurbsSurface);

  ADD_SO_TO_IFS(SoProfile); // FIXME: Should this be here? 20020805 kristian.

#undef ADD_PRE_CB
#undef ADD_POST_CB
#undef ADD_UNSUPPORTED
#undef ADD_TRIANGLE_CB
#undef ADD_SO_TO_IFS
}

SoToVRML2Action::~SoToVRML2Action(void)
{
  if (PRIVATE(this)->vrml2path) {
    PRIVATE(this)->vrml2path->unref();
  }
  if (PRIVATE(this)->vrml2root) {
    PRIVATE(this)->vrml2root->unref();
  }

  delete PRIVATE(this);
}

// Documented in superclass.
void 
SoToVRML2Action::apply(SoNode * root)
{
  PRIVATE(this)->init();
  PRIVATE(this)->cbaction.apply(root);
}

// Documented in superclass.
void 
SoToVRML2Action::apply(SoPath * path)
{
  PRIVATE(this)->init();
  PRIVATE(this)->cbaction.apply(path);
}

// Documented in superclass.
void 
SoToVRML2Action::apply(const SoPathList & pathlist, SbBool obeysrules)
{
  PRIVATE(this)->init();
  PRIVATE(this)->cbaction.apply(pathlist, obeysrules);
}

// Documented in superclass.
void
SoToVRML2Action::beginTraversal(SoNode * node)
{  
  assert(0 && "should never get here");
}

SoNode *
SoToVRML2Action::getVRML2SceneGraph(void) const
{
  return PRIVATE(this)->vrml2root;
}

void
SoToVRML2Action::reuseAppearanceNodes(SbBool appearance)
{
  // FIXME: not implemented yet. 20020808 mortene.
}

SbBool
SoToVRML2Action::doReuseAppearanceNodes(void) const
{
  // FIXME: not implemented yet. 20020808 mortene.
  return FALSE;
}

void
SoToVRML2Action::reusePropertyNodes(SbBool property)
{
  // FIXME: not implemented yet. 20020808 mortene.
}

SbBool
SoToVRML2Action::doReusePropertyNodes(void) const
{
  // FIXME: not implemented yet. 20020808 mortene.
  return FALSE;
}

void
SoToVRML2Action::reuseGeometryNodes(SbBool geometry)
{
  // FIXME: not implemented yet. 20020808 mortene.
}

SbBool
SoToVRML2Action::doReuseGeometryNodes(void) const
{
  // FIXME: not implemented yet. 20020808 mortene.
  return FALSE;
}


SoNode * 
SoToVRML2ActionP::search_for_recent_node(SoAction * action, const SoType & type)
{
  this->searchaction.setSearchingAll(FALSE);
  this->searchaction.setType(type);
  this->searchaction.setInterest(SoSearchAction::LAST);
  this->searchaction.apply((SoPath *)action->getCurPath());
  SoNode * tail = NULL;
  SoFullPath * path = (SoFullPath*) this->searchaction.getPath();
  if (path) {
    tail = path->getTail();
  }
  this->searchaction.reset();
  return tail;
}

SoGroup * 
SoToVRML2ActionP::get_current_tail(void)
{
  SoNode * node = this->vrml2path->getTail();
  assert(node->isOfType(SoVRMLGroup::getClassTypeId()) ||
         node->isOfType(SoVRMLSwitch::getClassTypeId()) ||
         node->isOfType(SoVRMLLOD::getClassTypeId()));
  return (SoGroup*) node;
}

SoVRMLCoordinate *
SoToVRML2ActionP::get_or_create_coordinate(const SbVec4f * coord4, int32_t num)
{
  SbList <SbVec3f> vec3f;
  for (int i = 0; i < num; i++) {
    SbVec3f tmp;
    coord4[i].getReal(tmp);
    vec3f.append(tmp);
  }
  return this->get_or_create_coordinate(vec3f.getArrayPtr(), num);
}

SoVRMLCoordinate *
SoToVRML2ActionP::get_or_create_coordinate(const SbVec3f * coord3, int32_t num)
{
  if (this->reusePropertyNodes) {
    // Search for a matching VRMLCoordinate
    int n = this->vrmlcoords->getLength();
    while (--n >= 0) {
      SoVRMLCoordinate * c = (*this->vrmlcoords)[n];
      if (c->point.getNum() == num &&
          memcmp(coord3, c->point.getValues(0), num*sizeof(SbVec3f)) == 0) {
        return c;
      }
    }
  }

  // Create new
  SoVRMLCoordinate * c = new SoVRMLCoordinate;
  c->point.setValues(0, num, coord3);
  if (this->reusePropertyNodes) this->vrmlcoords->append(c);
  return c;
}

SoVRMLNormal *
SoToVRML2ActionP::get_or_create_normal(const SbVec3f * normal, int32_t num)
{
  if (this->reusePropertyNodes) {
    // Search for a matching VRMLNormal
    int n = this->vrmlnormals->getLength();
    while (--n >= 0) {
      SoVRMLNormal * nor = (*this->vrmlnormals)[n];
      if (nor->vector.getNum() == num &&
          memcmp(normal, nor->vector.getValues(0), num*sizeof(SbVec3f)) == 0) {
        return nor;
      }
    }
  }

  // Create new
  SoVRMLNormal * nor = new SoVRMLNormal;
  nor->vector.setValues(0, num, normal);
  if (this->reusePropertyNodes) this->vrmlnormals->append(nor);
  return nor;
}

SoVRMLColor *
SoToVRML2ActionP::get_or_create_color(const uint32_t * packedColor, int32_t num)
{
  // Convert to SbColors
  SbList <SbColor> color;
  float f;
  for (int i = 0; i < num; i++) {
    SbColor tmp;
    tmp.setPackedValue(packedColor[i], f);
    color.append(tmp);
  }
  return this->get_or_create_color(color.getArrayPtr(), num);
}

SoVRMLColor *
SoToVRML2ActionP::get_or_create_color(const SbColor * color, int32_t num)
{
  if (this->reusePropertyNodes) {
    // Search for a matching VRMLColor
    int n = this->vrmlcolors->getLength();
    while (--n >= 0) {
      SoVRMLColor * c = (*this->vrmlcolors)[n];
      if (c->color.getNum() == num &&
          memcmp(color, c->color.getValues(0), num*sizeof(SbColor)) == 0) {
        return c;
      }
    }
  }

  // Create new
  SoVRMLColor * c = new SoVRMLColor;
  c->color.setValues(0, num, color);
  if (this->reusePropertyNodes) this->vrmlcolors->append(c);
  return c;
}

SoVRMLTextureCoordinate *
SoToVRML2ActionP::get_or_create_texcoordinate(const SbVec2f * texcoord2, int32_t num)
{
  if (this->reusePropertyNodes) {
    // Search for a matching VRMLTextureCoordinate
    int n = this->vrmltexcoords->getLength();
    while (--n >= 0) {
      SoVRMLTextureCoordinate * tc = (*this->vrmltexcoords)[n];
      if (tc->point.getNum() == num &&
          memcmp(texcoord2, tc->point.getValues(0), num*sizeof(SbVec2f)) == 0) {
        return tc;
      }
    }
  }

  // Create new
  SoVRMLTextureCoordinate * tc = new SoVRMLTextureCoordinate;
  tc->point.setValues(0, num, texcoord2);
  if (this->reusePropertyNodes) this->vrmltexcoords->append(tc);
  return tc;
}

void
SoToVRML2ActionP::insert_shape(SoCallbackAction * action, SoVRMLGeometry * geom)
{
  SoVRMLShape * shape = new SoVRMLShape;
  shape->geometry = geom;
    
  // Create appearance
  SoVRMLAppearance * appearance = new SoVRMLAppearance;
  shape->appearance = appearance;

  SoVRMLMaterial * mat = new SoVRMLMaterial;
  appearance->material = mat;

  // Get values from current state
  SbColor ambient, diffuse, specular, emissions;
  float shin, transp;
  action->getMaterial(ambient, diffuse, specular, emissions, shin, transp);
  
  if (!geom->isOfType(SoVRMLPointSet::getClassTypeId())) {
    if (mat->diffuseColor.getValue() != diffuse) mat->diffuseColor = diffuse;
    
    // Convert to grayscale for calculating the ambient intensity
    float ambientGray = ambient[0] * 77 + ambient[1] * 150 + ambient[2] * 29;
    float diffuseGray = diffuse[0] * 77 + diffuse[1] * 150 + diffuse[2] * 29;
    if (ambientGray != 0 && diffuseGray != 0) {
      float ambientIntensity = ambientGray / diffuseGray;
      if (mat->ambientIntensity.getValue() != ambientIntensity);
      mat->ambientIntensity = ambientIntensity;
    }
    
    if (mat->specularColor.getValue() != specular) mat->specularColor = specular;    
    if (mat->emissiveColor.getValue() != emissions) mat->emissiveColor = emissions;
    if (mat->shininess.getValue() != shin) mat->shininess = shin;
    if (mat->transparency.getValue() != transp) mat->transparency = transp;
  
    // Texture
    if (this->recentTex2 == NULL) {
      this->recentTex2 = (SoTexture2 *) search_for_recent_node(action, SoTexture2::getClassTypeId());
    }
    
    if (this->recentTex2 != NULL) {
      SbVec2s size;
      int numComponents;
      const unsigned char * image = this->recentTex2->image.getValue(size, numComponents);
      if (size[0] > 0 && size[1] > 0) {
        SoVRMLTexture * tex;
        if (!this->recentTex2->filename.isDefault()) {
          tex = new SoVRMLImageTexture;
          SbString url = this->master->getUrlName();
          url += this->recentTex2->filename.getValue();
          ((SoVRMLImageTexture *)tex)->url.setValue(url);
        } else {
          tex = new SoVRMLPixelTexture;
          ((SoVRMLPixelTexture *)tex)->image.setValue(size, numComponents, image);
        }
        tex->repeatS = this->recentTex2->wrapS.getValue() == SoTexture2::REPEAT;
        tex->repeatT = this->recentTex2->wrapT.getValue() == SoTexture2::REPEAT;
        appearance->texture = tex;
      
        // Texture transform
        const SbMatrix * matrix = &action->getTextureMatrix();

        if (!matrix->equals(SbMatrix::identity(), 0.0f)) {
          SbVec3f translation, scaleFactor;
          SbRotation rotation, scaleOrientation;
          matrix->getTransform(translation, rotation, scaleFactor, scaleOrientation);

          SoVRMLTextureTransform * textrans = new SoVRMLTextureTransform;
          textrans->translation = SbVec2f(translation[0], translation[1]);

          SbVec3f axis;
          float radians;
          rotation.getValue(axis, radians);
          if (axis[2] < 0) radians = 2.0f*(float)M_PI - radians;
          textrans->rotation = radians;
            
          textrans->scale = SbVec2f(scaleFactor[0], scaleFactor[1]);
      
          appearance->textureTransform = textrans;
        }
      }
      this->recentTex2 = NULL;
    }
  } else {
    if (mat->emissiveColor.getValue() != diffuse) mat->emissiveColor = diffuse;
  }
    
  get_current_tail()->addChild(shape);
}

SoCallbackAction::Response 
SoToVRML2ActionP::push_sep_cb(void * closure, SoCallbackAction * action, const SoNode * node)
{
  SoToVRML2ActionP * thisp = (SoToVRML2ActionP*) closure;
  SoGroup * prevgroup = THISP(closure)->get_current_tail();

  void * vp;
  if (THISP(closure)->dict.find((unsigned long)node, vp)) {
    // Re-use previous subgraph
    prevgroup->addChild((SoNode*)vp);
    return SoCallbackAction::PRUNE;
  }
    
  // Push a new SoVRMLGroup on the tail of the path
  SoVRMLGroup * newgroup = new SoVRMLGroup;
  prevgroup->addChild(newgroup);
  thisp->vrml2path->append(newgroup);
  return SoCallbackAction::CONTINUE;
}

SoCallbackAction::Response 
SoToVRML2ActionP::pop_sep_cb(void * closure, SoCallbackAction * action, const SoNode * node) 
{
  void * vp;
  if (THISP(closure)->dict.find((unsigned long)node, vp)) {
    return SoCallbackAction::CONTINUE;
  }
    
  // Pop node from the tail of the path until an SoVRMLGroup has been poped
  SoGroup * grp;
  do {
    grp = THISP(closure)->get_current_tail();
    THISP(closure)->vrml2path->pop();
  } while (grp->getTypeId() != SoVRMLGroup::getClassTypeId());

  THISP(closure)->dict.enter((unsigned long)node, grp);  
  return SoCallbackAction::CONTINUE;
}

SoCallbackAction::Response 
SoToVRML2ActionP::push_switch_cb(void * closure, SoCallbackAction * action, const SoNode * node)
{
  SoToVRML2ActionP * thisp = (SoToVRML2ActionP*) closure;
  SoGroup * prevgroup = THISP(closure)->get_current_tail();

  void * vp;
  if (THISP(closure)->dict.find((unsigned long)node, vp)) {
    // Re-use previous subgraph
    prevgroup->addChild((SoNode*)vp);
    return SoCallbackAction::PRUNE;
  }

  const SoSwitch * oldswitch = (const SoSwitch *) node;  
  SoVRMLSwitch * newswitch = new SoVRMLSwitch;
  newswitch->whichChoice = oldswitch->whichChild.getValue();
  prevgroup->addChild(newswitch);
  thisp->vrml2path->append(newswitch);

  /* Traverse all children separately, that is, save and restore state between each.
   * If there is a selected child, traverse it normally afterwards.
   */
  if (oldswitch->whichChild.getValue() != SO_SWITCH_ALL) {
    int wc = oldswitch->whichChild.getValue() == SO_SWITCH_INHERIT ?
      action->getSwitch() : oldswitch->whichChild.getValue();

    int n = oldswitch->getNumChildren();
    for (int i=0; i < n; i++) {
      if (i != wc) {
        action->switchToNodeTraversal(oldswitch->getChild(i));
      }
    }
  }
  
  return SoCallbackAction::CONTINUE;
}

SoCallbackAction::Response 
SoToVRML2ActionP::pop_switch_cb(void * closure, SoCallbackAction * action, const SoNode * node) 
{
  void * vp;
  if (THISP(closure)->dict.find((unsigned long)node, vp)) {
    return SoCallbackAction::CONTINUE;
  }

  SoGroup * grp;
  do {
    grp = THISP(closure)->get_current_tail();
    THISP(closure)->vrml2path->pop();
  } while (grp->getTypeId() != SoVRMLSwitch::getClassTypeId());

  SoVRMLSwitch * sw = (SoVRMLSwitch *) grp;
  int wc = sw->whichChoice.getValue() == SO_SWITCH_INHERIT ?
    action->getSwitch() : sw->whichChoice.getValue();
  if (wc != SO_SWITCH_ALL && wc != SO_SWITCH_NONE) {
    // Move the last child (which is the selected child) to its correct position
    SoNode * n = sw->getChild(sw->getNumChildren()-1);
    sw->removeChild(sw->getNumChildren()-1);
    sw->insertChild(n, wc);
  }
    
  THISP(closure)->dict.enter((unsigned long)node, grp);  
  return SoCallbackAction::CONTINUE;
}

SoCallbackAction::Response 
SoToVRML2ActionP::push_lod_cb(void * closure, SoCallbackAction * action, const SoNode * node)
{
  SoToVRML2ActionP * thisp = (SoToVRML2ActionP*) closure;
  SoGroup * prevgroup = THISP(closure)->get_current_tail();

  void * vp;
  if (THISP(closure)->dict.find((unsigned long)node, vp)) {
    // Re-use previous subgraph
    prevgroup->addChild((SoNode*)vp);
    return SoCallbackAction::PRUNE;
  }

  const SoLOD * oldlod = (const SoLOD *) node;
  SoVRMLLOD * newlod = new SoVRMLLOD;
  
  newlod->range.setValues(0, oldlod->range.getNum(), oldlod->range.getValues(0));
  newlod->center = oldlod->center.getValue();
  
  prevgroup->addChild(newlod);
  thisp->vrml2path->append(newlod);

  // Traverse all children separately, that is, save and restore state between each
  int n = oldlod->getNumChildren();
  for (int i=0; i < n; i++) {
    action->switchToNodeTraversal(oldlod->getChild(i));
  }

  thisp->vrml2path->pop();
  THISP(closure)->dict.enter((unsigned long)node, newlod);
  return SoCallbackAction::PRUNE;
}

SoCallbackAction::Response 
SoToVRML2ActionP::unsupported_cb(void * closure, SoCallbackAction * action, const SoNode * node)
{
  SoVRMLWorldInfo * info = new SoVRMLWorldInfo;
  SbString str;
  str.sprintf("Unsupported node: %s",
              node->getTypeId().getName().getString());
  info->title = str;
  THISP(closure)->get_current_tail()->addChild(info);

  if (THISP(closure)->master->isVerbose()) {
      SoDebugError::postWarning("SoToVRML2Action::unsupported_cb", "%s", str.getString());
  }
  
  return SoCallbackAction::CONTINUE;
}

// Shape nodes
SoCallbackAction::Response
SoToVRML2ActionP::soasciitext_cb(void * closure, SoCallbackAction * action, const SoNode * node)
{
  if (action->getDrawStyle() != SoDrawStyle::FILLED) {
      return SoToVRML2ActionP::sotoifs_cb(closure, action, node);
  }
  SoVRMLText * text = new SoVRMLText;
  const SoAsciiText * oldtext = (const SoAsciiText*) node;

  text->string = oldtext->string;
  text->length = oldtext->width;
  // FIXME: FontStyle. 20020805 kristian.
  
  THISP(closure)->insert_shape(action, text);
  return SoCallbackAction::PRUNE;
}

SoCallbackAction::Response 
SoToVRML2ActionP::socube_cb(void * closure, SoCallbackAction * action, const SoNode * node)
{
  if (action->getDrawStyle() != SoDrawStyle::FILLED) {
      return SoToVRML2ActionP::sotoifs_cb(closure, action, node);
  }
  SoVRMLBox * box = new SoVRMLBox;
  const SoCube * cube = (const SoCube*) node;
  if (box->size.getValue()[0] != cube->width.getValue() ||
      box->size.getValue()[1] != cube->height.getValue() ||
      box->size.getValue()[2] != cube->depth.getValue()) {
    box->size.setValue(cube->width.getValue(), cube->height.getValue(), cube->depth.getValue());
  }
  THISP(closure)->insert_shape(action, box);
  return SoCallbackAction::PRUNE;
}

SoCallbackAction::Response 
SoToVRML2ActionP::socone_cb(void * closure, SoCallbackAction * action, const SoNode * node)
{
  if (action->getDrawStyle() != SoDrawStyle::FILLED) {
      return SoToVRML2ActionP::sotoifs_cb(closure, action, node);
  }
  SoVRMLCone * cone = new SoVRMLCone;
  const SoCone * oldcone = (const SoCone*) node;
  
  if (oldcone->bottomRadius != cone->bottomRadius)
    cone->bottomRadius = oldcone->bottomRadius.getValue();
  if (oldcone->height != cone->height)
    cone->height = oldcone->height.getValue();
  SbBool bottom = (oldcone->parts.getValue() & SoCone::BOTTOM) ? TRUE : FALSE; 
  if (bottom != cone->bottom.getValue()) cone->bottom = bottom;
  SbBool side = (oldcone->parts.getValue() & SoCone::SIDES) ? TRUE : FALSE; 
  if (side != cone->side.getValue()) cone->side = side;
  
  THISP(closure)->insert_shape(action, cone);
  return SoCallbackAction::PRUNE;
}

SoCallbackAction::Response
SoToVRML2ActionP::socylinder_cb(void * closure, SoCallbackAction * action, const SoNode * node)
{
  if (action->getDrawStyle() != SoDrawStyle::FILLED) {
      return SoToVRML2ActionP::sotoifs_cb(closure, action, node);
  }
  SoVRMLCylinder * cyl = new SoVRMLCylinder;
  const SoCylinder * oldcyl = (const SoCylinder*) node;

  if (oldcyl->radius != cyl->radius)
    cyl->radius = oldcyl->radius.getValue();
  if (oldcyl->height != cyl->height)
    cyl->height = oldcyl->height.getValue();

  SbBool side = (oldcyl->parts.getValue() & SoCylinder::SIDES) ? TRUE : FALSE;
  if (side != cyl->side.getValue()) cyl->side = side;
  SbBool top = (oldcyl->parts.getValue() & SoCylinder::TOP) ? TRUE : FALSE;
  if (top != cyl->top.getValue()) cyl->top = top;
  SbBool bottom = (oldcyl->parts.getValue() & SoCylinder::BOTTOM) ? TRUE : FALSE;
  if (bottom != cyl->bottom.getValue()) cyl->bottom = bottom;

  THISP(closure)->insert_shape(action, cyl);
  return SoCallbackAction::PRUNE;
}

SoCallbackAction::Response
SoToVRML2ActionP::soifs_cb(void * closure, SoCallbackAction * action, const SoNode * node)
{
  if (action->getDrawStyle() != SoDrawStyle::FILLED) {
      return SoToVRML2ActionP::sotoifs_cb(closure, action, node);
  }
  const SoIndexedFaceSet * oldifs = (const SoIndexedFaceSet*) node;
  
  if (oldifs->coordIndex.getNum() == 0 ||
      oldifs->coordIndex[0] < 0)
    return SoCallbackAction::CONTINUE;
  
  SoToVRML2ActionP * thisp = (SoToVRML2ActionP*) closure;
  SoGroup * tail = thisp->get_current_tail();
  SoVRMLIndexedFaceSet * ifs = new SoVRMLIndexedFaceSet;

  // Set the values from the current ShapeHints
  ifs->creaseAngle = action->getCreaseAngle();
  ifs->ccw = action->getVertexOrdering() != SoShapeHints::CLOCKWISE;
  ifs->solid = SoShapeHintsElement::getShapeType(action->getState()) == SoShapeHintsElement::SOLID;
  ifs->convex = action->getFaceType() == SoShapeHints::CONVEX;

  // If there is a VertexProperty node set we need to put it on the state stack
  if (oldifs->vertexProperty.getValue() != NULL) {
    action->getState()->push();
    ((SoVertexProperty *)oldifs->vertexProperty.getValue())->callback(action);
  }

  const SoCoordinateElement * coordElem = SoCoordinateElement::getInstance(action->getState());
  if (coordElem->getNum() > 0) {
    if (coordElem->getArrayPtr3() != NULL) {
      ifs->coord = thisp->get_or_create_coordinate(coordElem->getArrayPtr3(),
                                                   coordElem->getNum());
    } else {
      ifs->coord = thisp->get_or_create_coordinate(coordElem->getArrayPtr4(),
                                                   coordElem->getNum());
    }
  }

  if (action->getNormalBinding() != SoNormalBinding::OVERALL) {
    const SoNormalElement * normalElem = SoNormalElement::getInstance(action->getState());
    if (coordElem->getNum() > 0) {
      ifs->normal = thisp->get_or_create_normal(normalElem->getArrayPtr(),
                                                normalElem->getNum());
      if (action->getNormalBinding() != SoNormalBinding::PER_VERTEX_INDEXED &&
          action->getNormalBinding() != SoNormalBinding::PER_VERTEX) {
        ifs->normalPerVertex = FALSE;
      }
    }
  }

  if (action->getMaterialBinding() != SoMaterialBinding::OVERALL) {
    SoLazyElement * lazy = SoLazyElement::getInstance(action->getState());
    if (lazy->getNumDiffuse() > 1) {
      if (lazy->isPacked()) {
        ifs->color = thisp->get_or_create_color(lazy->getPackedPointer(),
                                                lazy->getNumDiffuse());
      } 
      else {
        ifs->color = thisp->get_or_create_color(lazy->getDiffusePointer(),
                                                lazy->getNumDiffuse());
      }
      if (action->getMaterialBinding() != SoMaterialBinding::PER_VERTEX_INDEXED &&
          action->getMaterialBinding() != SoMaterialBinding::PER_VERTEX) {
        ifs->colorPerVertex = FALSE;
      }
    }
  }

  const SoTextureCoordinateElement * texcoordElem =
    SoTextureCoordinateElement::getInstance(action->getState());
  if (texcoordElem->getNum() > 0) {
    ifs->texCoord = thisp->get_or_create_texcoordinate(texcoordElem->getArrayPtr2(),
                                                       texcoordElem->getNum());
  }

  if (oldifs->vertexProperty.getValue() != NULL) {
    action->getState()->pop();
  }
  
  ifs->coordIndex.setValues(0, oldifs->coordIndex.getNum(),
                            oldifs->coordIndex.getValues(0));
  if (oldifs->textureCoordIndex.getNum()) {
    ifs->texCoordIndex.setValues(0, oldifs->textureCoordIndex.getNum(),
                                 oldifs->textureCoordIndex.getValues(0));
  }
  if (oldifs->materialIndex.getNum()) {
    ifs->colorIndex.setValues(0, oldifs->materialIndex.getNum(),
                              oldifs->materialIndex.getValues(0));
  }
  if (oldifs->normalIndex.getNum()) {
    ifs->normalIndex.setValues(0, oldifs->normalIndex.getNum(),
                               oldifs->normalIndex.getValues(0));
  }

  THISP(closure)->insert_shape(action, ifs);
  return SoCallbackAction::PRUNE;
}

SoCallbackAction::Response
SoToVRML2ActionP::soils_cb(void * closure, SoCallbackAction * action, const SoNode * node)
{
  if (action->getDrawStyle() != SoDrawStyle::FILLED) {
      return SoToVRML2ActionP::sotoifs_cb(closure, action, node);
  }
  SoToVRML2ActionP * thisp = (SoToVRML2ActionP*) closure;
  
  const SoIndexedLineSet * oldils = (const SoIndexedLineSet*) node;

  if (oldils->coordIndex.getNum() == 0 ||
      oldils->coordIndex[0] < 0)
    return SoCallbackAction::CONTINUE;

  SoVRMLIndexedLineSet * ils = new SoVRMLIndexedLineSet;
  SoGroup * tail = thisp->get_current_tail();

  // If there is a VertexProperty node set we need to put it on the state stack
  if (oldils->vertexProperty.getValue() != NULL) {
    action->getState()->push();
    ((SoVertexProperty *)oldils->vertexProperty.getValue())->callback(action);
  }
  
  SoVRMLCoordinate * newcoord = NULL;
  const SoCoordinateElement * coordElem = SoCoordinateElement::getInstance(action->getState());
  if (coordElem->getNum() > 0) {
    if (thisp->nodefuse) {
      newcoord = new SoVRMLCoordinate;
    }
    else {
      if (coordElem->getArrayPtr3() != NULL) {
        newcoord = thisp->get_or_create_coordinate(coordElem->getArrayPtr3(),
                                                   coordElem->getNum());
      } else {
        newcoord = thisp->get_or_create_coordinate(coordElem->getArrayPtr4(),
                                                   coordElem->getNum());
      }
    }
    ils->coord = newcoord;
  }

  if (action->getMaterialBinding() != SoMaterialBinding::OVERALL) {
    const SoLazyElement * colorElem =
      SoLazyElement::getInstance(action->getState());
    if (colorElem->getNumDiffuse() > 0) {
      if (colorElem->isPacked()) {
        ils->color = thisp->get_or_create_color(colorElem->getPackedPointer(),
                                                colorElem->getNumDiffuse());
      } else {
        ils->color = thisp->get_or_create_color(colorElem->getDiffusePointer(),
                                                colorElem->getNumDiffuse());
      }
      if (action->getMaterialBinding() != SoMaterialBinding::PER_VERTEX_INDEXED &&
          action->getMaterialBinding() != SoMaterialBinding::PER_VERTEX) {
        ils->colorPerVertex = FALSE;
      }
    }
  }

  if (thisp->nodefuse && coordElem->getNum() > 0) {
    SbBSPTree bsp;
    int n = oldils->coordIndex.getNum();
    const int32_t * src = oldils->coordIndex.getValues(0);

    SbVec3f * c = (SbVec3f*) coordElem->getArrayPtr3();
    if (c == NULL) {
      SbVec3f * vec3f = new SbVec3f[coordElem->getNum()];
      const SbVec4f * coord4 = coordElem->getArrayPtr4();
      for (int i=coordElem->getNum()-1; i >= 0; i--) {
        coord4[i].getReal(vec3f[i]);
      }
      c = vec3f;
    }

    ils->coordIndex.setNum(n);
    int32_t * dst = ils->coordIndex.startEditing();

    for (int i = 0; i < n; i++) {
      int idx = src[i];
      if (idx >= 0) {
        dst[i] = bsp.addPoint(c[idx]);
      }
      else dst[i] = -1;
    }
    ils->coordIndex.finishEditing();
    newcoord->point.setValues(0, bsp.numPoints(),
                              bsp.getPointsArrayPtr());

    if (coordElem->getArrayPtr3() == NULL) delete[] c;
    
  }
  else {
    ils->coordIndex.setValues(0, oldils->coordIndex.getNum(),
                              oldils->coordIndex.getValues(0));
  }

  if (action->getMaterialBinding() == SoMaterialBinding::PER_VERTEX_INDEXED ||
      action->getMaterialBinding() == SoMaterialBinding::PER_FACE_INDEXED) {
    ils->colorIndex.setValues(0, oldils->materialIndex.getNum(),
                              oldils->materialIndex.getValues(0));
  }
  else if (action->getMaterialBinding() == SoMaterialBinding::PER_PART_INDEXED ||
           action->getMaterialBinding() == SoMaterialBinding::PER_PART) {

    // Color per segment, convert to per vertex
    SbList <int32_t> coordIdx;
    SbBSPTree bsp;
    int colidx = 0;
    SoVRMLColor * color = (SoVRMLColor *)ils->color.getValue();
    int n = ils->coordIndex.getNum()-1;
    for (int i = 0; i < n; i++) {
      SbVec3f curcol, nextcol;
      if (action->getMaterialBinding() == SoMaterialBinding::PER_PART_INDEXED) {
        curcol = color->color[oldils->materialIndex[colidx]];
        if (i != n-1)
          nextcol = color->color[oldils->materialIndex[colidx+1]];
      }
      else {
        curcol = color->color[colidx];
        if (i != n-1)
          nextcol = color->color[colidx+1];
      }
      colidx++;

      coordIdx.append(bsp.addPoint(curcol));

      if (i == n-1 || ils->coordIndex[i+2] == -1) {
        // Current polyline is done
        coordIdx.append(coordIdx[coordIdx.getLength()-1]);
        coordIdx.append(-1);
        i += 2;
      }
      else if (curcol != nextcol) {
        // Create a new vertex to avoid color interpolation
        ils->coordIndex.insertSpace(i+1, 1);
        ils->coordIndex.set1Value(i+1, ils->coordIndex[i+2]);
        coordIdx.append(bsp.addPoint(curcol));
        i++; n++;
      }
    }

    ils->color = thisp->get_or_create_color((const SbColor *)bsp.getPointsArrayPtr(),
                                            bsp.numPoints());

    ils->colorIndex.setValues(0, coordIdx.getLength(),
                              coordIdx.getArrayPtr());
    
    ils->colorPerVertex = TRUE;
  }

  if (oldils->vertexProperty.getValue() != NULL) {
    action->getState()->pop();
  }
  
  THISP(closure)->insert_shape(action, ils);
  return SoCallbackAction::PRUNE;
}

SoCallbackAction::Response
SoToVRML2ActionP::solineset_cb(void * closure, SoCallbackAction * action, const SoNode * node)
{
  if (action->getDrawStyle() != SoDrawStyle::FILLED) {
      return SoToVRML2ActionP::sotoifs_cb(closure, action, node);
  }
  SoToVRML2ActionP * thisp = (SoToVRML2ActionP*) closure;
  
  const SoLineSet * oldls = (const SoLineSet*) node;

  if (oldls->numVertices.getNum() == 0)
    return SoCallbackAction::CONTINUE;

  SoVRMLIndexedLineSet * ils = new SoVRMLIndexedLineSet;
  SoGroup * tail = thisp->get_current_tail();

  // If there is a VertexProperty node set we need to put it on the state stack
  if (oldls->vertexProperty.getValue() != NULL) {
    action->getState()->push();
    ((SoVertexProperty *)oldls->vertexProperty.getValue())->callback(action);
  }
  
  const SoCoordinateElement * coordElem = SoCoordinateElement::getInstance(action->getState());
  if (coordElem->getNum() > 0) {
    if (coordElem->getArrayPtr3() != NULL) {
      ils->coord = thisp->get_or_create_coordinate(coordElem->getArrayPtr3(),
                                                   coordElem->getNum());
    } else {
      ils->coord = thisp->get_or_create_coordinate(coordElem->getArrayPtr4(),
                                                   coordElem->getNum());
    }
  }

  if (action->getMaterialBinding() != SoMaterialBinding::OVERALL) {
    const SoLazyElement * colorElem =
      SoLazyElement::getInstance(action->getState());
    if (colorElem->getNumDiffuse() > 0) {
      if (colorElem->isPacked()) {
        ils->color = thisp->get_or_create_color(colorElem->getPackedPointer(),
                                                colorElem->getNumDiffuse());
      } else {
        ils->color = thisp->get_or_create_color(colorElem->getDiffusePointer(),
                                                colorElem->getNumDiffuse());
      }
      if (action->getMaterialBinding() != SoMaterialBinding::PER_VERTEX) {
        ils->colorPerVertex = FALSE;
      }
    }
  }
  
  int n = oldls->numVertices.getNum();
  int curidx = 0;
  SbList <int> l;
  for (int i = 0; i < n; i++) {
    for (int j = oldls->numVertices[i]-1; j >= 0; j--) {
      l.append(curidx++);
    }
    l.append(-1);
  }
  ils->coordIndex.setValues(0, l.getLength(),
                            l.getArrayPtr());

  if (action->getMaterialBinding() == SoMaterialBinding::PER_PART) {
    // Color per segment, convert to per vertex
    SbList <int32_t> coordIdx;
    SbBSPTree bsp;
    int colidx = 0;
    SoVRMLColor * color = (SoVRMLColor *)ils->color.getValue();
    int n = ils->coordIndex.getNum()-1;
    for (int i = 0; i < n; i++) {
      SbVec3f curcol, nextcol;
      
      curcol = color->color[colidx];
      if (i != n-1)
        nextcol = color->color[colidx+1];
      colidx++;
      
      coordIdx.append(bsp.addPoint(curcol));

      if (i == n-1 || ils->coordIndex[i+2] == -1) {
        // Current polyline is done
        coordIdx.append(coordIdx[coordIdx.getLength()-1]);
        coordIdx.append(-1);
        i += 2;
      }
      else if (curcol != nextcol) {
        // Create a new vertex to avoid color interpolation
        ils->coordIndex.insertSpace(i+1, 1);
        ils->coordIndex.set1Value(i+1, ils->coordIndex[i+2]);
        coordIdx.append(bsp.addPoint(curcol));
        i++; n++;
      }
    }

    ils->color = thisp->get_or_create_color((const SbColor *)bsp.getPointsArrayPtr(),
                                            bsp.numPoints());

    ils->colorIndex.setValues(0, coordIdx.getLength(),
                              coordIdx.getArrayPtr());
    
    ils->colorPerVertex = TRUE;
  }

  if (oldls->vertexProperty.getValue() != NULL) {
    action->getState()->pop();
  }
  
  THISP(closure)->insert_shape(action, ils);
  return SoCallbackAction::PRUNE;
}

SoCallbackAction::Response
SoToVRML2ActionP::sopointset_cb(void * closure, SoCallbackAction * action, const SoNode * node)
{
  if (action->getDrawStyle() != SoDrawStyle::FILLED) {
      return SoToVRML2ActionP::sotoifs_cb(closure, action, node);
  }
  SoToVRML2ActionP * thisp = (SoToVRML2ActionP*) closure;

  const SoPointSet * oldps = (const SoPointSet*) node;

  SoVRMLPointSet * ps = new SoVRMLPointSet;
  SoGroup * tail = thisp->get_current_tail();

  // If there is a VertexProperty node set we need to put it on the state stack
  if (oldps->vertexProperty.getValue() != NULL) {
    action->getState()->push();
    ((SoVertexProperty *)oldps->vertexProperty.getValue())->callback(action);
  }

  const SoCoordinateElement * coordElem = SoCoordinateElement::getInstance(action->getState());
  if (coordElem->getNum() > 0) {
    if (coordElem->getArrayPtr3() != NULL) {
      ps->coord = thisp->get_or_create_coordinate(coordElem->getArrayPtr3(),
                                                  SbMin(coordElem->getNum(),
                                                        oldps->numPoints.getValue()));
    } else {
      ps->coord = thisp->get_or_create_coordinate(coordElem->getArrayPtr4(),
                                                  SbMin(coordElem->getNum(),
                                                        oldps->numPoints.getValue()));
    } 
  }

  if (action->getMaterialBinding() != SoMaterialBinding::OVERALL) {
    const SoLazyElement * colorElem = SoLazyElement::getInstance(action->getState());
    int n = ((SoVRMLCoordinate*)ps->coord.getValue())->point.getNum();
    if (colorElem->getNumDiffuse() >= n) {
      if (colorElem->isPacked()) {
        ps->color = thisp->get_or_create_color(colorElem->getPackedPointer(),
                                               n);
      } else {
        ps->color = thisp->get_or_create_color(colorElem->getDiffusePointer(),
                                               n);
      }
    }
  }

  if (oldps->vertexProperty.getValue() != NULL) {
    action->getState()->pop();
  }

  THISP(closure)->insert_shape(action, ps);
  return SoCallbackAction::PRUNE;
}

SoCallbackAction::Response
SoToVRML2ActionP::sosphere_cb(void * closure, SoCallbackAction * action, const SoNode * node)
{
  if (action->getDrawStyle() != SoDrawStyle::FILLED) {
      return SoToVRML2ActionP::sotoifs_cb(closure, action, node);
  }
  SoVRMLSphere * sphere = new SoVRMLSphere;
  const SoSphere * oldsphere = (const SoSphere*) node;
  if (oldsphere->radius != sphere->radius)
    sphere->radius = oldsphere->radius.getValue();
  THISP(closure)->insert_shape(action, sphere);
  return SoCallbackAction::PRUNE;
}

// Property nodes
SoCallbackAction::Response 
SoToVRML2ActionP::soinfo_cb(void * closure, SoCallbackAction * action, const SoNode * node)
{
  const SoInfo * oldinfo = (const SoInfo*) node;
  SoVRMLWorldInfo * info = new SoVRMLWorldInfo;
  info->title = oldinfo->string;
  THISP(closure)->get_current_tail()->addChild(info);
  return SoCallbackAction::CONTINUE;
}

SoCallbackAction::Response 
SoToVRML2ActionP::somattrans_cb(void * closure, SoCallbackAction * action, const SoNode * node)
{
  const SoMatrixTransform * oldt = (const SoMatrixTransform*) node;
  SoVRMLTransform * newt = new SoVRMLTransform;

  SbVec3f translation, scaleFactor;
  SbRotation rotation, scaleOrientation;
  oldt->matrix.getValue().getTransform(translation, rotation, scaleFactor, scaleOrientation);

  newt->translation = translation.getValue();
  newt->rotation = rotation.getValue();
  newt->scale = scaleFactor.getValue();
  newt->scaleOrientation = scaleOrientation.getValue();
  THISP(closure)->get_current_tail()->addChild(newt);
  THISP(closure)->vrml2path->append(newt);
  return SoCallbackAction::CONTINUE;
}

SoCallbackAction::Response 
SoToVRML2ActionP::sorotation_cb(void * closure, SoCallbackAction * action, const SoNode * node)
{
  const SoRotation * oldt = (const SoRotation*) node;
  SoVRMLTransform * newt = new SoVRMLTransform;
  newt->rotation = oldt->rotation.getValue();
  THISP(closure)->get_current_tail()->addChild(newt);
  THISP(closure)->vrml2path->append(newt);
  return SoCallbackAction::CONTINUE;
}

SoCallbackAction::Response 
SoToVRML2ActionP::sorotationxyz_cb(void * closure, SoCallbackAction * action, const SoNode * node)
{
  const SoRotationXYZ * oldt = (const SoRotationXYZ*) node;
  SoVRMLTransform * newt = new SoVRMLTransform;
  newt->rotation = oldt->getRotation();
  THISP(closure)->get_current_tail()->addChild(newt);
  THISP(closure)->vrml2path->append(newt);
  return SoCallbackAction::CONTINUE;
}

SoCallbackAction::Response 
SoToVRML2ActionP::soscale_cb(void * closure, SoCallbackAction * action, const SoNode * node)
{
  const SoScale * oldt = (const SoScale*) node;
  SoVRMLTransform * newt = new SoVRMLTransform;
  newt->scale = oldt->scaleFactor.getValue();
  THISP(closure)->get_current_tail()->addChild(newt);
  THISP(closure)->vrml2path->append(newt);
  return SoCallbackAction::CONTINUE;
}

SoCallbackAction::Response 
SoToVRML2ActionP::sotransform_cb(void * closure, SoCallbackAction * action, const SoNode * node)
{
  const SoTransform * oldt = (const SoTransform*) node;
  SoVRMLTransform * newt = new SoVRMLTransform;

  newt->translation = oldt->translation.getValue();
  newt->rotation = oldt->rotation.getValue();
  newt->scale = oldt->scaleFactor.getValue();
  newt->scaleOrientation = oldt->scaleOrientation.getValue();
  newt->center = oldt->center.getValue();
  THISP(closure)->get_current_tail()->addChild(newt);
  THISP(closure)->vrml2path->append(newt);
  return SoCallbackAction::CONTINUE;
}

SoCallbackAction::Response 
SoToVRML2ActionP::sotranslation_cb(void * closure, SoCallbackAction * action, const SoNode * node)
{
  const SoTranslation * oldt = (const SoTranslation*) node;
  SoVRMLTransform * newt = new SoVRMLTransform;
  newt->translation = oldt->translation.getValue();
  THISP(closure)->get_current_tail()->addChild(newt);
  THISP(closure)->vrml2path->append(newt);
  return SoCallbackAction::CONTINUE;
}

// Other nodes
SoCallbackAction::Response
SoToVRML2ActionP::sopercam_cb(void * closure, SoCallbackAction * action, const SoNode * node)
{
    return unsupported_cb(closure, action, node);
}

SoCallbackAction::Response
SoToVRML2ActionP::sodirlight_cb(void * closure, SoCallbackAction * action, const SoNode * node)
{
  SoVRMLDirectionalLight * dl = new SoVRMLDirectionalLight;
  const SoDirectionalLight * olddl = (const SoDirectionalLight *) node;
  
  dl->direction = olddl->direction.getValue();
  dl->on = olddl->on.getValue();
  dl->intensity = olddl->intensity.getValue();
  dl->color = olddl->color.getValue();
  // FIXME: SoDirectionalLight seems to not support this? 20020805 kristian.
  //dl->ambientIntensity = ambient.getValue()[0] / diffuse.getValue()[0];
  
  THISP(closure)->get_current_tail()->addChild(dl);
  return SoCallbackAction::CONTINUE;
}

SoCallbackAction::Response
SoToVRML2ActionP::sowwwinl_cb(void * closure, SoCallbackAction * action, const SoNode * node)
{
  SoVRMLInline * inl = new SoVRMLInline;
  const SoWWWInline * oldinl = (const SoWWWInline *) node;
  
  inl->url = oldinl->name.getValue();
  inl->bboxCenter = oldinl->bboxCenter.getValue();
  inl->bboxSize = oldinl->bboxSize.getValue();
  
  THISP(closure)->get_current_tail()->addChild(inl);
  return SoCallbackAction::CONTINUE;
}

// Convert nodes to ifs
SoCallbackAction::Response 
SoToVRML2ActionP::sotoifs_cb(void * closure, SoCallbackAction * action, const SoNode * node)
{
  SoToVRML2ActionP * thisp = (SoToVRML2ActionP*) closure;
  SoGroup * tail = thisp->get_current_tail();
  
  thisp->bsptree = new SbBSPTree;
  thisp->bsptreenormal = new SbBSPTree;

  thisp->coordidx = new SbList <int>;
  thisp->normalidx = new SbList <int>;

  if (action->getMaterialBinding() != SoMaterialBinding::OVERALL) {
    const SoLazyElement * colorElem = SoLazyElement::getInstance(action->getState());
    if (colorElem->getNumDiffuse() > 1) {
      thisp->coloridx = new SbList <int>;
    }
  }

  thisp->recentTex2 =
      (SoTexture2 *) thisp->search_for_recent_node(action, SoTexture2::getClassTypeId());
  if (thisp->recentTex2) {
    thisp->bsptreetex = new SbBSPTree;
    thisp->texidx = new SbList <int>;
  }
  
  thisp->do_post_primitives = TRUE;

  return SoCallbackAction::CONTINUE;
}

void 
SoToVRML2ActionP::triangle_cb(void * closure, SoCallbackAction * action,
                             const SoPrimitiveVertex * v1,
                             const SoPrimitiveVertex * v2,
                             const SoPrimitiveVertex * v3)
{
  SoToVRML2ActionP * thisp = (SoToVRML2ActionP*) closure;
  assert(thisp->bsptree);
  assert(thisp->bsptreenormal);

  SoPrimitiveVertex const * const arr[3] = {v1, v2, v3};
  for (int i = 0; i < 3; i++) {
    const SoPrimitiveVertex * v = arr[i];
    thisp->coordidx->append(thisp->bsptree->addPoint(v->getPoint()));
    thisp->normalidx->append(thisp->bsptreenormal->addPoint(v->getNormal()));
    if (thisp->texidx) {
      assert(thisp->bsptreetex);
      const SbVec4f & tc = v->getTextureCoords();
      thisp->texidx->append(thisp->bsptreetex->addPoint(SbVec3f(tc[0], tc[1], 0.0f)));
    }
    if (thisp->coloridx) thisp->coloridx->append(v->getMaterialIndex());
  }
  thisp->coordidx->append(-1);
  thisp->normalidx->append(-1);
  if (thisp->texidx) thisp->texidx->append(-1);
  if (thisp->coloridx) thisp->coloridx->append(-1);
}

SoCallbackAction::Response 
SoToVRML2ActionP::post_primitives_cb(void * closure, SoCallbackAction * action, const SoNode * node)
{
  SoToVRML2ActionP * thisp = (SoToVRML2ActionP*) closure;
  if (!thisp->do_post_primitives) return SoCallbackAction::CONTINUE;
  thisp->do_post_primitives = FALSE;
  
  SoVRMLGeometry * is;
  if (action->getDrawStyle() == SoDrawStyle::POINTS) {
    SoVRMLPointSet * ps = new SoVRMLPointSet;
    is = ps;

    ps->coord = thisp->get_or_create_coordinate(thisp->bsptree->getPointsArrayPtr(),
                                                thisp->bsptree->numPoints());
      
    if (thisp->coloridx) {
      // Copy the colors from the state
      SoLazyElement * colorElem = SoLazyElement::getInstance(action->getState());
      if (colorElem->getNumDiffuse() == thisp->bsptree->numPoints()) {
        if (colorElem->isPacked()) {
          ps->color = thisp->get_or_create_color(colorElem->getPackedPointer(),
                                                 colorElem->getNumDiffuse());
        } 
        else {
          ps->color = thisp->get_or_create_color(colorElem->getDiffusePointer(),
                                                 colorElem->getNumDiffuse());
        }
      }
    }

  } else
  if (action->getDrawStyle() == SoDrawStyle::LINES) {
    SoVRMLIndexedLineSet * ils = new SoVRMLIndexedLineSet;
    is = ils;

    ils->coord = thisp->get_or_create_coordinate(thisp->bsptree->getPointsArrayPtr(),
                                                 thisp->bsptree->numPoints());

    if (thisp->coloridx) {
      // Copy the colors from the state
      const SoLazyElement * colorElem = SoLazyElement::getInstance(action->getState());
      if (colorElem->isPacked()) {
        ils->color = thisp->get_or_create_color(colorElem->getPackedPointer(),
                                                colorElem->getNumDiffuse());
      } else {
        ils->color = thisp->get_or_create_color(colorElem->getDiffusePointer(),
                                                colorElem->getNumDiffuse());
      }
      
      // Index
      ils->colorIndex.setValues(0, thisp->coloridx->getLength(),
                                thisp->coloridx->getArrayPtr());  
    }

    int n = thisp->coordidx->getLength();
    const int * a = thisp->coordidx->getArrayPtr();
    SbList <int> l;
    int p = a[0];
    for (int i = 0; i < n; i++) {
      if (a[i] == -1) {
        l.append(p);
        if (i < n-1) p = a[i+1];
      }
      l.append(a[i]);
    }
    ils->coordIndex.setValues(0, l.getLength(),
                              l.getArrayPtr());
    
  } else {
    SoVRMLIndexedFaceSet * ifs = new SoVRMLIndexedFaceSet;
    is = ifs;

    // Set the values from the current ShapeHints
    ifs->creaseAngle = action->getCreaseAngle();
    if (node->isOfType(SoVertexShape::getClassTypeId())) {
      ifs->ccw = action->getVertexOrdering() != SoShapeHints::CLOCKWISE;
    } else {
      ifs->ccw = TRUE;
    }
    ifs->solid = SoShapeHintsElement::getShapeType(action->getState()) == SoShapeHintsElement::SOLID;
    ifs->convex = action->getFaceType() == SoShapeHints::CONVEX;

    ifs->coord = thisp->get_or_create_coordinate(thisp->bsptree->getPointsArrayPtr(),
                                                thisp->bsptree->numPoints());
  
    ifs->normal = thisp->get_or_create_normal(thisp->bsptreenormal->getPointsArrayPtr(),
                                             thisp->bsptreenormal->numPoints());

    if (thisp->coloridx) {
      // Copy the colors from the state
      const SoLazyElement * colorElem = SoLazyElement::getInstance(action->getState());
      if (colorElem->isPacked()) {
        ifs->color = thisp->get_or_create_color(colorElem->getPackedPointer(),
                                                colorElem->getNumDiffuse());
      } else {
        ifs->color = thisp->get_or_create_color(colorElem->getDiffusePointer(),
                                                colorElem->getNumDiffuse());
      }
      
      // Index
      ifs->colorIndex.setValues(0, thisp->coloridx->getLength(),
                                thisp->coloridx->getArrayPtr());

    }

    if (thisp->texidx) {
      // Copy texture coordinates
      SoVRMLTextureCoordinate * tex = new SoVRMLTextureCoordinate;
      int n = thisp->bsptreetex->numPoints();
      tex->point.setNum(n);
      SbVec2f * ptr = tex->point.startEditing();
      for (int i = 0; i < n; i++) {
        SbVec3f p = thisp->bsptreetex->getPoint(i);
        ptr[i] = SbVec2f(p[0], p[1]);
      }
      tex->point.finishEditing();
      ifs->texCoord = tex;

      // Index
      ifs->texCoordIndex.setValues(0, thisp->texidx->getLength(),
                                  thisp->texidx->getArrayPtr());
    }

    ifs->coordIndex.setValues(0, thisp->coordidx->getLength(),
                             thisp->coordidx->getArrayPtr());
    ifs->normalIndex.setValues(0, thisp->normalidx->getLength(),
                              thisp->normalidx->getArrayPtr());

  }

  delete thisp->bsptree; thisp->bsptree = NULL;
  delete thisp->bsptreetex; thisp->bsptreetex = NULL;
  delete thisp->bsptreenormal; thisp->bsptreenormal = NULL;
  
  delete thisp->coordidx; thisp->coordidx = NULL;
  delete thisp->normalidx; thisp->normalidx = NULL;
  delete thisp->texidx; thisp->texidx = NULL;
  delete thisp->coloridx; thisp->coloridx = NULL;

  thisp->insert_shape(action, is);
  return SoCallbackAction::CONTINUE;
}

#endif // HAVE_VRML97
