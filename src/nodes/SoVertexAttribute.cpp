/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2008 by Kongsberg SIM.  All rights reserved.
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

#include <Inventor/nodes/SoVertexAttribute.h>

#include <boost/scoped_ptr.hpp>

#include <Inventor/actions/SoWriteAction.h>
#include <Inventor/SoInput.h>
#include <Inventor/fields/SoFieldData.h>
#include <Inventor/fields/SoMFFloat.h>
#include <Inventor/fields/SoMFVec2f.h>
#include <Inventor/fields/SoMFVec3f.h>
#include <Inventor/fields/SoMFVec4f.h>
#include <Inventor/fields/SoMFShort.h>
#include <Inventor/fields/SoMFInt32.h>
#include <Inventor/fields/SoMFMatrix.h>
#include <Inventor/errors/SoReadError.h>
#include <Inventor/errors/SoDebugError.h>

#include "nodes/SoSubNodeP.h"

/*!
  \class SoVertexAttribute Inventor/nodes/SoVertexAttribute.h
  \brief A generic node for providing GL vertex attributes of various types.

  Vertex attributes are used for vertex shaders.

  Vertex attributes are sent using the texture coordinate indexes of
  the shape being rendered.  This was a design decision we made to
  make vertex attributes fit into the existing node design without
  having to make changes that would cause a lot of incompatibility
  problems and add too much complexity in scene graph design and the
  geometry generation code.  In many respects, texture coordinates are
  just a specialization of vertex attributes, so classifying them
  together makes in that way good sense.

  \ingroup nodes
  \since 2008-03-15
*/

class SoVertexAttributeP {
public:
  SbBool isreading;
  SoMFFloat dummyfield;
  boost::scoped_ptr<SoMField> valuesfield;
  boost::scoped_ptr<SoFieldData> fielddata;

  void setupFieldData(void);

}; // SoVertexAttributeP

// *************************************************************************

#define PRIVATE(obj) ((obj)->pimpl)
#define DMSG SoDebugError::postInfo

SO_NODE_SOURCE(SoVertexAttribute);

void
SoVertexAttribute::initClass(void)
{
  SO_NODE_INTERNAL_INIT_CLASS(SoVertexAttribute, SO_FROM_COIN_3_0);
}

SoVertexAttribute::SoVertexAttribute(void)
{
  PRIVATE(this)->isreading = FALSE;

  SO_NODE_CONSTRUCTOR(SoVertexAttribute);

  SO_NODE_ADD_FIELD(type, (NONE));
  SO_NODE_ADD_FIELD(name, (""));

  SO_NODE_DEFINE_ENUM_VALUE(AttributeType, NONE);
  SO_NODE_DEFINE_ENUM_VALUE(AttributeType, FLOAT);
  SO_NODE_DEFINE_ENUM_VALUE(AttributeType, VEC2F);
  SO_NODE_DEFINE_ENUM_VALUE(AttributeType, VEC3F);
  SO_NODE_DEFINE_ENUM_VALUE(AttributeType, VEC4F);
  SO_NODE_DEFINE_ENUM_VALUE(AttributeType, INT16);
  SO_NODE_DEFINE_ENUM_VALUE(AttributeType, INT32);
  SO_NODE_DEFINE_ENUM_VALUE(AttributeType, MATRIX);

  SO_NODE_SET_SF_ENUM_TYPE(type, AttributeType);
}

SoVertexAttribute::~SoVertexAttribute(void)
{
}

void
SoVertexAttribute::GLRender(SoGLRenderAction * action)
{
}

void
SoVertexAttribute::write(SoWriteAction * action)
{
}

//SoMField *
//SoVertexAttribute::getValuesField(void)
//{
//  return this->values;
//}

SbBool
SoVertexAttribute::readInstance(SoInput * in, unsigned short flags)
{
  PRIVATE(this)->isreading = TRUE;
  // avoid triggering in the notify()-function while reading the file.

  static const SbName typekey("type");
  static const SbName namekey("name");
  static const SbName valueskey("values");

  SbName fieldname(SbName::empty());
  SbBool ok = in->read(fieldname, TRUE);
  SbBool err = FALSE;

  while (ok && !err) {
    DMSG("SoVertexAttribute::readInstance", "found fieldname '%s'",
         fieldname.getString());

    if (fieldname == typekey) {
      err = !this->type.read(in, typekey);
      if (!err) {
        if (PRIVATE(this)->valuesfield.get()) {
          PRIVATE(this)->valuesfield.reset(NULL);
        }
        switch (this->type.getValue()) {
        case FLOAT:   PRIVATE(this)->valuesfield.reset(new SoMFFloat);   break;
        case VEC2F:   PRIVATE(this)->valuesfield.reset(new SoMFVec2f);   break;
        case VEC3F:   PRIVATE(this)->valuesfield.reset(new SoMFVec3f);   break;
        case VEC4F:   PRIVATE(this)->valuesfield.reset(new SoMFVec4f);   break;
        case INT16:   PRIVATE(this)->valuesfield.reset(new SoMFShort);   break;
        case INT32:   PRIVATE(this)->valuesfield.reset(new SoMFInt32);   break;
        case MATRIX:  PRIVATE(this)->valuesfield.reset(new SoMFMatrix);  break;
        default:
          DMSG("SoVertexAttribute::readInstance", "read unknown value %d",
               this->type.getValue());
          break;
        }
        if (PRIVATE(this)->valuesfield.get()) {
          //PRIVATE(this)->fielddata->addField(this, "values", PRIVATE(this)->valuesfield.get());
        }
      }
    }
    else if (fieldname == namekey) {
      err = !this->name.read(in, namekey);
      if (!err) {
        DMSG("SoVertexAttribute::readInstance", "read the value '%s'",
             this->name.getValue().getString());
      }
    }
    else if (fieldname == valueskey) {
      if (PRIVATE(this)->valuesfield.get()) {
        err = !PRIVATE(this)->valuesfield->read(in, valueskey);
        if (!err) {
          DMSG("SoVertexAttribute::readInstance", "read in %d values",
               PRIVATE(this)->valuesfield->getNum());
        }
      }
    }

    if (err) {
      DMSG("SoVertexAttribute::readInstance",
           "error reading field contents for field %s", fieldname.getString());
    }
    else {
      ok = in->read(fieldname, TRUE);
    }
  }

  PRIVATE(this)->isreading = FALSE;

  if (!err) {
    if (fieldname != "") in->putBack(fieldname.getString());
  }

  return !err;
}

void
SoVertexAttribute::notify(SoNotList * list)
{
  if (PRIVATE(this)->isreading) return; // ignore notification while reading

  DMSG("SoVertexAttribute::notify", "");
  inherited::notify(list);
}

#undef PRIVATE
#undef DMSG
