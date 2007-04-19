#ifndef COIN_FIELDS_SHARED_H
#define COIN_FIELDS_SHARED_H

/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2007 by Systems in Motion.  All rights reserved.
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
 *  See http://www.coin3d.org/ for more information.
 *
 *  Systems in Motion, Postboks 1283, Pirsenteret, 7462 Trondheim, NORWAY.
 *  http://www.sim.no/  sales@sim.no  coin-support@coin3d.org
 *
\**************************************************************************/

class SbMatrix;
class SbPlane;
class SbRotation;
class SbString;
class SbTime;
class SbVec2f;
class SbVec3d;
class SbVec3f;
class SbVec4f;
class SoField;
class SoInput;
class SoOutput;

#include <Inventor/SbBasic.h>

// *************************************************************************

SbBool sosfbool_read_value(SoInput * in, SbBool & val);
void sosfbool_write_value(SoOutput * out, SbBool val);

void sosffloat_write_value(SoOutput * out, float val);

void sosfstring_write_value(const SoField * f, SoOutput * out,
                            const SbString & val);

void sosfmatrix_write_value(SoOutput * out, const SbMatrix & m);

SbBool sosfplane_read_value(SoInput * in, SbPlane & p);
void sosfplane_write_value(SoOutput * out, const SbPlane & p);

SbBool sosfrotation_read_value(SoInput * in, SbRotation & r);
void sosfrotation_write_value(SoOutput * out, const SbRotation & r);

void sosfshort_write_value(SoOutput * out, short val);

SbBool sosftime_read_value(SoInput * in, SbTime & t);
void sosftime_write_value(SoOutput * out, const SbTime & p);

void sosfuint32_write_value(SoOutput * out, uint32_t val);

void sosfushort_write_value(SoOutput * out, unsigned short val);

void sosfvec2f_write_value(SoOutput * out, const SbVec2f & v);

SbBool sosfvec3d_read_value(SoInput * in, SbVec3d & v);
void sosfvec3d_write_value(SoOutput * out, const SbVec3d & v);

void sosfvec3f_write_value(SoOutput * out, const SbVec3f & v);

void sosfvec4f_write_value(SoOutput * out, const SbVec4f & v);

// *************************************************************************

#endif // ! COIN_FIELDS_SHARED_H
