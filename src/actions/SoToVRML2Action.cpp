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
  \class SoToVRML2Action Inventor/include/SoToVRML2Action.h
  \brief The SoToVRML2Action class builds a new scene graph, using only
  VRML 2.0 nodes.
*/

#include <Inventor/SbName.h>
#include <Inventor/actions/SoToVRML2Action.h>
#include <Inventor/actions/SoCallbackAction.h>
#include <Inventor/actions/SoSearchAction.h>

#include <Inventor/nodes/SoNodes.h>
#include <Inventor/elements/SoCoordinateElement.h>
#include <Inventor/elements/SoNormalElement.h>
#include <Inventor/elements/SoDiffuseColorElement.h>
#include <Inventor/elements/SoTextureCoordinateElement.h>

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

#ifdef HAVE_VRML97
#include <Inventor/VRMLnodes/SoVRMLNodes.h>
#include <Inventor/VRMLnodes/SoVRML.h>
#endif // HAVE_VRML97

SO_ACTION_SOURCE(SoToVRML2Action);

// *************************************************************************

#ifndef DOXYGEN_SKIP_THIS

class SoToVRML2ActionP {
public:
  SoToVRML2ActionP(SoToVRML2Action * master)
  {
    this->nodefuse = FALSE; // for optimizing bad scene graphs
    this->vrml2path = NULL;
    this->vrml2root = NULL;
  }

  void init(void) {
      
#ifdef HAVE_VRML97
    this->vrml2root = new SoVRMLGroup;
    this->vrmlcoords = new SbList <SoVRMLCoordinate *>;
    this->vrmlnormals = new SbList <SoVRMLNormal *>;
    this->vrmlcolors = new SbList <SoVRMLColor *>;
    this->vrmltexcoords = new SbList <SoVRMLTextureCoordinate *>;
#endif // HAVE_VRML97

    if (this->vrml2path) {
      this->vrml2path->unref();
    }
    this->vrml2path = (SoFullPath*) new SoPath;
    this->vrml2path->ref();
    
    if (this->vrml2root) {
      this->vrml2root->unref();
    }
    this->vrml2root->ref();
    this->vrml2path->setHead(this->vrml2root);
    
  }

  SoToVRML2Action * master;
  SbBool nodefuse;
  SbDict dict;
  SoCallbackAction cbaction;
  SoSearchAction searchaction;
  SoFullPath * vrml2path;

#ifdef HAVE_VRML97
  static SoCallbackAction::Response unsupported_cb(void *, SoCallbackAction *, const SoNode *);
  
  SoVRMLGroup * vrml2root;
  SbList <SoVRMLCoordinate *> * vrmlcoords;
  SbList <SoVRMLNormal *> * vrmlnormals;
  SbList <SoVRMLColor *> * vrmlcolors;
  SbList <SoVRMLTextureCoordinate *> * vrmltexcoords;
    
  SoGroup * get_current_tail(void);
  SoVRMLCoordinate * get_or_create_coordinate(const SbVec3f *, int32_t num);
  SoVRMLNormal * get_or_create_normal(const SbVec3f *, int32_t num);
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
  static SoCallbackAction::Response sopointset_cb(void *, SoCallbackAction *, const SoNode *);
  static SoCallbackAction::Response sosphere_cb(void *, SoCallbackAction *, const SoNode *);

  // Property nodes
  static SoCallbackAction::Response soinfo_cb(void *, SoCallbackAction *, const SoNode *);
  static SoCallbackAction::Response somattrans_cb(void *, SoCallbackAction *, const SoNode *);
  static SoCallbackAction::Response sorotation_cb(void *, SoCallbackAction *, const SoNode *);
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
#endif // HAVE_VRML97

};

#endif // DOXYGEN_SKIP_THIS


#define PRIVATE(p) (p->pimpl)
#define PUBLIC(p) (p->master)
#define THISP(p) ((SoToVRML2ActionP*)p)

