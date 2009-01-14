/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2009 by Kongsberg SIM.  All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  ("GPL") version 2 as published by the Free Software Foundation.
 *  See the file LICENSE.GPL at the root directory of this source
 *  distribution for additional information about the GNU GPL.
 *
 *  For using Coin with software that can not be combined with the GNU
 *  GPL, and for taking advantage of the additional benefits of our
 *  support services, please contact Kongsberg SIM about acquiring
 *  a Coin Professional Edition License.
 *
 *  See http://www.coin3d.org/ for more information.
 *
 *  Kongsberg SIM, Postboks 1283, Pirsenteret, 7462 Trondheim, NORWAY.
 *  http://www.sim.no/  sales@sim.no  coin-support@coin3d.org
 *
\**************************************************************************/

/*!
  \class SoVertexProperty SoVertexProperty.h Inventor/nodes/SoVertexProperty.h
  \brief The SoVertexProperty class collects the functionality of various appearance nodes.
  \ingroup nodes

  Instead of reading data from the current state stack of the
  scenegraph traversal, nodes inheriting SoVertexShape can be set up
  with an SoVertexProperty node in the SoVertexShape::vertexProperty
  field. Coordinates, normals, texture coordinates and material /
  color information will then be fetched from the vertexshape's
  SoVertexProperty node instead of from the state stack.

  The SoVertexProperty node provides fields for duplicating the
  functionality of all these other Inventor node types: SoCoordinate3,
  SoTextureCoordinate2, SoTextureCoordinate3, SoNormal, SoPackedColor,
  SoMaterialBinding and SoNormalBinding.


  The SoVertexProperty node was introduced fairly late in the design
  of the Inventor API by SGI. The idea behind it was to provide a
  means to specify the necessary data for vertexshape-derived nodes
  which would be more efficient to work with than fetching the data
  from the traversal state stack.

  In practice, the effect is not at all very noticable. Since the use
  of SoVertexProperty nodes in the SoVertexShape::vertexProperty field
  somewhat breaks with the basic design of the Open Inventor API (the
  SoVertexProperty data is not pushed to the traversal state stack),
  you might be better off design-wise by using the usual mechanisms,
  ie by setting up the individual nodes SoVertexProperty "collects".

  One of the drawbacks will for instance be that it's not possible to
  share \e parts of the SoVertexProperty node between several shapes,
  which is something that can easily be done when setting up
  individual state-changing nodes in the scenegraph.

  <b>FILE FORMAT/DEFAULTS:</b>
  \code
    VertexProperty {
        vertex [  ]
        normal [  ]
        texCoord [  ]
        orderedRGBA [  ]
        texCoord3 [  ]
        normalBinding PER_VERTEX_INDEXED
        materialBinding OVERALL
    }
  \endcode

  \since Inventor 2.1
  \sa SoVertexShape
  \sa SoCoordinate3, SoTextureCoordinate2, SoTextureCoordinate3, SoNormal
  \sa SoPackedColor
  \sa SoMaterialBinding, SoNormalBinding
*/
// FIXME: should have a usage-example in the class-doc. 20020109 mortene.

// FIXME: this class is really not optimally supported in Coin. Each
// vertex shape node should support this node with internal handling
// to harvest the expected efficiency gains.  Right now we just push
// its values on the stack. ????-??-?? pederb.

#include <Inventor/nodes/SoVertexProperty.h>

#include <Inventor/actions/SoCallbackAction.h>
#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/actions/SoGetBoundingBoxAction.h>
#include <Inventor/actions/SoGetPrimitiveCountAction.h>
#include <Inventor/actions/SoPickAction.h>
#include <Inventor/elements/SoLazyElement.h>
#include <Inventor/elements/SoGLCoordinateElement.h>
#include <Inventor/elements/SoGLNormalElement.h>
#include <Inventor/elements/SoGLTexture3EnabledElement.h>
#include <Inventor/elements/SoGLTextureCoordinateElement.h>
#include <Inventor/elements/SoMaterialBindingElement.h>
#include <Inventor/elements/SoNormalBindingElement.h>
#include <Inventor/elements/SoOverrideElement.h>
#include <Inventor/elements/SoShapeStyleElement.h>
#include <Inventor/elements/SoGLVBOElement.h>

