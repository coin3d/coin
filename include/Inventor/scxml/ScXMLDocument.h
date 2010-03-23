#ifndef COIN_SCXMLDOCUMENT_H
#define COIN_SCXMLDOCUMENT_H

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

#include <Inventor/scxml/ScXMLObject.h>

#include <Inventor/tools/SbPimplPtr.h>

class SbByteBuffer;
class ScXMLScxmlElt;
class ScXMLAbstractStateElt;
class ScXMLDataElt;

struct cc_xml_doc;

class COIN_DLL_API ScXMLDocument : public ScXMLObject {
  typedef ScXMLObject inherited;
  SCXML_OBJECT_HEADER(ScXMLDocument)

public:
  static void initClass(void);
  static void cleanClass(void);

  static ScXMLDocument * readFile(const char * filename);
  static ScXMLDocument * readBuffer(const SbByteBuffer & buffer);
  static ScXMLDocument * readXMLData(cc_xml_doc * xmldoc);

  ScXMLDocument(void);
  virtual ~ScXMLDocument(void);

  void setFilename(const char * filename);
  const char * getFilename(void) const;

  virtual void setRoot(ScXMLScxmlElt * root);
  ScXMLScxmlElt * getRoot(void) const;

  ScXMLAbstractStateElt * getStateById(SbName id) const;
  ScXMLDataElt * getDataById(SbName id) const;

protected:

private:
  ScXMLDocument(const ScXMLDocument & rhs); // N/A
  ScXMLDocument & operator = (const ScXMLDocument & rhs); // N/A

  class PImpl;
  SbPimplPtr<PImpl> pimpl;

}; // ScXMLDocument

#endif // COIN_SCXMLDOCUMENT_H
