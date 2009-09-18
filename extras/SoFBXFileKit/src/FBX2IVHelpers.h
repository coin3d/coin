#ifndef FBX2IVHELPERS_H
#define FBX2IVHELPERS_H

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

#include <Inventor/errors/SoDebugError.h>
#include <Inventor/fields/SoMFInt32.h>
#include <Inventor/lists/SoFieldList.h>
#include <fbxsdk.h>
#include <stdio.h>
#include <stdarg.h>

namespace FBX2IVHelpers
{
  template <typename T>
  T clamp(T val, T min, T max) {
    return std::min(max, std::max(min, val));
  }

  /*!
    Typesafe conversion function template.
    The general case tries to use built in conversion.
  */
  template<typename T, typename U>
  T convert(const U & u) {
    return T(u);
  }

  /*!
    Converts KFbxVector4 to SbVec2f.
   */
  template<>
  SbVec2f convert<>(const KFbxVector4 & d4)
  {
    return SbVec2f(d4[0], d4[1]);
  }

  /*!
    Converts fbxDouble3 to SbVec3f.
   */
  template<>
  SbVec3f convert<>(const fbxDouble3 & t1)
  {
    return SbVec3f(t1[0], t1[1], t1[2]);
  }

  /*!
    Converts fbxDouble3 to SbColor.
   */
  template<>
  SbColor convert<>(const fbxDouble3 & t1)
  {
    return SbColor(t1[0], t1[1], t1[2]);
  }

  /*!
    Converts a KFbxMatrix to a SbMatrix. 
   */
  template<>
  SbMatrix 
  convert<>(const KFbxMatrix & m)
  {
    return SbMatrix(m[0][0], m[0][1], m[0][2], m[0][3], 
                    m[1][0], m[1][1], m[1][2], m[1][3], 
                    m[2][0], m[2][1], m[2][2], m[2][3], 
                    m[3][0], m[3][1], m[3][2], m[3][3]);
  }

  
  /*!
    Converts KFbxColor to SbColor.
   */
  template<>
  SbColor convert<>(const KFbxColor & c)
  {
    return SbColor(c.mRed, c.mGreen, c.mBlue);
  }


  /*!
    Converts KFbxVector4 to SbVec3f.
   */
  template<>
  SbVec3f convert<>(const KFbxVector4 & d4)
  {
    return SbVec3f(d4[0], d4[1], d4[2]);
  }


  /*!
    Converts KFbxVector2 to SbVec2f.
   */
  template<>
  SbVec2f convert<>(const KFbxVector2 & d2)
  {
    return SbVec2f(d2[0], d2[1]);
  }


  /*!
    Converts KFbxVector2 to SbVec3f.
   */
  template<>
  SbVec3f convert<>(const KFbxVector2 & d2)
  {
    return SbVec3f(d2[0], d2[1], 0.0f);
  }

  /*!
    Gets the index of the element referenced by 
    either vertexIndex, vertexNumber or polygonNumber.
    Which value is used to retrieve the index depends 
    on the mapping and reference types of the given 
    layer element.
  */
  template<typename T1>
  int getIndex(const KFbxLayerElementTemplate<T1> & layerElement, 
               int vertexIndex, int vertexNumber, int polygonNumber)
  {
    int usedIndex;
    switch (layerElement.GetMappingMode()) {
    case KFbxLayerElement::eBY_CONTROL_POINT:
      usedIndex = vertexIndex;
      break;
    case KFbxLayerElement::eBY_POLYGON_VERTEX:
      usedIndex = vertexNumber;
      break;
    case KFbxLayerElement::eBY_POLYGON:
      usedIndex = polygonNumber;
      break;
    case KFbxLayerElement::eBY_EDGE:
    case KFbxLayerElement::eNONE:
    default:
#if COIN_DEBUG
      SoDebugError::post("FBX2IVHelpers::getIndex", 
                         "Unsupported mapping mode %d. Falling back to eALL_SAME.",
                         layerElement.GetMappingMode());
#endif // COIN_DEBUG
    case KFbxLayerElement::eALL_SAME:
      usedIndex = 0;
      break;
    }

    switch (layerElement.GetReferenceMode()) {
    case KFbxLayerElement::eDIRECT:
      return usedIndex;
    case KFbxLayerElement::eINDEX_TO_DIRECT:
      return layerElement.GetIndexArray().GetAt(usedIndex);
    case KFbxLayerElement::eINDEX:
    default:
#if COIN_DEBUG
      SoDebugError::post("FBX2IVHelpers::getIndex", "Unsupported reference mode %d.", 
                         layerElement.GetReferenceMode());
#endif // COIN_DEBUG
      assert(false);
      break;
    }
    return -1;
  }

