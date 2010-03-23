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

#include <Inventor/scxml/ScXML.h>

/*!
  \class ScXML ScXML.h Inventor/scxml/ScXML.h
  \brief namespace for static ScXML-related functions.

  This is a static namespace class for ScXML-related functions.

  \since Coin 3.0
  \ingroup scxml
*/

#ifdef _MSC_VER
#pragma warning(disable:4786) // symbol truncated
#endif // _MSC_VER

#include <cassert>
#include <cstring>

#include <Inventor/SbName.h>
#include <Inventor/errors/SoDebugError.h>
#include <Inventor/C/XML/document.h>
#include <Inventor/C/XML/element.h>
#include <Inventor/C/XML/attribute.h>

#include <Inventor/misc/CoinResources.h>

#include <Inventor/scxml/ScXML.h>
#include <Inventor/scxml/ScXMLObject.h>
#include <Inventor/scxml/ScXMLEvent.h>
#include <Inventor/scxml/ScXMLEventTarget.h>
#include <Inventor/scxml/ScXMLStateMachine.h>
#include <Inventor/scxml/ScXMLDocument.h>

#include <Inventor/scxml/ScXMLElt.h>
#include <Inventor/scxml/ScXMLScxmlElt.h>
#include <Inventor/scxml/ScXMLInitialElt.h>
#include <Inventor/scxml/ScXMLAbstractStateElt.h>
#include <Inventor/scxml/ScXMLStateElt.h>
#include <Inventor/scxml/ScXMLParallelElt.h>
#include <Inventor/scxml/ScXMLFinalElt.h>
#include <Inventor/scxml/ScXMLOnEntryElt.h>
#include <Inventor/scxml/ScXMLOnExitElt.h>
#include <Inventor/scxml/ScXMLTransitionElt.h>
#include <Inventor/scxml/ScXMLHistoryElt.h>

#include <Inventor/scxml/ScXMLExecutableElt.h>
#include <Inventor/scxml/ScXMLEventElt.h>
#include <Inventor/scxml/ScXMLIfElt.h>
#include <Inventor/scxml/ScXMLElseIfElt.h>
#include <Inventor/scxml/ScXMLElseElt.h>
#include <Inventor/scxml/ScXMLLogElt.h>

#include <Inventor/scxml/ScXMLDataModelElt.h>
#include <Inventor/scxml/ScXMLDataElt.h>
#include <Inventor/scxml/ScXMLAssignElt.h>
#include <Inventor/scxml/ScXMLValidateElt.h>

#include <Inventor/scxml/ScXMLSendElt.h>
#include <Inventor/scxml/ScXMLInvokeElt.h>
#include <Inventor/scxml/ScXMLParamElt.h>
#include <Inventor/scxml/ScXMLFinalizeElt.h>
#include <Inventor/scxml/ScXMLContentElt.h>

#include <Inventor/scxml/ScXMLAnchorElt.h>

#include <Inventor/scxml/ScXMLEvaluator.h>
#include <Inventor/scxml/ScXMLMinimumEvaluator.h>
#include <Inventor/scxml/ScXMLXPathEvaluator.h>
#include <Inventor/scxml/ScXMLECMAScriptEvaluator.h>
#include <Inventor/scxml/ScXMLCoinEvaluator.h>

#include <Inventor/scxml/SoScXMLEvent.h>
#include <Inventor/scxml/SoScXMLStateMachine.h>

#include "tidbitsp.h"

#include "threads/threadsutilp.h"

#include "scxml/ScXMLP.h"

// *************************************************************************

