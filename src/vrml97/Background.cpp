/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2003 by Systems in Motion.  All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  ("GPL") version 2 as published by the Free Software Foundation.
 *  See the file LICENSE.GPL at the root directory of this source
 *  distribution for additional information about the GNU GPL.
 *
 *  For using Coin with software that can not be combined with the GNU
 *  GPL, and for taking advantage of the additional benefits of our
 *  support services, please contact Systems in Motion about acquiring
 *  a Coin Professional Edition License.
 *
 *  See <URL:http://www.coin3d.org> for  more information.
 *
 *  Systems in Motion, Teknobyen, Abels Gate 5, 7030 Trondheim, NORWAY.
 *  <URL:http://www.sim.no>.
 *
\**************************************************************************/

/*!
  \class SoVRMLBackground SoVRMLBackground.h Inventor/VRMLnodes/SoVRMLBackground.h
  \brief The SoVRMLBackground class is used for specifying a viewer panorama.
  \ingroup VRMLnodes

  \WEB3DCOPYRIGHT

  \verbatim
  Background {
    eventIn      SFBool   set_bind
    exposedField MFFloat  groundAngle  []         # [0,pi/2]
    exposedField MFColor  groundColor  []         # [0,1]
    exposedField MFString backUrl      []
    exposedField MFString bottomUrl    []
    exposedField MFString frontUrl     []
    exposedField MFString leftUrl      []
    exposedField MFString rightUrl     []
    exposedField MFString topUrl       []
    exposedField MFFloat  skyAngle     []         # [0,pi]
    exposedField MFColor  skyColor     0 0 0      # [0,1]
    eventOut     SFBool   isBound
  }
  \endverbatim

  The Background node is used to specify a colour backdrop that
  simulates ground and sky, as well as a background texture, or
  panorama, that is placed behind all geometry in the scene and in
  front of the ground and sky. Background nodes are specified in the
  local coordinate system and are affected by the accumulated rotation
  of their ancestors as described below.  Background nodes are
  bindable nodes as described in 4.6.10, Bindable children nodes
  (<http://www.web3d.org/technicalinfo/specifications/vrml97/part1/concepts.html#4.6.10>).
  There exists a Background stack, in which the top-most Background on
  the stack is the currently active Background. To move a Background
  to the top of the stack, a TRUE value is sent to the set_bind
  eventIn.  Once active, the Background is then bound to the browsers
  view. A FALSE value sent to set_bind removes the Background from the
  stack and unbinds it from the browser's view. More detail on the
  bind stack is described in 4.6.10, Bindable children nodes
  (<http://www.web3d.org/technicalinfo/specifications/vrml97/part1/concepts.html#4.6.10>).

  The backdrop is conceptually a partial sphere (the ground) enclosed
  inside of a full sphere (the sky) in the local coordinate system
  with the viewer placed at the centre of the spheres. Both spheres
  have infinite radius and each is painted with concentric circles of
  interpolated colour perpendicular to the local Y-axis of the
  sphere. The Background node is subject to the accumulated rotations
  of its ancestors' transformations. Scaling and translation
  transformations are ignored. The sky sphere is always slightly
  farther away from the viewer than the ground partial sphere causing
  the ground to appear in front of the sky where they overlap.  The
  skyColor field specifies the colour of the sky at various angles on
  the sky sphere. The first value of the skyColor field specifies the
  colour of the sky at 0.0 radians representing the zenith (i.e.,
  straight up from the viewer). The skyAngle field specifies the
  angles from the zenith in which concentric circles of colour
  appear. The zenith of the sphere is implicitly defined to be 0.0
  radians, the natural horizon is at pi/2 radians, and the nadir
  (i.e., straight down from the viewer) is at pi radians. skyAngle is
  restricted to non-decreasing values in the range [0.0, pi]. There
  shall be one more skyColor value than there are skyAngle values. The
  first colour value is the colour at the zenith, which is not
  specified in the skyAngle field.  If the last skyAngle is less than
  pi, then the colour band between the last skyAngle and the nadir is
  clamped to the last skyColor.  The sky colour is linearly
  interpolated between the specified skyColor values.

  The groundColor field specifies the colour of the ground at the
  various angles on the ground partial sphere. The first value of the
  groundColor field specifies the colour of the ground at 0.0 radians
  representing the nadir (i.e., straight down from the user). The
  groundAngle field specifies the angles from the nadir that the
  concentric circles of colour appear. The nadir of the sphere is
  implicitly defined at 0.0 radians. groundAngle is restricted to
  non-decreasing values in the range [0.0, pi/2]. There shall be one
  more groundColor value than there are groundAngle values. The first
  colour value is for the nadir which is not specified in the
  groundAngle field. If the last groundAngle is less than pi/2, the
  region between the last groundAngle and the equator is
  non-existant. The ground colour is linearly interpolated between the
  specified groundColor values.

  The backUrl, bottomUrl, frontUrl, leftUrl, rightUrl, and topUrl
  fields specify a set of images that define a background panorama
  between the ground/sky backdrop and the scene's geometry.  The
  panorama consists of six images, each of which is mapped onto a face
  of an infinitely large cube contained within the backdrop spheres
  and centred in the local coordinate system. The images are applied
  individually to each face of the cube. On the front, back, right,
  and left faces of the cube, when viewed from the origin looking down
  the negative Z-axis with the Y-axis as the view up direction, each
  image is mapped onto the corresponding face with the same
  orientation as if the image were displayed normally in 2D (backUrl
  to back face, frontUrl to front face, leftUrl to left face, and
  rightUrl to right face). On the top face of the cube, when viewed
  from the origin looking along the +Y-axis with the +Z-axis as the
  view up direction, the topUrl image is mapped onto the face with the
  same orientation as if the image were displayed normally in 2D. On
  the bottom face of the box, when viewed from the origin along the
  negative Y-axis with the negative Z-axis as the view up direction,
  the bottomUrl image is mapped onto the face with the same
  orientation as if the image were displayed normally in 2D.

  <center>
  <img src="http://www.web3d.org/technicalinfo/specifications/vrml97/Images/background.gif">
  Figure 6.1
  </center>

  Figure 6.1 illustrates the Background node backdrop and background
  textures.  Alpha values in the panorama images (i.e., two or four
  component images) specify that the panorama is semi-transparent or
  transparent in regions, allowing the groundColor and skyColor to be
  visible.  See 4.6.11, Texture maps, for a general description of
  texture maps.  Often, the bottomUrl and topUrl images will not be
  specified, to allow sky and ground to show. The other four images
  may depict surrounding mountains or other distant scenery. Browsers
  shall support the JPEG (see 2.[JPEG]) and PNG (see 2.[PNG]) image
  file formats, and in addition, may support any other image format
  (e.g., CGM) that can be rendered into a 2D image. Support for the
  GIF (see E.[GIF]) format is recommended (including transparency).
  More detail on the url fields can be found in 4.5, VRML and the
  World Wide Web
  (<http://www.web3d.org/technicalinfo/specifications/vrml97/part1/concepts.html#4.5>).


*/

