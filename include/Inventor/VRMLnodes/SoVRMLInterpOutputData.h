/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 2001 by Systems in Motion. All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public License
 *  version 2.1 as published by the Free Software Foundation. See the
 *  file LICENSE.LGPL at the root directory of the distribution for
 *  more details.
 *
 *  If you want to use Coin for applications not compatible with the
 *  LGPL, please contact SIM to acquire a Professional Edition license.
 *
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY
 *  http://www.sim.no support@sim.no Voice: +47 22114160 Fax: +47 22207097
 *
\**************************************************************************/

#ifndef COIN_SOVRMLINTERPOUTPUTDATA_H
#define COIN_SOVRMLINTERPOUTPUTDATA_H

#include <Inventor/nodes/SoSubNode.h>

class SoVRMLInterpolator;

class COIN_DLL_API SoVRMLInterpOutputData
{
public:
  SoVRMLInterpOutputData(void);
  SoVRMLInterpOutputData( const SoVRMLInterpOutputData * other );
  SoVRMLInterpOutputData( int numOutputs );
  ~SoVRMLInterpOutputData();

  void addOutput( const SoVRMLInterpolator * defInterp, const char * name,
                  const SoVRMLInterpOutput * output, SoType type );

  int getNumOutputs(void) const;
  const SbName & getOutputName( int idx ) const;
  SoVRMLInterpOutput * getOutput( const SoVRMLInterpolator * interp,
                                  int idx ) const;

  int getIndex( const SoVRMLInterpolator * interp,
                const SoVRMLInterpOutput * output ) const;

  const SoType & getType( int idx ) const;

  SbBool readDescriptions( SoInput * in, SoVRMLInterpolator * interp ) const;
  void writeDescriptions( SoOutput * out, SoVRMLInterpolator * interp ) const;

}; // class SoVRMLInterpOutputData

#endif // ! COIN_SOVRMLINTERPOUTPUTDATA_H