/*!
  \page scxml State Chart XML

  The ScXML part of Coin is a basic, non-conformant, partial
  implementation of State Chart XML, based on the W3C Working Draft
  16 May 2008 of SCXML <http://www.w3.org/TR/2008/WD-scxml-20080516/>.
  The latest version should be at <http://www.w3.org/TR/scxml/>.
  Read that document for a basic understanding how SCXML documents
  should be constructed.

  Coin uses SCXML for setting up its navigation systems, to be able to
  remove hardcoded logic for user navigation and externalize it into XML
  files.  This makes it easy for users to customize the navigation
  system for their own purposes.

  Unsupported Items:

  - The &lt;parallel&gt; element is not supported as intended with
    parallel states.  Coin will just treat it as an ordinary
    &lt;state&gt; element for now.  Parallel states is not high up on
    the priority list, so expect this to be handled after a lot of
    other functionality is in place.

  - The &lt;datamodel&gt;-related part of the specification is not
    fully supported.

  - The 'target' attribute in the &lt;transition&gt; element can only
    identify a single state currently, not multiple as you would have
    to when having support for &lt;parallel&gt; elements (which we
    don't have).

  - The virtual state elements like &lt;history&gt; and &lt;anchor&gt;
    are just implemented as dummy states for now and do not do
    anything in relation to what they should actually do.

  For learning more about how ScXML is implemented and used in Coin,
  take a look at $COINDIR/scxml/navigation/examiner.xml (or in the
  Coin source directory, data/scxml/navigation/examiner.xml) for an
  example of how an SCXML system for camera navigation looks,
  and look at the source files in src/navigation/ for the C++ counterparts
  to the same SCXML navigation system.

  \ingroup scxml
  \since Coin 3.0
*/

#define SCXML_DEFAULT_NS "default" // to be deprecated

// *************************************************************************

// private static variables
ScXMLP::NamespaceDict * ScXMLP::namespaces = NULL;
ScXMLP::TargettypeDict * ScXMLP::targettypes = NULL;
ScXMLP::TypeDict * ScXMLP::profileevaluators = NULL;

// *************************************************************************

extern "C" {
static void scxml_cleanup(void);
}

/*!
  Initializes the basic ScXML classes.
*/
void
ScXML::initClasses(void)
{
  ScXMLP::init();
  ScXMLObject::initClass();
  ScXMLEvent::initClass();
  ScXMLEventTarget::initClass();
  ScXMLStateMachine::initClass();
  ScXMLDocument::initClass();

  ScXMLElt::initClass();
  ScXMLScxmlElt::initClass();
  ScXMLInitialElt::initClass();
  ScXMLAbstractStateElt::initClass();
  ScXMLStateElt::initClass();
  ScXMLParallelElt::initClass();
  ScXMLFinalElt::initClass();
  ScXMLOnEntryElt::initClass();
  ScXMLOnExitElt::initClass();
  ScXMLTransitionElt::initClass();
  ScXMLHistoryElt::initClass();

  ScXMLExecutableElt::initClass();
  ScXMLEventElt::initClass();
  ScXMLIfElt::initClass();
  ScXMLElseIfElt::initClass();
  ScXMLElseElt::initClass();
  ScXMLLogElt::initClass();

  ScXMLDataModelElt::initClass();
  ScXMLDataElt::initClass();
  ScXMLAssignElt::initClass();
  ScXMLValidateElt::initClass();

  ScXMLSendElt::initClass();
  ScXMLInvokeElt::initClass();
  ScXMLParamElt::initClass();
  ScXMLFinalizeElt::initClass();
  ScXMLContentElt::initClass();

  ScXMLAnchorElt::initClass();

  // evaluators also inits data-objs
  ScXMLEvaluator::initClass();
  ScXMLMinimumEvaluator::initClass();
  ScXMLXPathEvaluator::initClass();
  ScXMLECMAScriptEvaluator::initClass();
  ScXMLCoinEvaluator::initClass();

  SoScXMLEvent::initClass();
  SoScXMLStateMachine::initClass();

  coin_atexit(scxml_cleanup, CC_ATEXIT_NORMAL);
}

void
scxml_cleanup(void)
{
  ScXML::cleanClasses();
}