/*!
  SoMFFloat SoVRMLBackground::groundAngle

  The ground angles where different colors should be used.
*/

/*!
  SoMFColor SoVRMLBackground::groundColor

  The color for each groundAngle.
*/

/*!
  SoMFFloat SoVRMLBackground::skyAngle

  The sky angles where different colors should be used.
*/

/*!
  SoMFColor SoVRMLBackground::skyColor

  The color for each skyAngle.
*/

/*!
  SoMFString SoVRMLBackground::backUrl

  URL for the background image.
*/

/*!
  SoMFString SoVRMLBackground::bottomUrl

  URL for the bottom image.
*/

/*!
  SoMFString SoVRMLBackground::frontUrl

  URL for the front image.
*/

/*!
  SoMFString SoVRMLBackground::leftUrl

  URL for the left image.
*/

/*!
  SoMFString SoVRMLBackground::rightUrl

  URL for the right image.
*/

/*!
  SoMFString SoVRMLBackground::topUrl

  URL for the top image.
*/

/*!
  SoSFBool SoVRMLBackground::set_bind
  An eventIn which is triggered when the node is bound.
*/

/*!
  SoSFBool SoVRMLBackground::isBound
  An eventOut that is sent after the node has been bound/unbound.
*/


#include <Inventor/nodes/SoSubNodeP.h>
#include <Inventor/errors/SoDebugError.h>
#include <Inventor/nodes/SoIndexedTriangleStripSet.h>
#include <Inventor/nodes/SoVertexProperty.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/misc/SoChildList.h>
#include <Inventor/nodes/SoMaterialBinding.h>
#include <Inventor/nodes/SoShapeHints.h>
#include <Inventor/SoDB.h>
#include <Inventor/SoInput.h>
#include <Inventor/nodes/SoIndexedFaceSet.h>
#include <Inventor/nodes/SoScale.h>
#include <Inventor/nodes/SoLightModel.h>
#include <Inventor/sensors/SoFieldSensor.h>
#include <Inventor/VRMLnodes/SoVRMLImageTexture.h>
#include <Inventor/VRMLnodes/SoVRMLMacros.h>
#include <Inventor/VRMLnodes/SoVRMLBackground.h>
#include <Inventor/elements/SoViewVolumeElement.h>
#include <Inventor/elements/SoModelMatrixElement.h>
#include <Inventor/system/gl.h>