// *************************************************************************

// Overridden from parent class.
void
SoToVRML2Action::initClass(void)
{
  SO_ACTION_INIT_CLASS(SoToVRML2Action, SoToVRMLAction);
}


/*!
  Constructor.
*/

SoToVRML2Action::SoToVRML2Action(void)
{
  PRIVATE(this) = new SoToVRML2ActionP(this);

#define ADD_PRE_CB(_node_, _cb_) \
  PRIVATE(this)->cbaction.addPreCallback(_node_::getClassTypeId(), SoToVRML2ActionP::_cb_, PRIVATE(this))
#define ADD_POST_CB(_node_, _cb_) \
  PRIVATE(this)->cbaction.addPostCallback(_node_::getClassTypeId(), SoToVRML2ActionP::_cb_, PRIVATE(this))
#define ADD_UNSUPPORTED(_node_) \
  PRIVATE(this)->cbaction.addPreCallback(_node_::getClassTypeId(), SoToVRML2ActionP::unsupported_cb, PRIVATE(this))

#ifdef HAVE_VRML97

  // Shape nodes
  ADD_PRE_CB(SoAsciiText, soasciitext_cb);
  ADD_PRE_CB(SoCone, socone_cb);
  ADD_PRE_CB(SoCube, socube_cb);
  ADD_PRE_CB(SoCylinder, socylinder_cb);
  ADD_PRE_CB(SoIndexedFaceSet, soifs_cb);
  ADD_PRE_CB(SoIndexedLineSet, soils_cb);
  ADD_PRE_CB(SoPointSet, sopointset_cb);
  ADD_PRE_CB(SoSphere, sosphere_cb);

  // Property nodes
  ADD_PRE_CB(SoInfo, soinfo_cb);
  ADD_PRE_CB(SoMatrixTransform, somattrans_cb);
  ADD_PRE_CB(SoRotation, sorotation_cb);
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

#endif // HAVE_VRML97

#undef ADD_PRE_CB
#undef ADD_POST_CB
#undef ADD_UNSUPPORTED
}

/*!
  The destructor.
*/

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

#ifdef HAVE_VRML97

