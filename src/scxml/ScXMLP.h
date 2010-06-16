#ifndef COIN_SCXMLP_H
#define COIN_SCXMLP_H

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

#include <map>

#include <Inventor/SoType.h>

struct cc_xml_doc;
struct cc_xml_elt;
class ScXMLObject;
class ScXMLElt;

class ScXMLP {
public:
  static void init(void);
  static void cleanup(void);

  static void lock(void);
  static void unlock(void);

  static void registerClassType(const char * xmlns,
                                const char * classname, SoType type);

  static void registerInvokeClassType(const char * xmlns,
                                      const char * targettype,
                                      const char * source, SoType type);

  static ScXMLStateMachine * readXMLData(cc_xml_doc * doc);

  typedef std::map<const char *, SoType> TypeDict;
  typedef std::pair<const char *, SoType> TypeEntry;

protected:
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

public:
  static TypeDict * profileevaluators;

private:
  static void cleanup_namespacedict(NamespaceDict * dict);
  static void cleanup_targettypes(void);

  static void * mutex;
}; // ScXMLP

#endif // !COIN_SCXMLP_H