#include "nodes/SoSubNodeP.h"
#include "misc/SoVBO.h"

/*!
  \enum SoVertexProperty::Binding

  The binding types available for our SoVertexProperty::normalBinding
  and SoVertexProperty::materialBinding fields.

  For a detailed explanation of each of the enumeration value binding
  types, see the documentation of the SoMaterialBinding node.
*/


/*!
  \var SoMFVec3f SoVertexProperty::vertex

  This field sets up vertex coordinates in the same manner as
  SoCoordinate3::point.

  By default the field contains no coordinates.

  \sa SoCoordinate3
*/
/*!
  \var SoMFVec2f SoVertexProperty::texCoord

  Same functionality as SoTextureCoordinate2::point.  By default the
  field contains no coordinates.

  \sa SoTextureCoordinate2
*/

// FIXME: this field was added between TGS Inventor 2.5 and 2.6, and
// between Coin 1.0 and Coin 2.0. This means it must get special
// handling when exporting .iv-files, with regard to what header we
// can put on the output. See also item #003 in the Coin/docs/todo.txt
// file. 20030519 mortene.
/*!
  \var SoMFVec3f SoVertexProperty::texCoord3

  Same functionality as SoTextureCoordinate3::point.  By default the
  field contains no coordinates.

  \sa SoTextureCoordinate3
  \since Coin 2.0
  \since TGS Inventor 2.6
*/

/*!
  \var SoMFVec3f SoVertexProperty::normal

  This field defines a set of normal vectors in the same manner as
  SoNormal::vector.  By default the field contains no vectors.

  \sa SoNormal
*/
/*!
  \var SoSFEnum SoVertexProperty::normalBinding

  Defines how to bind the normals specified in the
  SoVertexProperty::normal set to the parts of the "owner" shape.
  Must be one of the values in the SoVertexProperty::Binding enum.

  Default value of the field is SoVertexProperty::PER_VERTEX_INDEXED.

  \sa SoNormalBinding
*/
/*!
  \var SoMFUInt32 SoVertexProperty::orderedRGBA

  A set of "packed" 32-bit diffusecolor plus transparency
  values. Works in the same manner as the SoPackedColor::orderedRGBA
  field.

  By default the field has no data.

  \sa SoPackedColor
*/
/*!
  \var SoSFEnum SoVertexProperty::materialBinding

  Defines how to bind the colorvalues specified in the
  SoVertexProperty::orderedRGBA set to the parts of the "owner" shape.
  Must be one of the values in the SoVertexProperty::Binding enum.

  Default value of the field is SoVertexProperty::OVERALL.

  \sa SoMaterialBinding
*/

class SoVertexPropertyP {
 public:
  SoVertexPropertyP(void) 
    : vertexvbo(NULL),
      normalvbo(NULL),
      texcoordvbo(NULL),
      colorvbo(NULL)
  {
    this->checktransparent = FALSE;
    this->transparent = FALSE;
  }
  ~SoVertexPropertyP() {
    delete this->vertexvbo;
    delete this->normalvbo;
    delete this->texcoordvbo;
    delete this->colorvbo;
  }
  
  SoVertexProperty * master;
  SbBool transparent;
  SbBool checktransparent;

  SoVBO * vertexvbo;
  SoVBO * normalvbo;
  SoVBO * texcoordvbo;
  SoVBO * colorvbo;

};

#define PRIVATE(obj) obj->pimpl

SO_NODE_SOURCE(SoVertexProperty);

