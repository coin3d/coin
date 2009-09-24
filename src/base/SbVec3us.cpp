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

#include <Inventor/SbVec3us.h>

#include <Inventor/SbVec3s.h>
#include <Inventor/SbVec3ub.h>
#include <Inventor/SbVec3ui32.h>
#include "coinString.h"

/*!
  \class SbVec3us Inventor/SbVec3us.h

  \since Coin 2.5
*/

SbVec3us &
SbVec3us::setValue(const SbVec3s & v)
{
  vec[0] = static_cast<unsigned short>(v[0]);
  vec[1] = static_cast<unsigned short>(v[1]);
  vec[2] = static_cast<unsigned short>(v[2]);
  return *this;
}

SbVec3us &
SbVec3us::setValue(const SbVec3ub & v)
{
  vec[0] = static_cast<unsigned short>(v[0]);
  vec[1] = static_cast<unsigned short>(v[1]);
  vec[2] = static_cast<unsigned short>(v[2]);
  return *this;
}

SbVec3us &
SbVec3us::setValue(const SbVec3ui32 & v)
{
  vec[0] = static_cast<unsigned short>(v[0]);
  vec[1] = static_cast<unsigned short>(v[1]);
  vec[2] = static_cast<unsigned short>(v[2]);
  return *this;
}

void
SbVec3us::negate(void)
{
  vec[0] = static_cast<unsigned short>(-vec[0]);
  vec[1] = static_cast<unsigned short>(-vec[1]);
  vec[2] = static_cast<unsigned short>(-vec[2]);
}

SbVec3us &
SbVec3us::operator *= (double d)
{
  vec[0] = static_cast<unsigned short>(vec[0] * d);
  vec[1] = static_cast<unsigned short>(vec[1] * d);
  vec[2] = static_cast<unsigned short>(vec[2] * d);
  return *this;
}

/*!
  Return a string representation of this object
*/
SbString
SbVec3us::toString() const
{
  return ToString(*this);
}

/*!
  Return a byte(binary) representation of this object
*/
SbByteBuffer
SbVec3us::byteRepr() const
{
  return ToByteRepr(*this);
}

/*!
  Convert from a byte(binary) representation, return wether this is a valid conversion
*/
SbBool
SbVec3us::fromByteRepr(const SbByteBuffer & repr)
{
  SbBool conversionOk;
  *this = FromByteRepr<SbVec3us>(repr,&conversionOk);
  return conversionOk;
}

/*!
  Convert from a string representation, return wether this is a valid conversion
*/
SbBool
SbVec3us::fromString(const SbString & str)
{
  SbBool conversionOk;
  *this = FromString<SbVec3us>(str,&conversionOk);
  return conversionOk;
}

/*!
  Dump the state of this object to the \a file stream. Only works in
  debug version of library, method does nothing in an optimized
  compile.
*/
void
SbVec3us::print(FILE * fp) const
{
#if COIN_DEBUG
  fputs(this->toString().getString(),fp);
#endif // COIN_DEBUG
}

#ifdef COIN_TEST_SUITE
#include <Inventor/SbTypeInfo.h>
#include <boost/lexical_cast.hpp>

typedef SbVec3us ToTest;
BOOST_AUTO_TEST_CASE(toString) {
  ToTest val(1,2,3);
  SbString str("<1, 2, 3>");
  BOOST_CHECK_MESSAGE(str == val.toString(),
                      std::string("Mismatch between ") +  val.toString().getString() + " and control string " + str.getString());
}

BOOST_AUTO_TEST_CASE(fromString) {
  ToTest foo;
  SbString test = "<1,2,3>";
  ToTest trueVal(1,2,3);
  foo.fromString(test);
  BOOST_CHECK_MESSAGE(trueVal == foo,
                      std::string("Mismatch between ") +  foo.toString().getString() + " and control " + trueVal.toString().getString());
}

BOOST_AUTO_TEST_CASE(fromXMLTypeString) {
  ToTest foo;
  SbString test = "SbVec3us(1,2,3)";
  ToTest trueVal(1,2,3);
  foo.fromString(test);
  BOOST_CHECK_MESSAGE(trueVal == foo,
                      std::string("Mismatch between ") +  foo.toString().getString() + " and control " + trueVal.toString().getString());
}

BOOST_AUTO_TEST_CASE(fromXMLTypeShortString) {
  ToTest foo;
  SbString test = "(1,2,3)";
  ToTest trueVal(1,2,3);
  foo.fromString(test);
  BOOST_CHECK_MESSAGE(trueVal == foo,
                      std::string("Mismatch between ") +  foo.toString().getString() + " and control " + trueVal.toString().getString());
}

BOOST_AUTO_TEST_CASE(fromInvalidString) {
  ToTest foo;
  SbString test = "<-1,2,3>";
  ToTest trueVal(1,2,3);
  SbBool conversionOk = foo.fromString(test);
  BOOST_CHECK_MESSAGE(conversionOk == FALSE,
                      std::string("Able to convert from ") + test.getString() + " which is not a valid " + SbTypeInfo<ToTest>::getTypeName() + " representation");
}

BOOST_AUTO_TEST_CASE(byteRepr) {
  ToTest val;
  for (int i=0;i<3;++i) {
    SbTypeInfo<ToTest>::PrimitiveType tmp=0;
    for (int j=0;j<sizeof(SbTypeInfo<ToTest>::PrimitiveType);++j) {
      tmp|=(i*sizeof(SbTypeInfo<ToTest>::PrimitiveType)+j+'0')<<(j*8);
    }
    val[i]=tmp;
  }
  SbByteBuffer buf = val.byteRepr();

  const char ToCheck [] = "103254";
  BOOST_CHECK_MESSAGE(buf.size() == (sizeof(ToCheck)-1),
                        std::string("Wrong size in byterepresentation, is ") + boost::lexical_cast<std::string>(buf.size()) + " should be " + boost::lexical_cast<std::string>((sizeof(ToCheck)-1))
                      );
  //FIXME: This check probably needs to be rewritten for a big-endian machine
  bool allOk = true;
  for (int i=0;i<sizeof(ToCheck)-1;++i) {
    if(buf[i] != ToCheck[i]) {
      printf("CharVal %d: %x != %x \n",i,static_cast<unsigned char>(buf[i]),static_cast<unsigned char>(ToCheck[i]));
      allOk = false;
    }
  }
  BOOST_CHECK_MESSAGE(allOk,
                      "Wrong pattern in byterepresentation.");
}

BOOST_AUTO_TEST_CASE(fromByteRepr) {
  ToTest foo(1,2,3);
  ToTest bar;

  SbBool conversionOk = bar.fromByteRepr(foo.byteRepr());

  BOOST_CHECK_MESSAGE(foo == bar && conversionOk,
                      std::string("Mismatch between ") +  foo.toString().getString() + " and control " + bar.toString().getString()    );
}

BOOST_AUTO_TEST_CASE(fromInvalidByteRepr) {
  ToTest bar;
  SbByteBuffer test("FOOBAR");

  SbBool conversionOk = bar.fromByteRepr(test);

  BOOST_CHECK_MESSAGE(conversionOk == FALSE,
                      std::string("Able to convert from '") + test.constData() + "' which is not a valid " + SbTypeInfo<ToTest>::getTypeName() + " binary representation");
}
#endif //COIN_TEST_SUITE