void
ScXML::cleanClasses(void)
{
  SoScXMLStateMachine::cleanClass();
  SoScXMLEvent::cleanClass();

  ScXMLCoinEvaluator::cleanClass();
  ScXMLECMAScriptEvaluator::cleanClass();
  ScXMLXPathEvaluator::cleanClass();
  ScXMLMinimumEvaluator::cleanClass();
  ScXMLEvaluator::cleanClass();

  ScXMLAnchorElt::cleanClass();

  ScXMLContentElt::cleanClass();
  ScXMLFinalizeElt::cleanClass();
  ScXMLParamElt::cleanClass();
  ScXMLInvokeElt::cleanClass();
  ScXMLSendElt::cleanClass();

  ScXMLValidateElt::cleanClass();
  ScXMLAssignElt::cleanClass();
  ScXMLDataElt::cleanClass();
  ScXMLDataModelElt::cleanClass();

  ScXMLLogElt::cleanClass();
  ScXMLElseElt::cleanClass();
  ScXMLElseIfElt::cleanClass();
  ScXMLIfElt::cleanClass();
  ScXMLEventElt::cleanClass();
  ScXMLExecutableElt::cleanClass();

  ScXMLHistoryElt::cleanClass();
  ScXMLTransitionElt::cleanClass();
  ScXMLOnExitElt::cleanClass();
  ScXMLOnEntryElt::cleanClass();
  ScXMLFinalElt::cleanClass();
  ScXMLParallelElt::cleanClass();
  ScXMLStateElt::cleanClass();
  ScXMLAbstractStateElt::cleanClass();
  ScXMLInitialElt::cleanClass();
  ScXMLScxmlElt::cleanClass();
  ScXMLElt::cleanClass();

  ScXMLDocument::cleanClass();
  ScXMLStateMachine::cleanClass();
  ScXMLEventTarget::cleanClass();
  ScXMLEvent::cleanClass();
  ScXMLObject::cleanClass();
  ScXMLP::cleanup();
}

// *************************************************************************

SbBool
ScXML::registerEvaluatorType(SbName profilename, SoType evaluatortype)
{
  assert(!evaluatortype.isBad());
  assert(ScXMLP::profileevaluators);
  if (!evaluatortype.isDerivedFrom(ScXMLEvaluator::getClassTypeId())) {
    SoDebugError::post("ScXMLStateMachine::registerEvaluator",
                       "Evaluator type must be derived from ScXMLEvaluator");
    return FALSE;
  }
  ScXMLP::TypeDict::iterator it = ScXMLP::profileevaluators->find(profilename.getString());
  if (it != ScXMLP::profileevaluators->end()) {
    SoDebugError::post("ScXML::registerEvaluatorType",
                       "Evaluator for profile '%s' already registered.\n",
                       profilename.getString());
    return FALSE;
  }
  ScXMLP::TypeEntry entry(profilename.getString(), evaluatortype);
  ScXMLP::profileevaluators->insert(entry);
  return TRUE;
}

SbBool
ScXML::unregisterEvaluatorType(SbName profilename, SoType evaluatortype)
{
  assert(!evaluatortype.isBad());
  assert(ScXMLP::profileevaluators);

  ScXMLP::TypeDict::iterator it = ScXMLP::profileevaluators->find(profilename.getString());
  if (it == ScXMLP::profileevaluators->end()) {
    SoDebugError::post("ScXML::unregisterEvaluatorType",
                       "No evaluator type is registered for profile '%s'.\n",
                       profilename.getString());
    return FALSE;
  }
  if (it->second != evaluatortype) {
    SoDebugError::post("ScXML::unregisterEvaluatorType",
                       "Different evaluator type registered for profile '%s'.\n",
                       profilename.getString());
    return FALSE;
  }
  ScXMLP::profileevaluators->erase(it);
  return TRUE;
}

SoType
ScXML::getEvaluatorTypeForProfile(SbName profilename)
{
  assert(ScXMLP::profileevaluators);

  ScXMLP::TypeDict::iterator it = ScXMLP::profileevaluators->find(profilename.getString());
  if (it == ScXMLP::profileevaluators->end()) {
    // no type registered for profile
    return SoType::badType();
  }
  return it->second;
}

// *************************************************************************
// SCXML FILE INPUT
// *************************************************************************

/*!
  This function reads in an SCXML document from a file.

  \returns an ScXMLDocument-derived statemachine object hierarchy.
*/
ScXMLStateMachine *
ScXML::readFile(const char * filename)
{
  ScXMLDocument * doc = ScXMLDocument::readFile(filename);
  if (!doc) {
    return NULL;
  }

  ScXMLStateMachine * statemachine = new SoScXMLStateMachine;
  statemachine->setDescription(doc);
  statemachine->setName(filename);

  return statemachine;
}