/*!
  Constructor.
*/
SoVertexProperty::SoVertexProperty(void)
{
  PRIVATE(this) = new SoVertexPropertyP;

  SO_NODE_INTERNAL_CONSTRUCTOR(SoVertexProperty);

  SO_NODE_ADD_FIELD(vertex, (0));
  SO_NODE_ADD_FIELD(normal, (0));
  SO_NODE_ADD_FIELD(texCoord, (0));
  SO_NODE_ADD_FIELD(orderedRGBA, (0));
  // FIXME: this field was added in TGS Inventor 2.6 and Coin
  // 2.0. This should have repercussions for file format
  // compatibility. 20030227 mortene.
  SO_NODE_ADD_FIELD(texCoord3, (0));

  // Make multivalue fields empty.
  this->vertex.setNum(0);
  this->texCoord.setNum(0);
  this->texCoord3.setNum(0);
  this->normal.setNum(0);
  this->orderedRGBA.setNum(0);

  // So they are not written in their default state on SoWriteAction
  // traversal.
  this->vertex.setDefault(TRUE);
  this->texCoord.setDefault(TRUE);
  this->texCoord3.setDefault(TRUE);
  this->normal.setDefault(TRUE);
  this->orderedRGBA.setDefault(TRUE);

  SO_NODE_ADD_FIELD(normalBinding, (SoVertexProperty::PER_VERTEX_INDEXED));
  SO_NODE_ADD_FIELD(materialBinding, (SoVertexProperty::OVERALL));

  SO_NODE_DEFINE_ENUM_VALUE(Binding, OVERALL);
  SO_NODE_DEFINE_ENUM_VALUE(Binding, PER_PART);
  SO_NODE_DEFINE_ENUM_VALUE(Binding, PER_PART_INDEXED);
  SO_NODE_DEFINE_ENUM_VALUE(Binding, PER_FACE);
  SO_NODE_DEFINE_ENUM_VALUE(Binding, PER_FACE_INDEXED);
  SO_NODE_DEFINE_ENUM_VALUE(Binding, PER_VERTEX);
  SO_NODE_DEFINE_ENUM_VALUE(Binding, PER_VERTEX_INDEXED);

  SO_NODE_SET_SF_ENUM_TYPE(normalBinding, Binding);
  SO_NODE_SET_SF_ENUM_TYPE(materialBinding, Binding);

}

/*!
  Destructor.
*/
SoVertexProperty::~SoVertexProperty()
{
  delete PRIVATE(this);
}

// Documented in superclass.
void
SoVertexProperty::initClass(void)
{
  SO_NODE_INTERNAL_INIT_CLASS(SoVertexProperty, SO_FROM_INVENTOR_2_1);

  SO_ENABLE(SoGetBoundingBoxAction, SoCoordinateElement);

  SO_ENABLE(SoGLRenderAction, SoGLCoordinateElement);
  SO_ENABLE(SoGLRenderAction, SoMaterialBindingElement);
  SO_ENABLE(SoGLRenderAction, SoNormalBindingElement);
  SO_ENABLE(SoGLRenderAction, SoGLNormalElement);
  SO_ENABLE(SoGLRenderAction, SoGLTextureCoordinateElement);

  SO_ENABLE(SoPickAction, SoCoordinateElement);
  SO_ENABLE(SoPickAction, SoMaterialBindingElement);
  SO_ENABLE(SoPickAction, SoNormalBindingElement);
  SO_ENABLE(SoPickAction, SoNormalElement);
  SO_ENABLE(SoPickAction, SoTextureCoordinateElement);

  SO_ENABLE(SoCallbackAction, SoCoordinateElement);
  SO_ENABLE(SoCallbackAction, SoMaterialBindingElement);
  SO_ENABLE(SoCallbackAction, SoNormalBindingElement);
  SO_ENABLE(SoCallbackAction, SoNormalElement);
  SO_ENABLE(SoCallbackAction, SoTextureCoordinateElement);

  SO_ENABLE(SoGetPrimitiveCountAction, SoCoordinateElement);
  SO_ENABLE(SoGetPrimitiveCountAction, SoMaterialBindingElement);
  SO_ENABLE(SoGetPrimitiveCountAction, SoNormalBindingElement);
  SO_ENABLE(SoGetPrimitiveCountAction, SoNormalElement);
  SO_ENABLE(SoGetPrimitiveCountAction, SoTextureCoordinateElement);
}

