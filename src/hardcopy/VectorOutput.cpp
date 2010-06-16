/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) by Kongsberg Oil & Gas Technologies.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  ("GPL") version 2 as published by the Free Software Foundation.
 *  See the file LICENSE.GPL at the root directory of this source
 *  distribution for additional information about the GNU GPL.
 *
 *  For using Coin with software that can not be combined with the GNU
 *  GPL, and for taking advantage of the additional benefits of our
 *  support services, please contact Kongsberg Oil & Gas Technologies
 *  about acquiring a Coin Professional Edition License.
 *
 *  See http://www.coin3d.org/ for more information.
 *
 *  Kongsberg Oil & Gas Technologies, Bygdoy Alle 5, 0257 Oslo, NORWAY.
 *  http://www.sim.no/  sales@sim.no  coin-support@coin3d.org
 *
\**************************************************************************/

#include <Inventor/annex/HardCopy/SoVectorOutput.h>

#include <Inventor/SbBasic.h>

#include "tidbitsp.h"

// *************************************************************************

/*!
  \class SoVectorOutput HardCopy/SoVectorOutput.h
  \brief The SoVectorOutput class is used for setting vector output file.
  \ingroup hardcopy

  SoVectorizeAction will create an SoVectorOutput which will output
  to stdout by default. SoVectorizeAction::getOutput() can be used to
  fetch this output, and the user will probably want to set a
  file to output to.

  \since Coin 2.1
  \since TGS provides HardCopy support as a separate extension for TGS Inventor.
*/

class SoVectorOutputP {
public:
  FILE * fp;
  SbBool didopen;
};

#define PRIVATE(p) (p->pimpl)
#define PUBLIC(p) (p->publ)

// *************************************************************************

/*!
  Constructor. Will make this instance output to \e stdout.
*/
SoVectorOutput::SoVectorOutput(void)
{
  PRIVATE(this) = new SoVectorOutputP;
  PRIVATE(this)->fp = coin_get_stdout();
  PRIVATE(this)->didopen = FALSE;
}

/*!
  Destructor. Will close the file opened in openFile().
*/
SoVectorOutput::~SoVectorOutput()
{
  this->closeFile();
  delete PRIVATE(this);
}

// *************************************************************************

/*!
  Opens \a filename for output. Returns \e FALSE if file could not be
  opened for writing. If the file already exists, it will be
  truncated.
 */
SbBool
SoVectorOutput::openFile(const char * filename)
{
  this->closeFile();

  FILE * fp = fopen(filename, "wb");
  if (fp) {
    PRIVATE(this)->fp = fp;
    PRIVATE(this)->didopen = TRUE;
  }
  return fp != NULL;
}

/*!
  Closes the file opened in openFile()
*/
void
SoVectorOutput::closeFile(void)
{
  if (PRIVATE(this)->didopen) {
    fclose(PRIVATE(this)->fp);
    PRIVATE(this)->fp = stdout;
    PRIVATE(this)->didopen = FALSE;
  }
}

/*!
  Returns the \e stdio file handle pointer.
*/
FILE *
SoVectorOutput::getFilePointer(void)
{
  return PRIVATE(this)->fp;
}

#undef PRIVATE
#undef PUBLIC
