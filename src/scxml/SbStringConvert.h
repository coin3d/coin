#ifndef COIN_SBSTRINGCONVERT_H
#define COIN_SBSTRINGCONVERT_H

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

#include <Inventor/SbBasic.h>
#include <Inventor/SbString.h>
#include "base/coinString.h"

class SbString;
class SbVec2s;
class SbVec2f;
class SbVec3f;
class SbVec3d;
class SbRotation;

template<size_t numArgs, typename Type, typename ArgumentType>
  struct constructFromArray {
    
  };

template<typename Type, typename ArgumentType>
  struct constructFromArray<2,Type,ArgumentType> {
  static Type constructor(const ArgumentType * inArray)
  {
    return Type(inArray[0],inArray[1]);
  }
};

template<typename Type, typename ArgumentType>
  struct constructFromArray<3,Type,ArgumentType> {
  static Type constructor(const ArgumentType * inArray)
  {
    return Type(inArray[0],inArray[1],inArray[2]);
  }
};

template<typename Type, typename ArgumentType>
  struct constructFromArray<4,Type,ArgumentType> {
  static Type constructor(const ArgumentType * inArray)
  {
    return Type(inArray[0],inArray[1],inArray[2],inArray[3]);
  }
};

template <typename T>
struct ScXMLConvert {
    static
    T fromString(const SbString & str, SbBool * conversionOk = NULL)
    {
      typename SbTypeInfo<T>::PrimitiveType tmpVal[SbTypeInfo<T>::Dimensions];
      SbString substr;
      int start = str.find("(");
      int end = str.find(")");
      if (start == -1 || end == -1) {
        if (conversionOk) *conversionOk = FALSE;
        return T();
      }
      substr = str.getSubString(0, start-1);
      if (substr != SbTypeInfo<T>::getTypeName()) {
        if (conversionOk) *conversionOk = FALSE;
        return T();
      }
      --end;
      ++start;
      assert(end>start);
      substr = str.getSubString(start,end);
      start = 0;
      SbIntList indices;
      substr.findAll(",",indices);
      if (indices.getLength()!= SbTypeInfo<T>::Dimensions-1) {
        if (conversionOk) *conversionOk = FALSE;
        return T();
      }
      for (int i=0;i<SbTypeInfo<T>::Dimensions-1;++i) {
        end = indices[i];
        SbString token = substr.getSubString(start,end);
        start=end+1;
        typename SbTypeInfo<T>::PrimitiveType t = FromString< typename SbTypeInfo<T>::PrimitiveType > (token, conversionOk);
        tmpVal[i]=t;
      }
      SbString token = substr.getSubString(start,-1);
      typename SbTypeInfo<T>::PrimitiveType t = FromString< typename SbTypeInfo<T>::PrimitiveType > (token, conversionOk);
      tmpVal[SbTypeInfo<T>::Dimensions-1]=t;
      if (conversionOk) *conversionOk = TRUE;
      return constructFromArray<SbTypeInfo<T>::Dimensions ,T, typename SbTypeInfo<T>::PrimitiveType >::constructor(tmpVal);
    }
};

template<typename T>
struct PrimitiveConvert {
  static T fromString(const SbString & str, SbBool * conversionOk = NULL) {
      return FromString<T>(str, conversionOk);
  }
};

class SbStringConvert {
public:
  enum TypeIdentity {
    BOOLEAN,
    NUMERIC,
    SBVEC2S,
    SBVEC2F,
    SBVEC3F,
    SBVEC3D,
    SBROTATION,

    UNKNOWN
  };

  static TypeIdentity typeOf(const SbString & str);
  
  template <typename T>
    static
    T fromString(const SbString & str, SbBool * conversionOk = NULL)
    {
      return
        IF< SbTypeInfo<T>::isPrimitive, PrimitiveConvert<T>, ScXMLConvert<T> >::RET::fromString(str,conversionOk);
    }

  template <typename T>
    static
    SbString toString(const T & in)
    {
      SbString retVal =  IF<
        SbTypeInfo<T>::isPrimitive,
        SbStringConvert,
        SbTypeInfo<T>
        >::RET::getTypeName();
      
      retVal += start(SbTypeInfo<T>::isPrimitive);
      retVal += ToString<T>(in);
      retVal += end(SbTypeInfo<T>::isPrimitive);
      return retVal;
    }

 private:
  static const char * getTypeName()
  {
    return "";
  }
  static const char * start(bool yes)
  {
    return (yes)?"(":"";
  }
  static const char * end(bool yes)
  {
    return (yes)?")":"";
  }
};

#endif // !COIN_SBSTRINGCONVERT_H