#define PRIVATE(p) (p->pimpl)
#define PUBLIC(p) (p->master)

SO_NODE_SOURCE(SoVRMLBackground);

static char scenery_data[] = {
  "#Inventor 2.1 ascii\n\n"
  "  BaseColor { rgb [1 1 1] }\n"
  "  Coordinate3 { point [-1 -1 -1, -1 1 -1, 1 1 -1, 1 -1 -1,   -1 -1 1, -1 1 1, 1 1 1, 1 -1 1]}\n"
  "  TextureCoordinate2 { point [0 0, 1 0, 1 1, 0 1] }\n"
  "  TextureCoordinateBinding { value PER_VERTEX_INDEXED }\n"
};

static void geometrychangeCB(void * data, SoSensor * sensor);
static void vrmltexturechangeCB(void * data, SoSensor * sensor);
static void bindingchangeCB(void * data, SoSensor * sensor);

class SoVRMLBackgroundP {

public:
  SoVRMLBackgroundP(SoVRMLBackground * master) {
    this->master = master;
  };

  SoVRMLBackground * master;

  SoSeparator * rootnode;
  SoChildList * children;

  SoFieldSensor * setbindsensor;
  SoFieldSensor * isboundsensor;
  SoFieldSensor * groundanglesensor;
  SoFieldSensor * groundcolorsensor;
  SoFieldSensor * skyanglesensor;
  SoFieldSensor * skycolorsensor;

  SoFieldSensor * backurlsensor;
  SoFieldSensor * fronturlsensor;
  SoFieldSensor * lefturlsensor;
  SoFieldSensor * righturlsensor;
  SoFieldSensor * bottomurlsensor;
  SoFieldSensor * topurlsensor;

  SoVRMLImageTexture * fronttexture;
  SoVRMLImageTexture * backtexture;
  SoVRMLImageTexture * lefttexture;
  SoVRMLImageTexture * righttexture;
  SoVRMLImageTexture * toptexture;
  SoVRMLImageTexture * bottomtexture;

  SoSeparator * frontface;
  SoSeparator * backface;
  SoSeparator * bottomface;
  SoSeparator * topface;
  SoSeparator * leftface;
  SoSeparator * rightface;

  SbBool geometrybuilt;

  void buildGeometry();
  void modifyCubeFace(SoMFString & urls, SoSeparator * facesep, int * vindices);
  SoSeparator * createCubeFace(SoMFString & urls, SoSeparator * sep, int * vindices);
  void buildIndexList(SoIndexedTriangleStripSet * sphere, int len, int matlength);



};


// Doc in parent
void
SoVRMLBackground::initClass(void) // static
{
  SO_NODE_INTERNAL_INIT_CLASS(SoVRMLBackground, SO_VRML97_NODE_TYPE);
}