/*!
  This function reads in an SCXML document residing in memory.

  \returns an ScXMLDocument-derived statemachine object hierarchy.
*/
ScXMLStateMachine *
ScXML::readBuffer(const SbByteBuffer & bufferdata)
{
  ScXMLDocument * doc = ScXMLDocument::readBuffer(bufferdata);
  if (!doc) {
    return NULL;
  }

  ScXMLStateMachine * statemachine = new SoScXMLStateMachine;
  statemachine->setDescription(doc);
  statemachine->setName("<memory buffer>");

  return statemachine;
}

// *************************************************************************
// TYPE SYSTEM REGISTERING AND LOOKUP
// *************************************************************************

/*
*/
void
ScXMLP::registerClassType(const char * xmlns, const char * classname, SoType type)
{
  assert(!type.isBad());
  ScXMLP::registerClassType(ScXMLP::namespaces, xmlns, classname, type);
}

/*
*/
void
ScXMLP::registerInvokeClassType(const char * xmlns, const char * targettype, const char * source, SoType type)
{
  assert(!type.isBad());
  assert(xmlns);

  // Note that we actually split on targettype on the top of the
  // hierarchy/tree here, and not on xml-namespace as one might think
  // at first, but this is intentional. This is because the namespace
  // logic should fall through to the default namespace if you don't
  // find the type you are looking for in the specified namespace, and
  // because of that, it is most convenient to split on namespace at
  // the next-to-bottom level to be able to share code when looking up
  // types.

  ScXMLP::NamespaceDict * nsdict = ScXMLP::getNamespaceDict(ScXMLP::targettypes, targettype);
  assert(nsdict);

  ScXMLP::registerClassType(nsdict, xmlns, source, type);
}

/*
*/
void
ScXMLP::registerClassType(NamespaceDict * nsdict, const char * xmlns, const char * classname, SoType type)
{
  assert(xmlns);

  TypeDict * typedict = ScXMLP::getTypeDict(nsdict, xmlns);
  assert(typedict);

  SbName classnamename(classname);
  const char * key = classnamename.getString();
  TypeDict::iterator it = typedict->find(key);
  if (it != typedict->end()) {
    it->second = type;
  } else {
    typedict->insert(TypeEntry(key, type));
  }
}

/*
*/
ScXMLP::TypeDict *
ScXMLP::getTypeDict(NamespaceDict * nsdict, const char * xmlns)
{
  SbName xmlnsname(xmlns); // uniqify on string pointer
  const char * key = xmlnsname.getString();
  NamespaceDict::iterator it = nsdict->find(key);
  if (it != nsdict->end()) {
    return it->second;
  } else {
    TypeDict * dict = new TypeDict;
    nsdict->insert(NamespaceEntry(key, dict));
    return dict;
  }
}

// *************************************************************************
/*
*/
SoType
ScXMLP::getClassType(NamespaceDict * nsdict, const char * xmlns, const char * classname)
{
  assert(xmlns && classname);

  const SbName classnamename(classname);
  const char * key = classnamename.getString();

  ScXMLP::TypeDict * typedict = NULL;

  typedict = ScXMLP::getTypeDict(nsdict, xmlns);
  if (typedict) {
    TypeDict::iterator it = typedict->find(key);
    if (it != typedict->end()) {
      return it->second;
    }
  }

  typedict = ScXMLP::getTypeDict(nsdict, SCXML_DEFAULT_NS);
  if (typedict) {
    TypeDict::iterator it = typedict->find(key);
    if (it != typedict->end()) {
      return it->second;
    }
  }

  return SoType::badType();
}

/*
*/
ScXMLP::NamespaceDict *
ScXMLP::getNamespaceDict(TargettypeDict * ttdict, const char * targettype)
{
  SbName targettypename(targettype); // uniqify on string pointer
  const char * key = targettypename.getString();
  TargettypeDict::iterator it = ttdict->find(key);
  if (it != ttdict->end()) {
    return it->second;
  } else {
    NamespaceDict * dict = new NamespaceDict;
    ttdict->insert(TargettypeEntry(key, dict));
    return dict;
  }
}