SoNode *
SoToVRML2Action::getVRML2SceneGraph(void) const
{
  return PRIVATE(this)->vrml2root;
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
SoToVRML2ActionP::get_or_create_coordinate(const SbVec3f * coord3, int32_t num)
{
  // Search for a matching VRMLCoordinate
  int n = this->vrmlcoords->getLength();
  while (--n >= 0) {
    SoVRMLCoordinate * c = (*this->vrmlcoords)[n];
    if (c->point.getNum() == num &&
        memcmp(coord3, c->point.getValues(0), num*sizeof(SbVec3f)) == 0) {
      return c;
    }
  }

  // Create new
  SoVRMLCoordinate * c = new SoVRMLCoordinate;
  c->point.setValues(0, num, coord3);
  this->vrmlcoords->append(c);
  return c;
}

SoVRMLNormal *
SoToVRML2ActionP::get_or_create_normal(const SbVec3f * normal, int32_t num)
{
  // Search for a matching VRMLNormal
  int n = this->vrmlnormals->getLength();
  while (--n >= 0) {
    SoVRMLNormal * nor = (*this->vrmlnormals)[n];
    if (nor->vector.getNum() == num &&
        memcmp(normal, nor->vector.getValues(0), num*sizeof(SbVec3f)) == 0) {
      return nor;
    }
  }

  // Create new
  SoVRMLNormal * nor = new SoVRMLNormal;
  nor->vector.setValues(0, num, normal);
  this->vrmlnormals->append(nor);
  return nor;
}

SoVRMLColor *
SoToVRML2ActionP::get_or_create_color(const SbColor * color, int32_t num)
{
  // Search for a matching VRMLColor
  int n = this->vrmlcolors->getLength();
  while (--n >= 0) {
    SoVRMLColor * c = (*this->vrmlcolors)[n];
    if (c->color.getNum() == num &&
        memcmp(color, c->color.getValues(0), num*sizeof(SbColor)) == 0) {
      return c;
    }
  }

  // Create new
  SoVRMLColor * c = new SoVRMLColor;
  c->color.setValues(0, num, color);
  this->vrmlcolors->append(c);
  return c;
}

SoVRMLTextureCoordinate *
SoToVRML2ActionP::get_or_create_texcoordinate(const SbVec2f * texcoord2, int32_t num)
{
  // Search for a matching VRMLTextureCoordinate
  int n = this->vrmltexcoords->getLength();
  while (--n >= 0) {
    SoVRMLTextureCoordinate * tc = (*this->vrmltexcoords)[n];
    if (tc->point.getNum() == num &&
        memcmp(texcoord2, tc->point.getValues(0), num*sizeof(SbVec2f)) == 0) {
      return tc;
    }
  }

  // Create new
  SoVRMLTextureCoordinate * tc = new SoVRMLTextureCoordinate;
  tc->point.setValues(0, num, texcoord2);
  this->vrmltexcoords->append(tc);
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

  // Get values from current state
  SbColor ambient, diffuse, specular, emissions;
  float shin, transp;
  action->getMaterial(ambient, diffuse, specular, emissions, shin, transp);

  if (mat->diffuseColor.getValue() != diffuse) mat->diffuseColor = diffuse;

  float ambientIntensity = ambient.getValue()[0] / diffuse.getValue()[0];
  if (mat->ambientIntensity.getValue() != ambientIntensity)
    mat->ambientIntensity = ambientIntensity;
    
  if (mat->specularColor.getValue() != specular) mat->specularColor = specular;    
  if (mat->emissiveColor.getValue() != emissions) mat->emissiveColor = emissions;
  if (mat->shininess.getValue() != shin) mat->shininess = shin;
  if (mat->transparency.getValue() != transp) mat->transparency = transp;
  if (appearance->material.getValue() != mat) appearance->material = mat;

  // Texture
  SbVec2s size;
  int numComponents;
  SoTextureImageElement::Wrap wrapS, wrapT;
  SoTextureImageElement::Model model;
  SbColor blendColor;
  const unsigned char * image = SoTextureImageElement::get(
    action->getState(), size, numComponents, wrapS, wrapT, model, blendColor);
  if (size[0] > 0 && size[1] > 0) {
    SoVRMLPixelTexture * tex = new SoVRMLPixelTexture;
    tex->image.setValue(size, numComponents, image);
    tex->repeatS = wrapS == SoTextureImageElement::REPEAT;
    tex->repeatT = wrapT == SoTextureImageElement::REPEAT;

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

      appearance->texture = tex;
      appearance->textureTransform = textrans;
    }
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
  if (sw->whichChoice.getValue() != SO_SWITCH_ALL &&
      sw->whichChoice.getValue() != SO_SWITCH_NONE) {
    // Move the last child (which is the selected child) to its correct position
    SoNode * n = sw->getChild(sw->getNumChildren()-1);
    sw->removeChild(sw->getNumChildren()-1);
    sw->insertChild(n, sw->whichChoice.getValue());
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
  return SoCallbackAction::CONTINUE;
}

// Shape nodes
SoCallbackAction::Response
SoToVRML2ActionP::soasciitext_cb(void * closure, SoCallbackAction * action, const SoNode * node)
{
  SoVRMLText * text = new SoVRMLText;
  const SoAsciiText * oldtext = (const SoAsciiText*) node;

  text->string = oldtext->string;
  text->length = oldtext->width;
  // FIXME, FontStyle
  
  THISP(closure)->insert_shape(action, text);
  return SoCallbackAction::CONTINUE;
}

SoCallbackAction::Response 
SoToVRML2ActionP::socube_cb(void * closure, SoCallbackAction * action, const SoNode * node)
{
  SoVRMLBox * box = new SoVRMLBox;
  const SoCube * cube = (const SoCube*) node;
  if (box->size.getValue()[0] != cube->width.getValue() ||
      box->size.getValue()[1] != cube->height.getValue() ||
      box->size.getValue()[2] != cube->depth.getValue()) {
    box->size.setValue(cube->width.getValue(), cube->height.getValue(), cube->depth.getValue());
  }
  THISP(closure)->insert_shape(action, box);
  return SoCallbackAction::CONTINUE;
}

SoCallbackAction::Response 
SoToVRML2ActionP::socone_cb(void * closure, SoCallbackAction * action, const SoNode * node)
{
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
  return SoCallbackAction::CONTINUE;
}

SoCallbackAction::Response
SoToVRML2ActionP::socylinder_cb(void * closure, SoCallbackAction * action, const SoNode * node)
{
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
  return SoCallbackAction::CONTINUE;
}

SoCallbackAction::Response
SoToVRML2ActionP::soifs_cb(void * closure, SoCallbackAction * action, const SoNode * node)
{
  const SoIndexedFaceSet * oldifs = (const SoIndexedFaceSet*) node;
  
  if (oldifs->coordIndex.getNum() == 0 ||
      oldifs->coordIndex[0] < 0)
    return SoCallbackAction::CONTINUE;
  
  SoToVRML2ActionP * thisp = (SoToVRML2ActionP*) closure;
  SoGroup * tail = thisp->get_current_tail();
  SoVRMLIndexedFaceSet * ifs = new SoVRMLIndexedFaceSet;

  // Set the values from the current ShapeHints
  ifs->creaseAngle = action->getCreaseAngle();
  ifs->ccw = action->getVertexOrdering() == SoShapeHints::COUNTERCLOCKWISE;
  ifs->solid = SoShapeHintsElement::getShapeType(action->getState()) == SoShapeHintsElement::SOLID;
  ifs->convex = action->getFaceType() == SoShapeHints::CONVEX;

  const SoCoordinateElement * coordElem = SoCoordinateElement::getInstance(action->getState());
  if (coordElem->getNum() > 0) {
    ifs->coord = thisp->get_or_create_coordinate(coordElem->getArrayPtr3(),
                                                 coordElem->getNum());
  }

  const SoNormalElement * normalElem = SoNormalElement::getInstance(action->getState());
  if (coordElem->getNum() > 0) {
    ifs->normal = thisp->get_or_create_normal(normalElem->getArrayPtr(),
                                              normalElem->getNum());
    if (action->getNormalBinding() == SoNormalBinding::PER_FACE_INDEXED ||
        action->getNormalBinding() == SoNormalBinding::PER_FACE) {
      ifs->normalPerVertex = FALSE;
    }
  }

  const SoDiffuseColorElement * colorElem = SoDiffuseColorElement::getInstance(action->getState());
  if (colorElem->getNum() > 0) {
    ifs->color = thisp->get_or_create_color(colorElem->getColorArrayPtr(),
                                            colorElem->getNum());
    if (action->getMaterialBinding() == SoMaterialBinding::PER_FACE_INDEXED ||
        action->getMaterialBinding() == SoMaterialBinding::PER_FACE) {
      ifs->colorPerVertex = FALSE;
    }
  }

  const SoTextureCoordinateElement * texcoordElem =
    SoTextureCoordinateElement::getInstance(action->getState());
  if (texcoordElem->getNum() > 0) {
    ifs->texCoord = thisp->get_or_create_texcoordinate(texcoordElem->getArrayPtr2(),
                                                       texcoordElem->getNum());
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
  return SoCallbackAction::CONTINUE;
}

SoCallbackAction::Response
SoToVRML2ActionP::soils_cb(void * closure, SoCallbackAction * action, const SoNode * node)
{
  SoToVRML2ActionP * thisp = (SoToVRML2ActionP*) closure;
  
  const SoIndexedLineSet * oldils = (const SoIndexedLineSet*) node;

  if (oldils->coordIndex.getNum() == 0 ||
      oldils->coordIndex[0] < 0)
    return SoCallbackAction::CONTINUE;

  SoVRMLIndexedLineSet * ils = new SoVRMLIndexedLineSet;
  SoGroup * tail = thisp->get_current_tail();

  SoVRMLCoordinate * newcoord = NULL;
  const SoCoordinateElement * coordElem = SoCoordinateElement::getInstance(action->getState());
  if (coordElem->getNum() > 0) {
    if (thisp->nodefuse) {
      newcoord = new SoVRMLCoordinate;
    }
    else {
      newcoord = thisp->get_or_create_coordinate(coordElem->getArrayPtr3(),
                                                 coordElem->getNum());
    }
    ils->coord = newcoord;
  }

  const SoDiffuseColorElement * colorElem = SoDiffuseColorElement::getInstance(action->getState());
  if (colorElem->getNum() > 0) {
    ils->color = thisp->get_or_create_color(colorElem->getColorArrayPtr(),
                                            colorElem->getNum());
    if (action->getMaterialBinding() == SoMaterialBinding::PER_FACE_INDEXED ||
        action->getMaterialBinding() == SoMaterialBinding::PER_FACE) {
      ils->colorPerVertex = FALSE;
    }
  }
  
  if (thisp->nodefuse && coordElem->getNum() > 0) {
    SbBSPTree bsp;
    int n = oldils->coordIndex.getNum();
    const int32_t * src = oldils->coordIndex.getValues(0);

    const SbVec3f * c = coordElem->getArrayPtr3();

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
    
  }
  else {
    ils->coordIndex.setValues(0, oldils->coordIndex.getNum(),
                              oldils->coordIndex.getValues(0));
  }
  
  if (oldils->materialIndex.getNum()) {
    ils->colorIndex.setValues(0, oldils->materialIndex.getNum(),
                              oldils->materialIndex.getValues(0));
  }

  THISP(closure)->insert_shape(action, ils);
  return SoCallbackAction::CONTINUE;
}

SoCallbackAction::Response
SoToVRML2ActionP::sopointset_cb(void * closure, SoCallbackAction * action, const SoNode * node)
{
  SoToVRML2ActionP * thisp = (SoToVRML2ActionP*) closure;

  const SoPointSet * oldps = (const SoPointSet*) node;

  SoVRMLPointSet * ps = new SoVRMLPointSet;
  SoGroup * tail = thisp->get_current_tail();

  const SoCoordinateElement * coordElem = SoCoordinateElement::getInstance(action->getState());
  if (coordElem->getNum() > 0) {
    ps->coord = thisp->get_or_create_coordinate(coordElem->getArrayPtr3(),
                                                coordElem->getNum());
  }

  const SoDiffuseColorElement * colorElem = SoDiffuseColorElement::getInstance(action->getState());
  if (colorElem->getNum() > 0) {
    ps->color = thisp->get_or_create_color(colorElem->getColorArrayPtr(),
                                           colorElem->getNum());
  }  

  THISP(closure)->insert_shape(action, ps);
  return SoCallbackAction::CONTINUE;
}

SoCallbackAction::Response
SoToVRML2ActionP::sosphere_cb(void * closure, SoCallbackAction * action, const SoNode * node)
{
  SoVRMLSphere * sphere = new SoVRMLSphere;
  const SoSphere * oldsphere = (const SoSphere*) node;
  if (oldsphere->radius != sphere->radius)
    sphere->radius = oldsphere->radius.getValue();
  THISP(closure)->insert_shape(action, sphere);
  return SoCallbackAction::CONTINUE;
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
  // FIXME: SoDirectionalLight seems to not support this?
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


#endif // HAVE_VRML97