/*!
  Constructor.
*/
SoVRMLBackground::SoVRMLBackground(void)
{
  SO_VRMLNODE_INTERNAL_CONSTRUCTOR(SoVRMLBackground);

  SO_VRMLNODE_ADD_EMPTY_EXPOSED_MFIELD(groundColor);
  SO_VRMLNODE_ADD_EMPTY_EXPOSED_MFIELD(skyColor);
  SO_VRMLNODE_ADD_EMPTY_EXPOSED_MFIELD(groundAngle);
  SO_VRMLNODE_ADD_EMPTY_EXPOSED_MFIELD(skyAngle);
  SO_VRMLNODE_ADD_EMPTY_EXPOSED_MFIELD(backUrl);
  SO_VRMLNODE_ADD_EMPTY_EXPOSED_MFIELD(bottomUrl);
  SO_VRMLNODE_ADD_EMPTY_EXPOSED_MFIELD(frontUrl);
  SO_VRMLNODE_ADD_EMPTY_EXPOSED_MFIELD(leftUrl);
  SO_VRMLNODE_ADD_EMPTY_EXPOSED_MFIELD(rightUrl);
  SO_VRMLNODE_ADD_EMPTY_EXPOSED_MFIELD(topUrl);

  SO_VRMLNODE_ADD_EVENT_IN(set_bind);
  SO_VRMLNODE_ADD_EVENT_OUT(isBound);

  PRIVATE(this) = new SoVRMLBackgroundP(this);
  PRIVATE(this)->children = new SoChildList(this);
  
  // Binding sensors 
  PRIVATE(this)->setbindsensor = new SoFieldSensor(bindingchangeCB, PRIVATE(this));
  PRIVATE(this)->isboundsensor = new SoFieldSensor(bindingchangeCB, PRIVATE(this));
 
  PRIVATE(this)->setbindsensor->attach(&this->set_bind);
  PRIVATE(this)->isboundsensor->attach(&this->isBound);

  PRIVATE(this)->setbindsensor->setPriority(5);
  PRIVATE(this)->isboundsensor->setPriority(5);

  // Geometry sensors
  PRIVATE(this)->groundanglesensor = new SoFieldSensor(geometrychangeCB, PRIVATE(this));
  PRIVATE(this)->groundcolorsensor = new SoFieldSensor(geometrychangeCB, PRIVATE(this));
  PRIVATE(this)->skyanglesensor = new SoFieldSensor(geometrychangeCB, PRIVATE(this));
  PRIVATE(this)->skycolorsensor = new SoFieldSensor(geometrychangeCB, PRIVATE(this));

  PRIVATE(this)->groundanglesensor->attach(&this->groundAngle);
  PRIVATE(this)->groundcolorsensor->attach(&this->groundColor);
  PRIVATE(this)->skyanglesensor->attach(&this->skyAngle);
  PRIVATE(this)->skycolorsensor->attach(&this->skyColor);

  PRIVATE(this)->groundanglesensor->setPriority(5); 
  PRIVATE(this)->groundcolorsensor->setPriority(5); 
  PRIVATE(this)->skyanglesensor->setPriority(5);
  PRIVATE(this)->skycolorsensor->setPriority(5);

  // URL/skybox sensors  
  PRIVATE(this)->backurlsensor = new SoFieldSensor(vrmltexturechangeCB, PRIVATE(this));
  PRIVATE(this)->fronturlsensor = new SoFieldSensor(vrmltexturechangeCB, PRIVATE(this));
  PRIVATE(this)->lefturlsensor = new SoFieldSensor(vrmltexturechangeCB, PRIVATE(this));
  PRIVATE(this)->righturlsensor = new SoFieldSensor(vrmltexturechangeCB, PRIVATE(this));
  PRIVATE(this)->bottomurlsensor = new SoFieldSensor(vrmltexturechangeCB, PRIVATE(this));
  PRIVATE(this)->topurlsensor = new SoFieldSensor(vrmltexturechangeCB, PRIVATE(this));

  PRIVATE(this)->backurlsensor->attach(&this->backUrl);
  PRIVATE(this)->fronturlsensor->attach(&this->frontUrl);
  PRIVATE(this)->lefturlsensor->attach(&this->leftUrl);
  PRIVATE(this)->righturlsensor->attach(&this->rightUrl);
  PRIVATE(this)->bottomurlsensor->attach(&this->bottomUrl);
  PRIVATE(this)->topurlsensor->attach(&this->topUrl);

  PRIVATE(this)->backurlsensor->setPriority(5);
  PRIVATE(this)->fronturlsensor->setPriority(5);
  PRIVATE(this)->lefturlsensor->setPriority(5);
  PRIVATE(this)->righturlsensor->setPriority(5);
  PRIVATE(this)->bottomurlsensor->setPriority(5);
  PRIVATE(this)->topurlsensor->setPriority(5);

  PRIVATE(this)->geometrybuilt = FALSE;

}

/*!
  Destructor.
*/
SoVRMLBackground::~SoVRMLBackground()
{
  delete PRIVATE(this)->children;
  delete PRIVATE(this);
}

// Doc in parent
void
SoVRMLBackground::GLRender(SoGLRenderAction * action)
{
  
  SoState * state = action->getState();

  state->push();

  SbViewVolume vv = SoViewVolumeElement::get(state);
  SbVec3f trans  = vv.getProjectionPoint();

  // FIXME: When trans[z] > 10000000, things start to act strange
  // (geometry starts to bounce uncontrolled). This can be observed
  // by zooming out in an examinerviewer. At first the background is
  // still (as it should), then suddenly it starts to move as
  // described. Might be caused by a variabel overflow/wrap somewhere
  // (7Aug2003 handegar)
  SoModelMatrixElement::translateBy(state, (SoNode *) this, trans);
 
  SbBool depthtest = glIsEnabled(GL_DEPTH_TEST);
  glDisable(GL_DEPTH_TEST);

  int numindices;
  const int * indices;
  if (action->getPathCode(numindices, indices) == SoAction::IN_PATH) {
    PRIVATE(this)->children->traverseInPath(action, numindices, indices);
  }
  else {
    PRIVATE(this)->children->traverse((SoAction *) action);
  }

  if (depthtest)
    glEnable(GL_DEPTH_TEST);

  state->pop();

}