  /*!
    Gets the value of the referenced element of a layer element and 
    converts the result into type T2. The function uses the proper 
    index value by evaluating the mapping and referencing mode.
   */
  template<typename T2, typename T1>
  T2 getValue(const KFbxLayerElementTemplate<T1> & layerElement, 
              int vertexIndex, int vertexNumber, int polygonNumber)
  {
    int usedIndex = getIndex(layerElement, vertexIndex, vertexNumber, polygonNumber);
    return convert<T2>(layerElement.GetDirectArray()[usedIndex]);
  }

  /*!
    Checks the given index field for optimisation and writes the possible optimization
    options in canMakePerFace and canMakeOverall.
    Returns false if the index array was empty otherwise true.
    If canMakePerFace is initially true per face indexing will be forced no matter of 
    different values per face.
    Although OVERALL will override per face if possible.
   */
  bool checkIndices(SoMFInt32 & indices, bool & canMakePerFace, bool & canMakeOverall, int & overallIndex, std::map<int, int> & perFaceIndices)
  {
    bool forcePerFace = canMakePerFace;

    canMakeOverall = true;
    canMakePerFace = true;
    int indexCount = indices.getNum();
    if (!indexCount) {
      return false;
    }
    int polygonNo = 0;
    int current;
    bool isFirstIndex = true;
    overallIndex = -1;
    bool needMinus = false;
    for (int i = 0; i < indexCount; ++i) {
      current = indices[i];
      if (-1 == current) {
        if (needMinus) {
          isFirstIndex = true;
          ++polygonNo;
        } else {
          // skip empty polygons
          continue;       
        }
      } else {
        needMinus = true;
        if (isFirstIndex) {
          if (-1 == overallIndex) {
            overallIndex = current;
          }
          isFirstIndex = false;
          perFaceIndices[polygonNo] = current;
        } else if (perFaceIndices[polygonNo] != current) {
          if (!forcePerFace) {
            canMakePerFace = false;
          }
          canMakeOverall = false;
          break;
        }
        if (overallIndex != -1 && overallIndex != current) {
          canMakeOverall = false;
        }
      }
    }
    return true;
  }


  /*!
    Sets the default flag of each field in \a soNode if its content matches 
    the corresponding field in \a defaultNode.
    The two given nodes must be of the same type.

    /return true if all fields of \a soNode and \a defaultNode have
    the same values, false otherwise.
  */
  bool resetDefault(SoNode * soNode, SoNode * defaultNode) 
  {
    bool allDefault = true;

    SoFieldList fields;
    SoFieldList defaultFields;
    int count = soNode->getFields(fields);
    defaultNode->getFields(defaultFields);
    for (int i=0;i<count;i++) {
      if ((*fields[i]) == (*defaultFields[i])) fields[i]->setDefault(true);
      else allDefault = false;
    }
    return allDefault;
  }

