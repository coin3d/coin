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
 *  http://www.sim.no/ sales@sim.no Voice: +47 22114160 Fax: +47 67172912
 *
\**************************************************************************/

/*!
  \class SoColorPacker Inventor/elements/SoColorPacker.h
  \brief The SoColorPacker class is meant to be used by nodes that set a
  diffuse color or transparency in the lazy element.

  The SoColorPacker maintains a cache of the current diffuse colors and
  transparencies in an array of packed colors ((uint32_t) BGRA).
*/

#include <Inventor/elements/SoColorPacker.h>

#include <Inventor/SbName.h>

#include <string.h>

/*!
  The constructor initializes the packedColors array to NULL.
*/

SoColorPacker::SoColorPacker()
  : transpNodeId(0), diffuseNodeId( 0 ), packedColors( NULL ),
    packedArraySize(0)
{
}

/*!
  The destructor frees the packedColors array.
*/

SoColorPacker::~SoColorPacker()
{
  delete [] packedColors;
}

/*!
  This method returns the array of packed colors (modifyable).
*/

//$ EXPORT INLINE
uint32_t *
SoColorPacker::getPackedColors()
{
  return this->packedColors;
}

/*!
  This method returns a constant array of packed colors (for constant
  SoPackedColor objects).
*/

//$ EXPORT INLINE
const uint32_t *
SoColorPacker::getPackedColors() const
{
  return this->packedColors;
}

/*!
  This method returns TRUE if \a nodeId equals the diffNodeId set by
  SoColorPacker::setNodeIds().
*/

//$ EXPORT INLINE
SbBool
SoColorPacker::diffuseMatch(const uint32_t nodeId) const
{
  return (nodeId == this->diffuseNodeId);
}

/*!
  This method returns TRUE if \a nodeId equals the tNodeId set by
  SoColorPacker::setNodeIds().
*/

//$ EXPORT INLINE
SbBool
SoColorPacker::transpMatch(const uint32_t nodeId) const
{
  return (nodeId == this->transpNodeId);
}

/*!
  This function sets the node identifiers for the node setting the diffuse
  colors and transparencies.
*/

//$ EXPORT INLINE
void
SoColorPacker::setNodeIds(const uint32_t diffNodeId, const uint32_t tNodeId)
{
  this->diffuseNodeId = diffNodeId;
  this->transpNodeId = tNodeId;
}

/*!
  This function returns the allocated size (in elements) for the array.
  The number of packed colors in the array may not be the same.
*/

//$ EXPORT INLINE
int32_t
SoColorPacker::getSize() const
{
  return this->packedArraySize;
}

/*!
  This function allocates an array with space for \a size packed colors, and
  copies the colors from the old array into it.
*/

void
SoColorPacker::reallocate(const int32_t size)
{
  uint32_t * newArray = new uint32_t [ size ];
  int bytes = SbMin(size, this->packedArraySize) * sizeof(int32_t);
  if (bytes)
    memcpy(newArray, this->packedColors, bytes);
  this->packedArraySize = size;
  if (this->packedColors)
    delete [] this->packedColors;
  this->packedColors = newArray;
}