// FIXME: This method is disabled as there is no virtual 'readInstance'
// method in Coin-2 yet (there is one in Coin-1). (11Aug2003 handegar)
/*
// Documented in superclass. Overridden to check if texture file (if
// any) can be found and loaded.
SbBool
SoVRMLBackground::readInstance(SoInput * in, unsigned short flags)
{

  PRIVATE(this)->groundanglesensor->detach();
  PRIVATE(this)->groundcolorsensor->detach();
  PRIVATE(this)->skyanglesensor->detach();
  PRIVATE(this)->skycolorsensor->detach();
  
  PRIVATE(this)->backurlsensor->detach();
  PRIVATE(this)->fronturlsensor->detach();
  PRIVATE(this)->lefturlsensor->detach();
  PRIVATE(this)->righturlsensor->detach();
  PRIVATE(this)->bottomurlsensor->detach();
  PRIVATE(this)->topurlsensor->detach();

  PRIVATE(this)->setbindsensor->detach();
  PRIVATE(this)->isboundsensor->detach();

  SbBool readok = inherited::readInstance(in, flags);
  PRIVATE(this)->buildGeometry(); 

  PRIVATE(this)->groundanglesensor->attach(&this->groundAngle);
  PRIVATE(this)->groundcolorsensor->attach(&this->groundColor);
  PRIVATE(this)->skyanglesensor->attach(&this->skyAngle);
  PRIVATE(this)->skycolorsensor->attach(&this->skyColor);
 
  PRIVATE(this)->backurlsensor->attach(&this->backUrl);
  PRIVATE(this)->fronturlsensor->attach(&this->frontUrl);
  PRIVATE(this)->lefturlsensor->attach(&this->leftUrl);
  PRIVATE(this)->righturlsensor->attach(&this->rightUrl);
  PRIVATE(this)->bottomurlsensor->attach(&this->bottomUrl);
  PRIVATE(this)->topurlsensor->attach(&this->topUrl);

  PRIVATE(this)->setbindsensor->attach(&this->set_bind);
  PRIVATE(this)->isboundsensor->attach(&this->isBound);

  return readok;
}
*/

