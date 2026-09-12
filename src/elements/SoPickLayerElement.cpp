/**************************************************************************\
 * Copyright (c) Kongsberg Oil & Gas Technologies AS
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 * Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 *
 * Redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the distribution.
 *
 * Neither the name of the copyright holder nor the names of its
 * contributors may be used to endorse or promote products derived from
 * this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
\**************************************************************************/

/*!
  \class SoPickLayerElement Inventor/elements/SoPickLayerElement.h
  \brief The SoPickLayerElement holds the current pick layer.

  \ingroup coin_elements

  The pick layer orders picked points across the scene graph. Picked points
  in a higher layer sort in front of those in a lower layer, independent of
  their depth. It is paired with SoPickStyleElement: on-top pick styles ignore
  the layer and always sort frontmost. The default layer is 0.
*/

#include <Inventor/elements/SoPickLayerElement.h>

SO_ELEMENT_SOURCE(SoPickLayerElement);

/*!
  \copydetails SoElement::initClass(void)
*/

void
SoPickLayerElement::initClass(void)
{
  SO_ELEMENT_INIT_CLASS(SoPickLayerElement, inherited);
}

/*!
  Destructor.
*/

SoPickLayerElement::~SoPickLayerElement()
{
}

//! Sets the current pick layer.

void
SoPickLayerElement::set(SoState * const state,
                        SoNode * const node,
                        const int32_t layer)
{
  SoInt32Element::set(classStackIndex, state, node, layer);
}

/*!
  Initializes the element to its default value. The default
  value for the pick layer is 0.
*/

void
SoPickLayerElement::init(SoState * state)
{
  inherited::init(state);
  this->data = getDefault();
}

//! Sets the current pick layer.

//$ EXPORT INLINE
void
SoPickLayerElement::set(SoState * const state, const int32_t layer)
{
  set(state, NULL, layer);
}

//! Returns the current pick layer.

//$ EXPORT INLINE
int32_t
SoPickLayerElement::get(SoState * const state)
{
  return SoInt32Element::get(classStackIndex, state);
}

//! Returns the default pick layer (0).

//$ EXPORT INLINE
int32_t
SoPickLayerElement::getDefault()
{
  return 0;
}