// Documented in superclass.
void
SoVertexProperty::getBoundingBox(SoGetBoundingBoxAction * action)
{
  if (vertex.getNum() > 0) {
    SoCoordinateElement::set3(action->getState(), this,
                              vertex.getNum(), vertex.getValues(0));
  }
}

// Documented in superclass.
void
SoVertexProperty::GLRender(SoGLRenderAction * action)
{
  SoVertexProperty::doAction(action);
}

// Documented in superclass.
void
SoVertexProperty::doAction(SoAction *action)
{
  SoState * state = action->getState();

  uint32_t overrideflags = SoOverrideElement::getFlags(state);
#define TEST_OVERRIDE(bit) ((SoOverrideElement::bit & overrideflags) != 0)

  SbBool glrender = action->isOfType(SoGLRenderAction::getClassTypeId());

  if (PRIVATE(this)->checktransparent) {
    PRIVATE(this)->checktransparent = FALSE;
    PRIVATE(this)->transparent = FALSE;
    int n = this->orderedRGBA.getNum();
    for (int i = 0; i < n; i++) {
      if ((this->orderedRGBA[i] & 0xff) != 0xff) {
        PRIVATE(this)->transparent = TRUE;
        break;
      }
    }
  }
  if (glrender) SoBase::staticDataLock();
  int num = this->vertex.getNum();
  const int numvertex = num;
  const SbBool shouldcreatevbo = glrender ? SoGLVBOElement::shouldCreateVBO(state, numvertex) : FALSE;
  
  if (num > 0) {    
    SoCoordinateElement::set3(state, this, num,
                              this->vertex.getValues(0));

    SbBool setvbo = FALSE;
    if (glrender) {
      if (shouldcreatevbo) {
        SbBool dirty = FALSE;
        setvbo = TRUE;
        if (PRIVATE(this)->vertexvbo == NULL) {
          PRIVATE(this)->vertexvbo = new SoVBO(GL_ARRAY_BUFFER, GL_STATIC_DRAW); 
          dirty =  TRUE;
        }
        else if (PRIVATE(this)->vertexvbo->getBufferDataId() != this->getNodeId()) {
          dirty = TRUE;
        }
        if (dirty) {
          PRIVATE(this)->vertexvbo->setBufferData(this->vertex.getValues(0),
                                                  num*sizeof(SbVec3f),
                                                  this->getNodeId());
        }
      }
      else if (PRIVATE(this)->vertexvbo && PRIVATE(this)->vertexvbo->getBufferDataId()) {
        // clear buffers to deallocate VBO memory
        PRIVATE(this)->vertexvbo->setBufferData(NULL, 0, 0);
      }
      if (setvbo) {
        SoGLVBOElement::setVertexVBO(state, PRIVATE(this)->vertexvbo);
      }
    }
  }
  num = this->texCoord3.getNum();
  if (num > 0) {
    if (glrender) {
      if (SoGLTexture3EnabledElement::get(state)) {
        // it's important to call this _before_ setting the coordinates
        // on the state.
        SoGLTextureCoordinateElement::setTexGen(state,
                                                this, NULL);
      }
    }
    SoTextureCoordinateElement::set3(state, this, num,
                                     this->texCoord3.getValues(0));
    if (glrender) {
      SbBool setvbo = FALSE;
      if ((num == numvertex) && shouldcreatevbo) {
        SbBool dirty = FALSE;
        setvbo = TRUE;
        if (PRIVATE(this)->texcoordvbo == NULL) {
          PRIVATE(this)->texcoordvbo = new SoVBO(GL_ARRAY_BUFFER, GL_STATIC_DRAW); 
          dirty =  TRUE;
        }
        else if (PRIVATE(this)->texcoordvbo->getBufferDataId() != this->getNodeId()) {
          dirty = TRUE;
        }
        if (dirty) {
          PRIVATE(this)->texcoordvbo->setBufferData(this->texCoord3.getValues(0),
                                                    num*sizeof(SbVec3f),
                                                    this->getNodeId());
        }
      }
      else if (PRIVATE(this)->texcoordvbo && PRIVATE(this)->texcoordvbo->getBufferDataId()) {
        // clear buffers to deallocate VBO memory
        PRIVATE(this)->texcoordvbo->setBufferData(NULL, 0, 0);
      }
      if (setvbo) {
        SoGLVBOElement::setTexCoordVBO(state, 0, PRIVATE(this)->texcoordvbo);
      }
    }
  }
  else {
    num = this->texCoord.getNum();
    if (num > 0) {
      if (glrender) {
        // it's important to call this _before_ setting the coordinates
        // on the state.
        SoGLTextureCoordinateElement::setTexGen(state,
                                                this, NULL);
      }
      SoTextureCoordinateElement::set2(state, this, num,
                                       this->texCoord.getValues(0));

      if (glrender) {
        SbBool setvbo = FALSE;
        if ((num == numvertex) && shouldcreatevbo) {
          SbBool dirty = FALSE;
          setvbo = TRUE;
          if (PRIVATE(this)->texcoordvbo == NULL) {
            PRIVATE(this)->texcoordvbo = new SoVBO(GL_ARRAY_BUFFER, GL_STATIC_DRAW); 
            dirty =  TRUE;
          }
          else if (PRIVATE(this)->texcoordvbo->getBufferDataId() != this->getNodeId()) {
            dirty = TRUE;
          }
          if (dirty) {
            PRIVATE(this)->texcoordvbo->setBufferData(this->texCoord.getValues(0),
                                                      num*sizeof(SbVec2f),
                                                    this->getNodeId());
          }
        }
        else if (PRIVATE(this)->texcoordvbo && PRIVATE(this)->texcoordvbo->getBufferDataId()) {
          // clear buffers to deallocate VBO memory
          PRIVATE(this)->texcoordvbo->setBufferData(NULL, 0, 0);
        }
        if (setvbo) {
          SoGLVBOElement::setTexCoordVBO(state, 0, PRIVATE(this)->texcoordvbo);
        }
      }
    }
  }
  
  num = this->normal.getNum();
  if (num > 0 && !TEST_OVERRIDE(NORMAL_VECTOR)) {
    SoNormalElement::set(state, this, num,
                         this->normal.getValues(0));
    if (this->isOverride()) {
      SoOverrideElement::setNormalVectorOverride(state, this, TRUE);
    }

    SbBool setvbo = FALSE;
    if (glrender) {
      if ((num == numvertex) && shouldcreatevbo) {
        SbBool dirty = FALSE;
        setvbo = TRUE;
        if (PRIVATE(this)->normalvbo == NULL) {
          PRIVATE(this)->normalvbo = new SoVBO(GL_ARRAY_BUFFER, GL_STATIC_DRAW); 
          dirty =  TRUE;
        }
        else if (PRIVATE(this)->normalvbo->getBufferDataId() != this->getNodeId()) {
          dirty = TRUE;
        }
        if (dirty) {
          PRIVATE(this)->normalvbo->setBufferData(this->normal.getValues(0),
                                                  num*sizeof(SbVec3f),
                                                  this->getNodeId());
        }
      }
      else if (PRIVATE(this)->normalvbo && PRIVATE(this)->normalvbo->getBufferDataId()) {
        // clear buffers to deallocate VBO memory
        PRIVATE(this)->normalvbo->setBufferData(NULL, 0, 0);
      }
      if (setvbo) {
        SoGLVBOElement::setNormalVBO(state, PRIVATE(this)->normalvbo);
      }
    }
  }
  if (this->normal.getNum() > 0 && !TEST_OVERRIDE(NORMAL_BINDING)) {
    SoNormalBindingElement::set(state, this,
                                (SoNormalBindingElement::Binding)
                                this->normalBinding.getValue());
    if (this->isOverride()) {
      SoOverrideElement::setNormalBindingOverride(state, this, TRUE);
    }
  }

  num = this->orderedRGBA.getNum();
  if (num > 0 && 
      !TEST_OVERRIDE(DIFFUSE_COLOR)) {
    
    SoLazyElement::setPacked(state, this, num,
                             this->orderedRGBA.getValues(0),
                             PRIVATE(this)->transparent);
    if (this->isOverride()) {
      SoOverrideElement::setDiffuseColorOverride(state, this, TRUE);
    }
    if (glrender) {
      SbBool setvbo = FALSE;
      if ((num == numvertex) && shouldcreatevbo) {
        SbBool dirty = FALSE;
        setvbo = TRUE;
        if (PRIVATE(this)->colorvbo == NULL) {
          PRIVATE(this)->colorvbo = new SoVBO(GL_ARRAY_BUFFER, GL_STATIC_DRAW);
          dirty = TRUE;
        }
        else if (PRIVATE(this)->colorvbo->getBufferDataId() != this->getNodeId()) {
          dirty = TRUE;
        }
        if (dirty) {
          if (coin_host_get_endianness() == COIN_HOST_IS_BIGENDIAN) {
            PRIVATE(this)->colorvbo->setBufferData(this->orderedRGBA.getValues(0),
                                                   num*sizeof(uint32_t),
                                                   this->getNodeId());
          }
          else {
            const uint32_t * src = this->orderedRGBA.getValues(0);
            uint32_t * dst = (uint32_t*) 
              PRIVATE(this)->colorvbo->allocBufferData(num*sizeof(uint32_t), 
                                                       this->getNodeId());  
            for (int i = 0; i < num; i++) {
              uint32_t tmp = src[i];
              dst[i] = 
                (tmp << 24) |
                ((tmp & 0xff00) << 8) |
                ((tmp & 0xff0000) >> 8) |
                (tmp >> 24);
            }
          }
        }
      }
      else if (PRIVATE(this)->colorvbo) {
        PRIVATE(this)->colorvbo->setBufferData(NULL, 0, 0);
      }
      if (setvbo) {
        SoGLVBOElement::setColorVBO(state, PRIVATE(this)->colorvbo);
      }
    }
  }

  if (glrender) SoBase::staticDataUnlock();

  if (this->orderedRGBA.getNum() && !TEST_OVERRIDE(MATERIAL_BINDING)) {
    SoMaterialBindingElement::set(state, this,
                                  (SoMaterialBindingElement::Binding)
                                  this->materialBinding.getValue());
    if (this->isOverride()) {
      SoOverrideElement::setMaterialBindingOverride(state, this, TRUE);
    }
  }
#undef TEST_OVERRIDE
}

// Documented in superclass.
void
SoVertexProperty::callback(SoCallbackAction *action)
{
  SoVertexProperty::doAction((SoAction*)action);
}

// Documented in superclass.
void
SoVertexProperty::pick(SoPickAction *action)
{
  SoVertexProperty::doAction((SoAction*)action);
}

// Documented in superclass.
void
SoVertexProperty::getPrimitiveCount(SoGetPrimitiveCountAction *action)
{
  SoVertexProperty::doAction((SoAction*)action);
}

// Documented in superclass. Overridden to check for transparency when
// orderedRGBA changes.
void
SoVertexProperty::notify(SoNotList *list)
{
  SoField *f = list->getLastField();
  if (f == &this->orderedRGBA) {
    PRIVATE(this)->checktransparent = TRUE;
  }
  inherited::notify(list);
}

#undef PRIVATE