void
SoVRMLBackgroundP::buildGeometry()
{

  float sphereradius = 1.5;
  SbList <float> angles;
  
  //
  // Initial scenegraph
  //

  this->rootnode = new SoSeparator;
  this->rootnode->ref();

  SoLightModel * lightmodel = new SoLightModel;
  lightmodel->model.setValue(SoLightModel::BASE_COLOR);

  this->rootnode->addChild(lightmodel);

  
  //
  // Sky sphere
  //

  if(PUBLIC(this)->skyAngle.getNum() > 0){

    angles.append(0);
    float angle = 0;
    for (int k=0;k<PUBLIC(this)->skyAngle.getNum();++k) { 
      if (angle > PUBLIC(this)->skyAngle[k]) {
        SoDebugError::postWarning("buildGeometry","skyAngle array must be non-decreasing.");
        continue;
      }
      angle = PUBLIC(this)->skyAngle[k];
      if (angle > M_PI) {
        SoDebugError::postWarning("buildGeometry","skyAngle > PI not allowed.");
        angle = M_PI;
      } else if (angle < 0) {
        SoDebugError::postWarning("buildGeometry","skyAngle < 0 not allowed.");
        angle = 0;
      } 
      angles.append(angle);
    }
    if (angle != M_PI)
      angles.append(M_PI);
    
    int len = angles.getLength();
    
    SbVec3f * skyvertexarray = new SbVec3f[len * len];    
    SoIndexedTriangleStripSet * sky = new SoIndexedTriangleStripSet;
    SoVertexProperty * skyproperties = new SoVertexProperty;    
    skyproperties->normalBinding.setValue(SoVertexProperty::PER_VERTEX_INDEXED);

   
    // Calculate vertices and normals
    double x, y, z;
    int counter = 0;
    for (int i=0;i<len;++i) {
      for (int j=0;j<len;++j) {
        x = sphereradius * cos(i * ((2 * M_PI) / len)) * sin(angles[j]);
        y = sphereradius * cos(angles[j]);
        z = sphereradius * sin(i * ((2 * M_PI) / len)) * sin(angles[j]);
        skyvertexarray[counter++] = SbVec3f((float) x, (float) y, (float) z);
      }
    }
    for (i=0;i<len*len;++i) {
      skyproperties->vertex.set1Value(i, skyvertexarray[i]);
      SbVec3f normal = -skyvertexarray[i];
      normal.normalize();
      skyproperties->normal.set1Value(i, normal);
    }
    delete [] skyvertexarray;
    sky->vertexProperty.setValue(skyproperties);
    

    // Setup color arrays
    if (PUBLIC(this)->skyColor.getNum() > 0) {
      for (int i=0;i<PUBLIC(this)->skyColor.getNum();++i)
        skyproperties->orderedRGBA.set1Value(i, PUBLIC(this)->skyColor[i].getPackedValue(0));
      skyproperties->materialBinding = SoMaterialBinding::PER_VERTEX_INDEXED;
    } else {
      SoDebugError::postWarning("buildGeometry","No colors specified for sky");
      return;
    }


    buildIndexList(sky, len, PUBLIC(this)->skyColor.getNum());


    SoShapeHints * shapehintssky = new SoShapeHints;
    shapehintssky->vertexOrdering = SoShapeHints::COUNTERCLOCKWISE;
    shapehintssky->shapeType = SoShapeHints::SOLID;
    shapehintssky->faceType = SoShapeHints::CONVEX;

    this->rootnode->addChild(shapehintssky);
    this->rootnode->addChild(sky);  

  }
  
  //
  // Ground sphere
  //

  if (PUBLIC(this)->groundAngle.getNum() > 0) {

    sphereradius = sphereradius * 0.9;
    angles.truncate(0);
    angles.append(0);
    float angle = 0;
    for (int k=0;k<PUBLIC(this)->groundAngle.getNum();++k) { 
      if (angle > PUBLIC(this)->groundAngle[k]) {
        SoDebugError::postWarning("buildGeometry","groundAngle array must be non-decreasing.");
        continue;
      }
      angle = PUBLIC(this)->groundAngle[k];
      if (angle > M_PI/2) {
        SoDebugError::postWarning("buildGeometry","groundAngle > PI/2 not allowed.");
        angle = M_PI / 2;
      } else if (angle < 0) {
        SoDebugError::postWarning("buildGeometry","groundAngle < 0 not allowed.");
        angle = 0;
      } 
      angles.append(angle);
    }
    if (angle != M_PI/2)
      angles.append(M_PI / 2);

    int len = angles.getLength();

    SbVec3f * groundvertexarray = new SbVec3f[len * len];
    SoIndexedTriangleStripSet * ground = new SoIndexedTriangleStripSet;
    SoVertexProperty * groundproperties = new SoVertexProperty;
    groundproperties->normalBinding.setValue(SoVertexProperty::PER_VERTEX_INDEXED);

    // Calculate vertices and normals
    int counter = 0;
    double x,y,z;
    for (int i=0;i<len;++i) {
      for (int j=0;j<len;++j) {
        x = sphereradius * cos(i * ((2 * M_PI) / len)) * sin(angles[j]);
        y = -sphereradius * cos(angles[j]);
        z = sphereradius * sin(i * ((2 * M_PI) / len)) * sin(angles[j]);
        groundvertexarray[counter++] = SbVec3f((float) x, (float) y, (float) z);
      }
    }
    for (i=0;i<len*len;++i) {
      groundproperties->vertex.set1Value(i, groundvertexarray[i]);
      SbVec3f normal = -groundvertexarray[i];
      normal.normalize();
      groundproperties->normal.set1Value(i, normal);
    }
    delete [] groundvertexarray;
    ground->vertexProperty.setValue(groundproperties);


    // Setup color arrays
    if (PUBLIC(this)->groundColor.getNum() > 0) {
      for (int i=0;i<PUBLIC(this)->groundColor.getNum();++i)
        groundproperties->orderedRGBA.set1Value(i, PUBLIC(this)->groundColor[i].getPackedValue(0));
      groundproperties->materialBinding = SoMaterialBinding::PER_VERTEX_INDEXED;
    } else {
      SoDebugError::postWarning("buildGeometry","No colors specified for ground");
      return;
    }

    // Build vertex and normal indices
    buildIndexList(ground, len, PUBLIC(this)->groundColor.getNum());
      
    SoShapeHints * shapehintsground = new SoShapeHints;
    shapehintsground->vertexOrdering = SoShapeHints::CLOCKWISE;
    shapehintsground->shapeType = SoShapeHints::SOLID;
    shapehintsground->faceType = SoShapeHints::CONVEX;

    this->rootnode->addChild(shapehintsground);
    this->rootnode->addChild(ground);

  }


  //
  // Scenery cube
  //
  SoDB::init();
  SoInput in;
  in.setBuffer(scenery_data, sizeof(scenery_data));
  SoSeparator * cubedata = SoDB::readAll(&in);

  SoScale * scale = new SoScale;
  SbVec3f factor(2, 2, 2);
  scale->scaleFactor.setValue(factor);
  this->rootnode->addChild(scale);

  this->rootnode->addChild(cubedata);
  
  int tindices[] = {1, 2, 3, 0, -1};
  this->frontface = NULL;
  this->backface = NULL;
  this->leftface = NULL;
  this->rightface = NULL;
  this->topface = NULL;
  this->bottomface = NULL;

  SoVRMLImageTexture * tex;

  if (PUBLIC(this)->backUrl.getNum() != 0) {     
    int vindices[] = {3, 2, 1, 0, -1};
    SoSeparator * sep = this->createCubeFace(PUBLIC(this)->backUrl,this->backface, vindices);    
    cubedata->addChild(sep);
  }
  
  if (PUBLIC(this)->leftUrl.getNum() != 0) {
    int vindices[] = {0, 1, 5, 4, -1};
    SoSeparator * sep = this->createCubeFace(PUBLIC(this)->leftUrl,this->leftface, vindices);    
    cubedata->addChild(sep);
  }
  
  if (PUBLIC(this)->frontUrl.getNum() != 0) {
    int vindices[] = {4, 5, 6, 7, -1};
    SoSeparator * sep = this->createCubeFace(PUBLIC(this)->frontUrl,this->frontface, vindices);    
    cubedata->addChild(sep);
  }

  if (PUBLIC(this)->rightUrl.getNum() != 0) {
    int vindices[] = {7, 6, 2, 3, -1};
    SoSeparator * sep = this->createCubeFace(PUBLIC(this)->rightUrl,this->rightface, vindices);    
    cubedata->addChild(sep);
  }

  if (PUBLIC(this)->bottomUrl.getNum() != 0) {
    int vindices[] = {7, 3, 0, 4, -1};
    SoSeparator * sep = this->createCubeFace(PUBLIC(this)->bottomUrl,this->bottomface, vindices);    
    cubedata->addChild(sep);
  }

  if (PUBLIC(this)->topUrl.getNum() != 0) {
    int vindices[] = {2, 6, 5, 1, -1};
    SoSeparator * sep = this->createCubeFace(PUBLIC(this)->topUrl,this->topface, vindices);    
    cubedata->addChild(sep);
  }
  
  this->children->append(rootnode);
  angles.truncate(0);
 
  this->geometrybuilt = TRUE;
 
}


