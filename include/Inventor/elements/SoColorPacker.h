/**************************************************************************\
 * 
 *  Copyright (C) 1998-1999 by Systems in Motion.  All rights reserved.
 *
 *  This file is part of the Coin library.
 *
 *  This file may be distributed under the terms of the Q Public License
 *  as defined by Troll Tech AS of Norway and appearing in the file
 *  LICENSE.QPL included in the packaging of this file.
 *
 *  If you want to use Coin in applications not covered by licenses
 *  compatible with the QPL, you can contact SIM to aquire a
 *  Professional Edition license for Coin.
 *
 *  Systems in Motion AS, Prof. Brochs gate 6, N-7030 Trondheim, NORWAY
 *  http://www.sim.no/ sales@sim.no Voice: +47 73540378 Fax: +47 73943861
 *
\**************************************************************************/

#ifndef _SO_COLOR_PACKER_H_
#define _SO_COLOR_PACKER_H_

#include <Inventor/confdep.h>
#if defined(COIN_EXCLUDE_SOCOLORPACKER)
#error "Configuration settings disable this class!"
#endif // COIN_EXCLUDE_SOCOLORPACKER

#include <Inventor/SbBasic.h>
#include <Inventor/system/inttypes.h>

class SoColorPacker { // this is not an element
public:
  SoColorPacker( void );
  ~SoColorPacker( void );

  uint32_t * getPackedColors( void );
  const uint32_t * getPackedColors( void ) const;
  SbBool diffuseMatch( const uint32_t nodeId ) const;
  SbBool transpMatch( const uint32_t nodeId ) const;
  void setNodeIds( const uint32_t diffNodeId, const uint32_t tNodeId );
  int32_t getSize( void ) const;
  void reallocate( const int32_t size );
  
private:
  uint32_t transpNodeId;
  uint32_t diffuseNodeId;
  uint32_t *packedColors;
  int32_t packedArraySize;
  
}; // class SoColorPacker

#endif // ! _SO_COLOR_PACKER_H_