/*
*/
SoType
ScXMLP::getClassType(const char * xmlns, const char * classname)
{
  return ScXMLP::getClassType(ScXMLP::namespaces, xmlns, classname);
}

SoType
ScXMLP::getInvokeClassType(const char * xmlns, const char * targettype, const char * source)
{
  NamespaceDict * nsdict = ScXMLP::getNamespaceDict(ScXMLP::targettypes, targettype);
  assert(nsdict);

  SoType invoketype = ScXMLP::getClassType(nsdict, xmlns, source);

#if 0
  if (invoketype == SoType::badType()) {
    return ScXMLFallbackInvoke::getClassTypeId();
  }
#endif

  return invoketype;
}

// *************************************************************************

// internal implementation
ScXMLStateMachine *
ScXMLP::readXMLData(cc_xml_doc * doc)
{
  const char * xmlns = SCXML_DEFAULT_NS;

  // get a handle on the root element
  cc_xml_elt * root = cc_xml_doc_get_root(doc);
  if (strcmp(cc_xml_elt_get_type(root), "scxml") != 0) {
    return NULL;
  }

  // peek at xmlns attribute to get the correct object types instantiated
  const cc_xml_attr * nsattr = cc_xml_elt_get_attribute(root, "xmlns");
  if (nsattr) {
    xmlns = cc_xml_attr_get_value(nsattr);
    assert(xmlns);
  }

  SoType statemachinetype = ScXMLP::getClassType(xmlns, "statemachine");
  assert(statemachinetype.canCreateInstance());
  ScXMLStateMachine * statemachine =
    static_cast<ScXMLStateMachine *>(statemachinetype.createInstance());
  assert(statemachine && statemachine->isOfType(ScXMLStateMachine::getClassTypeId()));

  // FIXME
  ScXMLObject * documentobj = NULL;
  // ScXMLObject * documentobj = ScXMLP::readScXMLDocument(statemachine, root, xmlns);
  assert(documentobj && documentobj->isOfType(ScXMLDocument::getClassTypeId()));
  ScXMLDocument * document = static_cast<ScXMLDocument *>(documentobj);

  statemachine->setDescription(document);

  return statemachine;
}

// *************************************************************************

void * ScXMLP::mutex = NULL;

void
ScXMLP::init(void)
{
  ScXMLP::namespaces = new ScXMLP::NamespaceDict;
  ScXMLP::targettypes = new ScXMLP::TargettypeDict;
  ScXMLP::profileevaluators = new ScXMLP::TypeDict;
  CC_MUTEX_CONSTRUCT(ScXMLP::mutex);
}

void
ScXMLP::cleanup(void)
{
  cleanup_namespacedict(ScXMLP::namespaces);
  delete ScXMLP::namespaces;
  ScXMLP::namespaces = NULL;
  delete ScXMLP::profileevaluators;
  ScXMLP::profileevaluators = NULL;
  cleanup_targettypes();
  CC_MUTEX_DESTRUCT(ScXMLP::mutex);
}

void
ScXMLP::lock(void)
{
  assert(ScXMLP::mutex);
  CC_MUTEX_LOCK(ScXMLP::mutex);
}

void
ScXMLP::unlock(void)
{
  assert(ScXMLP::mutex);
  CC_MUTEX_UNLOCK(ScXMLP::mutex);
}


void
ScXMLP::cleanup_namespacedict(NamespaceDict * dict)
{
  assert(dict);
  NamespaceDict::iterator it = dict->begin();
  while (it != dict->end()) {
    delete it->second;
    it++;
  }
}

void
ScXMLP::cleanup_targettypes(void)
{
  assert(ScXMLP::targettypes);
  TargettypeDict::iterator it = ScXMLP::targettypes->begin();
  while (it != ScXMLP::targettypes->end()) {
    ScXMLP::NamespaceDict * dict = it->second;
    cleanup_namespacedict(dict);
    delete dict;
    it++;
  }
  delete ScXMLP::targettypes;
  ScXMLP::targettypes = NULL;
}

// *************************************************************************