void
SoVRMLBackgroundP::buildIndexList(SoIndexedTriangleStripSet * sphere, int len, int matlength)
{

  // Build vertex and normal indices for triangle strips
  int matindex = 0;
  int counter = 0;
  for (int i=0;i<len - 1;++i) {
    for (int j=0;j<len;++j) {
      
      sphere->materialIndex.set1Value(counter,matindex);
      sphere->normalIndex.set1Value(counter, ((i + 1) * len) + j);
      sphere->coordIndex.set1Value(counter++, ((i + 1) * len) + j);
      
      sphere->materialIndex.set1Value(counter, matindex);
      sphere->normalIndex.set1Value(counter, (i * len) + j);
      sphere->coordIndex.set1Value(counter++, (i * len) + j);
      
      ++matindex;
      if (matindex >= matlength) 
        matindex = matlength - 1;
    }
    sphere->materialIndex.set1Value(counter, -1);
    sphere->coordIndex.set1Value(counter, -1);
    sphere->normalIndex.set1Value(counter++, -1);
    matindex = 0;
  }
  
  //matindex = 0;
  i = len - 1;
  for (int j=0;j<len;++j) {
    
    sphere->materialIndex.set1Value(counter, matindex);
    sphere->normalIndex.set1Value(counter, j);
    sphere->coordIndex.set1Value(counter++, j);
    
    sphere->materialIndex.set1Value(counter, matindex);
    sphere->normalIndex.set1Value(counter, (i * len) + j);
    sphere->coordIndex.set1Value(counter++, (i * len) + j);
    
    ++matindex;
    if (matindex >= matlength)
      matindex = matlength - 1;
  }
  
  sphere->materialIndex.set1Value(counter, -1);
  sphere->coordIndex.set1Value(counter, -1);
  sphere->normalIndex.set1Value(counter++, -1);
  
}



SoSeparator * 
SoVRMLBackgroundP::createCubeFace(SoMFString & urls, SoSeparator * sep, int * vindices)
{

  int tindices[] = {1, 2, 3, 0, -1};
  sep = new SoSeparator;
  sep->ref();
  SoVRMLImageTexture * tex = new SoVRMLImageTexture;
  tex->url = urls;
  tex->repeatS.setValue(FALSE);
  tex->repeatT.setValue(FALSE);
  SoIndexedFaceSet * faceset = new SoIndexedFaceSet;
  faceset->coordIndex.setValues(0, 5, vindices);
  faceset->textureCoordIndex.setValues(0, 5, tindices);
  sep->addChild(tex);
  sep->addChild(faceset);
  
  return sep;

}