  /*!
    Performs an evaluation of the \a toCheck node's fields.
    Every field which has only default value will get the default flag set.

    \a defaults is a cache of default node instances which will be updated
    with an instance of the type of \a toCheck if necessary.

    /return true if all fields have default values, false otherwise.
   */
  bool 
  resetDefaultFields(SoNode * toCheck, std::map<int, SoNode *> & defaults)
  { 
    assert(toCheck && "FBX2IVHelpers::resetDefaultFields: No SoNode to test given.");

    SoNode * soNode = (SoNode *)toCheck;
    SoNode * defaultNode = NULL;
    SoType type = toCheck->getTypeId();
    if ((defaultNode = defaults[type.getKey()]) == NULL) {
      defaultNode = (SoNode *)type.createInstance();
      defaultNode->ref();
      defaults[type.getKey()] = defaultNode;
    }

    return resetDefault(soNode, defaultNode);
  }

#if 0
  /*!
    Performes default check by creating (or fetching from the given defauls by using
    the type id of the node type T) a defaultNode of the
    given type and use of resetDefault.
    Returns true if all fields have default values otherwise false.
   */
  template<typename T>
  bool 
  resetDefaultFields(T * toCheck, std::map<int, SoNode *> & defaults)
  { 
    assert(toCheck && "FBX2IVHelpers::resetDefaultFields: No SoNode to test given.");

    SoNode * soNode = (SoNode *)toCheck;
    SoNode * defaultNode = NULL;
    if (NULL == (defaultNode = defaults[T::getClassTypeId().getKey()])) {
     defaultNode = new T();
     defaultNode->ref();
     defaults[T::getClassTypeId().getKey()] = defaultNode;
    }

    return resetDefault(soNode, defaultNode);
  }
#endif

  /*!
    This function checks is a given nodes fields are all default (by using
    resetDefaultFields). If so, the node will be deleted and the function 
    returns NULL otherwise the fields of the node will be tested for 
    reseting them to default.
   */
  SoNode * 
  destroyIfAllDefault(SoNode ** soNode, std::map<int, SoNode *> & defaults) 
  {
    if (!(*soNode)) {
      return NULL;
    }
    if (resetDefaultFields(*soNode, defaults)) {
      (*soNode)->ref();
      (*soNode)->unref();
      (*soNode) = NULL;
    }
    return *soNode;
  }


  /*!
    Moves the indices stored in perFaceIndices to indices.
    Used for conversion of index fields with per vertex indexing
    to fields with per face indexing.
   */
  void convertIndicesToPerFace(SoMFInt32 & indices, std::map<int, int> perFaceIndices) {
    indices.setNum(perFaceIndices.size());
    for (std::map<int, int>::iterator i = perFaceIndices.begin(); i != perFaceIndices.end(); ++i) {
      indices.set1Value(i->first, i->second);
    }
  }


  /*!
    Creates a binding node of the type given by SOBINDINGTYPE by checking and
    probably converting the given index array. The index array is supposed 
    to be set up in PER_VERTEX_INDEXED mode.
    If the functuion encounters that each face's indices are the same a PER_FACE_INDEXED
    binding will be created where it is possible.
    If the function encounters that all indices are the same a OVERALL binding will be
    created. OVERALL will be used if possible.
   */
  template<typename SOBINDINGTYPE>
  SOBINDINGTYPE * createBindingAndOptimizedIndices(SoMFInt32 & indices, std::map<int, SoNode *> & defaults, bool forcePerFace = false)
  {
    bool canMakePerFace = forcePerFace;
    bool canMakeOverall = false;
    SOBINDINGTYPE * binding = NULL;

    std::map<int, int> perFaceIndices;
    int overallIndex = 0;

    if (checkIndices(indices, canMakePerFace, canMakeOverall, overallIndex, perFaceIndices)) {
      binding = new SOBINDINGTYPE();
      if (canMakeOverall) {
        indices.setValue(overallIndex);
        binding->value = SOBINDINGTYPE::OVERALL;
      } else if (canMakePerFace) {
        convertIndicesToPerFace(indices, perFaceIndices);
        binding->value = SOBINDINGTYPE::PER_FACE_INDEXED;
      } else {
        binding->value = SOBINDINGTYPE::PER_VERTEX_INDEXED;
      }
      destroyIfAllDefault((SoNode **)&binding, defaults);
    }
    return binding;
  }


  /*!
    Adds a node to the given parent unless the node contains only default values.
    If the node contains only default values it will be deleted and set to NULL.
    Fields having default values will be set explicitly to default (preventing 
    them from being written to file).

    The defaults map is a map containing a cache of nodes initialized with default
    values. This is used to check if fields not flagged as default actually contain
    default values.
  */
  bool
  addNonDefault(SoGroup * parent, SoNode ** node, std::map<int, SoNode *> & defaults)
  {
    (*node) = destroyIfAllDefault(node, defaults);
    if (*node) {
      parent->addChild(*node);
      return true;
    }

    return false;
  }
  
} // namespace

#endif
