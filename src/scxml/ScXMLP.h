#ifndef COIN_SCXMLP_H
#define COIN_SCXMLP_H

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

#include <map>

#include <Inventor/SoType.h>

struct cc_xml_doc;
struct cc_xml_elt;
class ScXMLObject;

class ScXMLP {
public:
  static void init(void);
  static void cleanup(void);

  static void registerClassType(const char * xmlns,
                                const char * classname, SoType type);

  static void registerInvokeClassType(const char * xmlns,
                                      const char * targettype,
                                      const char * source, SoType type);

  static ScXMLStateMachine * readXMLData(cc_xml_doc * doc);

protected:
  typedef std::map<const char *, SoType> TypeDict;
  typedef std::pair<const char *, SoType> TypeEntry;
  typedef std::map<const char *, TypeDict *> NamespaceDict;
  typedef std::pair<const char *, TypeDict *> NamespaceEntry;
  typedef std::map<const char *, NamespaceDict *> TargettypeDict;
  typedef std::pair<const char *, NamespaceDict *> TargettypeEntry;

  static NamespaceDict * namespaces;
  static TargettypeDict * targettypes;

  // toplevel getters
  static SoType getClassType(const char * xmlns, const char * classname);
  static SoType getInvokeClassType(const char * xmlns, const char * targettype,
                                   const char * source);

  // utilities
  static void registerClassType(NamespaceDict * nsdict, const char * xmlns,
                                const char * classname, SoType type);

  static NamespaceDict * getNamespaceDict(TargettypeDict * ttdict, const char * targettype);

  static TypeDict * getTypeDict(NamespaceDict * dict, const char * xmlns);

  static SoType getClassType(NamespaceDict * dict, const char * xmlns,
                             const char * classname);


  static ScXMLObject * readScXMLDocument(ScXMLObject * container, cc_xml_elt * elt, const char * xmlns);
  static ScXMLObject * readScXMLState(ScXMLObject * container, cc_xml_elt * elt, const char * xmlns, SbBool isparallel = FALSE);
  static ScXMLObject * readScXMLTransition(ScXMLObject * container, cc_xml_elt * elt, const char * xmlns);
  static ScXMLObject * readScXMLFinal(ScXMLObject * container, cc_xml_elt * elt, const char * xmlns);
  static ScXMLObject * readScXMLParallel(ScXMLObject * container, cc_xml_elt * elt, const char * xmlns);
  static ScXMLObject * readScXMLDatamodel(ScXMLObject * container, cc_xml_elt * elt, const char * xmlns);
  static ScXMLObject * readScXMLOnEntry(ScXMLObject * container, cc_xml_elt * elt, const char * xmlns);
  static ScXMLObject * readScXMLOnExit(ScXMLObject * container, cc_xml_elt * elt, const char * xmlns);
  static ScXMLObject * readScXMLInitial(ScXMLObject * container, cc_xml_elt * elt, const char * xmlns);
  static ScXMLObject * readScXMLHistory(ScXMLObject * container, cc_xml_elt * elt, const char * xmlns);
  static ScXMLObject * readScXMLAnchor(ScXMLObject * container, cc_xml_elt * elt, const char * xmlns);
  static ScXMLObject * readScXMLInvoke(ScXMLObject * container, cc_xml_elt * elt, const char * xmlns);

  static SbBool no_datamodel_warning;

private:
  static void cleanup_namespacedict(NamespaceDict * dict);
  static void cleanup_targettypes(void);
}; // ScXMLP

#endif // !COIN_SCXMLP_H