void
SoVRMLBackgroundP::modifyCubeFace(SoMFString & urls, SoSeparator * sep, int * vindices)
{

  SoVRMLImageTexture * tex;

  if (urls.getNum() == 0) {
    if (sep != NULL) {
      sep->unref();
      sep = NULL;
    }
    return;
  }
  else if (sep == NULL) { 
    sep = new SoSeparator;
    sep->ref();

    tex = new SoVRMLImageTexture;
    tex->ref();
    tex->repeatS.setValue(FALSE);
    tex->repeatT.setValue(FALSE);
    int tindices[] = {1, 2, 3, 0, -1};
    SoIndexedFaceSet * faceset = new SoIndexedFaceSet;
    faceset->textureCoordIndex.setValues(0, 5, tindices);

    faceset->coordIndex.setValues(0, 5, vindices);
    sep->addChild(tex);
    sep->addChild(faceset);
  }
  else {
    tex = (SoVRMLImageTexture *) sep->getChild(0);
  }
  tex->url = urls;

}


void
vrmltexturechangeCB(void * data, SoSensor * sensor)
{

  SoVRMLBackgroundP * pimpl = (SoVRMLBackgroundP *) data;

  if(!pimpl->geometrybuilt)
    pimpl->buildGeometry();


  SoVRMLImageTexture * tex = new SoVRMLImageTexture;
  tex->ref();
  tex->repeatS.setValue(FALSE);
  tex->repeatT.setValue(FALSE);
  int tindices[] = {1, 2, 3, 0, -1};
  SoIndexedFaceSet * faceset = new SoIndexedFaceSet;
  faceset->textureCoordIndex.setValues(0, 5, tindices);
  
  if (sensor == pimpl->fronturlsensor) {
    int vindices[] = {4, 5, 6, 7, -1};
    pimpl->modifyCubeFace(pimpl->master->frontUrl, pimpl->frontface, vindices);
  }

  else if (sensor == pimpl->backurlsensor) {
    int vindices[] = {3, 2, 1, 0, -1};
    pimpl->modifyCubeFace(pimpl->master->backUrl, pimpl->backface, vindices);
  }
  
  else if (sensor == pimpl->lefturlsensor) {
    int vindices[] = {0, 1, 5, 4, -1};
    pimpl->modifyCubeFace(pimpl->master->leftUrl, pimpl->leftface, vindices);
  }

  else if (sensor == pimpl->righturlsensor) {
    int vindices[] = {7, 6, 2, 3, -1};
    pimpl->modifyCubeFace(pimpl->master->rightUrl, pimpl->rightface, vindices);
  }

  else if (sensor == pimpl->topurlsensor) {
    int vindices[] = {2, 6, 5, 1, -1};
    pimpl->modifyCubeFace(pimpl->master->topUrl, pimpl->topface, vindices);
  }

  else if (sensor == pimpl->bottomurlsensor) {
    int vindices[] = {7, 3, 0, 4, -1};
    pimpl->modifyCubeFace(pimpl->master->bottomUrl, pimpl->bottomface, vindices);
  }
  

}

void
geometrychangeCB(void * data, SoSensor * sensor)
{
  SoVRMLBackgroundP * pimpl = (SoVRMLBackgroundP *) data;

  if(!pimpl->geometrybuilt)
    pimpl->buildGeometry();

  pimpl->rootnode->removeAllChildren(); // Remove everything incase this was called earlier
  pimpl->rootnode->unref();

  pimpl->buildGeometry();

}

void
bindingchangeCB(void * data, SoSensor * sensor)
{
  SoVRMLBackgroundP * pimpl = (SoVRMLBackgroundP *) data;

  SoFieldSensor * setbindsensor;
  SoFieldSensor * isboundsensor;

  // FIXME: Support for 'set_bind' and 'isBound' must be implemented,
  // but first a Coin viewer must support this kind of special node
  // treatment (goes for Background, Fog, NavigationInfo and Viewport
  // vrml-nodes) (11Aug2003 handegar)

  if (sensor == pimpl->setbindsensor) {
    SoDebugError::postWarning("bindingchangeCB", "'set_bind' event not implemented yet");
  }
  else if (sensor == pimpl->isboundsensor) {
    SoDebugError::postWarning("bindingchangeCB", "'isBound' event not implemented yet");
  }

}

