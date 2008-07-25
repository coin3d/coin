/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2008 by Kongsberg SIM.  All rights reserved.
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

#include <Inventor/scxml/ScXML.h>

#include <cassert>
#include <cstring>

#include <Inventor/SbName.h>
#include <Inventor/errors/SoDebugError.h>
#include <Inventor/C/XML/document.h>
#include <Inventor/C/XML/element.h>
#include <Inventor/C/XML/attribute.h>

#include <Inventor/scxml/ScXMLObject.h>
#include <Inventor/scxml/ScXMLEvent.h>
#include <Inventor/scxml/ScXMLInvoke.h>
#include <Inventor/scxml/ScXMLStateMachine.h>
#include <Inventor/scxml/SoScXMLEvent.h>
#include <Inventor/scxml/SoScXMLStateMachine.h>

#include "scxml/ScXMLDocument.h"
#include "scxml/ScXMLState.h"
#include "scxml/ScXMLInitial.h"
#include "scxml/ScXMLFinal.h"
#include "scxml/ScXMLTransition.h"
#include "scxml/ScXMLOnEntry.h"
#include "scxml/ScXMLOnExit.h"
#include "scxml/ScXMLAnchor.h"
#include "scxml/ScXMLHistory.h"
#include "tidbitsp.h"

#include "misc/CoinResources.h"

#include "scxml/ScXMLP.h"
#include "scxml/ScXMLFallbackInvoke.h"

#include "scxml/SoScXMLDebugInfoInvoke.h"
#include "scxml/SoScXMLScheduleRedrawInvoke.h"
#include "scxml/SoScXMLScheduleTimerEventInvoke.h"

// *************************************************************************

/*!
  \page scxml State Chart XML

  The ScXML part of Coin is a basic, non-conformant, partial
  implementation of State Chart XML, based on the W3C Working Draft 21
  February 2007 of SCXML <http://www.w3.org/TR/2007/WD-scxml-20070221/>.  
  Read that document for a basic understanding how SCXML documents
  should be constructed.

  NOTE: A new version of the draft has just been released (May 2008).
  The code here does not reflect any changes done to the draft SCXML
  specification yet.

  Coin uses it for its navigation system, to be able to remove
  hardcoded logic for user navigation and externalize it into XML
  files.  It is hoped that this technology will prove to be a useful
  addition to Coin and get broader use as this subsystem matures.  The
  dragger and manipulator user interaction logic is likely one area
  where ScXML might be employed later. Attaching state machines to
  interactive parts of the scene graph could be another interesting
  idea for the future.

  For now, the ScXML system is not employed in any pre-established
  Coin usage patterns, and you will need to employ new features to touch
  base with it. The most obvious are where ScXML will be used is if
  you use Quarter 1.0 over Coin 3.0, as you manipulate the camera
  movements around the 3D models through mouse and keyboard interaction.

  The Coin type system makes it possible to override the default types
  to have the state chart description instantiated using customized
  objects. A design choice made in that regard was to both use the
  type overriding system in Coin, and also to use the 'xmlns'
  (XML Namespace) attribute to make ScXML types available and to
  prioritize which classtypes to instantiate objects from.  The xmlns
  attribute is currently only checked at the document level, but the
  plan is to let any state have this attribute, and to simulate
  static scoping rules for namespace prioritization.

  Partially Supported Items:

  - At the moment, the Coin ScXML module only supports "Executable
    Content" through the &lt;invoke&gt; element, using the ScXMLInvoke
    class interface and having the Executable Content written in C++
    in subclasses of the ScXMLInvoke class. ECMAScript and XPath is
    not supported yet.

  Unsupported Items:

  - External document referencing through the &lt;state&gt; /
    &lt;parallel&gt; 'src' attribute is not yet handled, but should be
    easy to implement so it will likely be one of the first things
    that will be fixed.

  - The &lt;parallel&gt; element is not supported as intended with
    parallel states.  Coin will just treat it as an ordinary
    &lt;state&gt; element for now.  Parallel states is not high up on
    the priority list, so expect this to be handled after a lot of
    other functionality is in place.

  - The &lt;datamodel&gt;-related part of the specification is not
    supported.

  - The condition attribute in the &lt;transition&gt; element is not
    supported, so the only condition you can set is on the event type
    in the event attribute.  The ScXMLTransition class has a virtual
    method
      SbBool evaluateCondition()
    which is used in the state machine logic, and should be all that is
    needed to overload to implement this in subclasses yourself.

  - The 'target' attribute in the &lt;transition&gt; element can only
    identify a single state currently, not multiple as you would have
    to when having support for &lt;parallel&gt; elements (which we
    don't have).

  - The virtual state elements like &lt;history&gt; and &lt;anchor&gt;
    are just implemented as dummy states for now and do not do
    anything in relation to what they should actually do.

  - There are no mechanisms for inter-statemachine event passing yet.

  For learning more about how ScXML is implemented and used in Coin,
  take a look at $COINDIR/scxml/navigation/examiner.xml (or in the
  Coin source directory, data/scxml/navigation/examiner.xml) for an
  example of how an SCXML system for camera navigation looks,
  and look at the ScXML* source files in src/navigation/ for the C++
  counterparts to the same SCXML navigation system.

  With support for &lt;datamodel&gt;, &lt;transition&gt;-conditions,
  and inline executable content in the XML file in some scripting
  language, the C++ parts could probably have been greatly simplified,
  if not more or less eliminated.  This will hopefully evolve for
  future versions of Coin.

  \ingroup scxml
  \since Coin 3.0
*/

// *************************************************************************

// private static variables
ScXMLP::NamespaceDict *ScXMLP::namespaces;
ScXMLP::TargettypeDict *ScXMLP::targettypes;

SbBool ScXMLP::no_datamodel_warning = FALSE;

// *************************************************************************

/*!
  \class ScXML ScXML.h Inventor/scxml/ScXML.h
  \brief Namespace for static ScXML-related functions.

  This is a static namespace class for ScXML-related functions.

  \ingroup scxml
  \since Coin 3.0
*/

/*!
  Initializes the basic ScXML classes.
*/
void
ScXML::initClasses(void)
{
  ScXMLP::init();
  ScXMLObject::initClass();
  ScXMLDocument::initClass();
  ScXMLState::initClass();
  ScXMLInitial::initClass();
  ScXMLFinal::initClass();
  ScXMLTransition::initClass();
  ScXMLOnEntry::initClass();
  ScXMLOnExit::initClass();
  ScXMLEvent::initClass();
  ScXMLInvoke::initClass();
  ScXMLFallbackInvoke::initClass();
  ScXMLStateMachine::initClass();
  ScXMLAnchor::initClass();
  ScXMLHistory::initClass();

  SoScXMLEvent::initClass();
  SoScXMLStateMachine::initClass();

  SoScXMLDebugInfoInvoke::initClass();
  SoScXMLScheduleRedrawInvoke::initClass();
  SoScXMLScheduleTimerEventInvoke::initClass();
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
  if (strncmp(filename, "coin:", 5) == 0) { // is a "resource"
    const char * buffer = NULL;
    size_t buffersize = 0;
    if (CoinResources::get(filename, buffer, buffersize)) {
      return ScXML::readBuffer(buffer);
    }
  }

  cc_xml_doc * doc = cc_xml_doc_new();
  if (!cc_xml_doc_read_file_x(doc, filename)) {
    cc_xml_doc_delete_x(doc);
    return NULL;
  }

  ScXMLStateMachine * statemachine = ScXMLP::readXMLData(doc);
  cc_xml_doc_delete_x(doc);

  if (statemachine) {
    statemachine->setName(filename);
  }

  return statemachine;
}

/*!
  This function reads in an SCXML document residing in memory.

  \returns an ScXMLDocument-derived statemachine object hierarchy.
*/
ScXMLStateMachine *
ScXML::readBuffer(const char * bufferdata)
{
  cc_xml_doc * doc = cc_xml_doc_new();
  if (!cc_xml_doc_read_buffer_x(doc, bufferdata, strlen(bufferdata))) {
    cc_xml_doc_delete_x(doc);
    return NULL;
  }

  ScXMLStateMachine * statemachine = ScXMLP::readXMLData(doc);
  cc_xml_doc_delete_x(doc);
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

  if (invoketype == SoType::badType()) {
    return ScXMLFallbackInvoke::getClassTypeId();
  }

  return invoketype;
}

// *************************************************************************

// internal implementation
ScXMLStateMachine *
ScXMLP::readXMLData(cc_xml_doc * doc)
{
  ScXMLP::no_datamodel_warning = FALSE;

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

  ScXMLObject * documentobj = ScXMLP::readScXMLDocument(statemachine, root, xmlns);
  assert(documentobj && documentobj->isOfType(ScXMLDocument::getClassTypeId()));
  ScXMLDocument * document = static_cast<ScXMLDocument *>(documentobj);

  statemachine->setDescription(document);

  return statemachine;
}

// *************************************************************************

// internal
ScXMLObject *
ScXMLP::readScXMLDocument(ScXMLObject * container, cc_xml_elt * elt, const char * xmlns)
{
  assert(elt && xmlns);

  // instantiate root ScXMLDocument object
  SoType documenttype = ScXMLP::getClassType(xmlns, "scxml");
  assert(documenttype.canCreateInstance());
  ScXMLDocument * document = static_cast<ScXMLDocument *>(documenttype.createInstance());

  // handle XML attributes
  assert(document && document->isOfType(ScXMLDocument::getClassTypeId()));
  const int numattrs = cc_xml_elt_get_num_attributes(elt);
  const cc_xml_attr ** attrs = cc_xml_elt_get_attributes(elt);
  int c;
  for (c = 0; c < numattrs; ++c) {
    const char * attrname = cc_xml_attr_get_name(attrs[c]);
    const char * attrvalue = cc_xml_attr_get_value(attrs[c]);
    document->setXMLAttribute(attrname, attrvalue);
  }
  if (!document->handleXMLAttributes()) {
    SoDebugError::postInfo("ScXML::readFile", "error in handleXMLAttributes()");
    delete document;
    return NULL;
  }
  document->setContainer(container);

  // read in all children, and recurse down
  const int numchildren = cc_xml_elt_get_num_children(elt);
  for (c = 0; c < numchildren; ++c) {
    cc_xml_elt * element = cc_xml_elt_get_child(elt, c);
    const char * elementtype = cc_xml_elt_get_type(element);
    if (strcmp(elementtype, COIN_XML_CDATA_TYPE) == 0) continue;
    // <state> - zero or more times
    if (strcmp(elementtype, "state") == 0) {
      ScXMLObject * stateobj = ScXMLP::readScXMLState(document, element, xmlns);
      if (stateobj) {
        assert(stateobj->isOfType(ScXMLState::getClassTypeId()));
        document->addState(static_cast<ScXMLState *>(stateobj));
      } else {
        SoDebugError::post("ScXML::readFile", "error reading <state> element");
        delete document;
        return NULL;
      }
    }
    // <parallel> - zero or more times
    else if (strcmp(elementtype, "parallel") == 0) {
      ScXMLObject * parallelobj = ScXMLP::readScXMLParallel(document, element, xmlns);
      if (parallelobj) {
        assert(parallelobj->isOfType(ScXMLState::getClassTypeId()));
        document->addParallel(static_cast<ScXMLState *>(parallelobj));
      } else {
        SoDebugError::post("ScXML::readFile", "error reading <parallel> element");
        delete document;
        return NULL;
      }
    }
    // <final> - one or more times
    else if (strcmp(elementtype, "final") == 0) {
      ScXMLObject * finalobj = ScXMLP::readScXMLFinal(document, element, xmlns);
      if (finalobj) {
        assert(finalobj->isOfType(ScXMLFinal::getClassTypeId()));
        document->addFinal(static_cast<ScXMLFinal *>(finalobj));
      } else {
        SoDebugError::post("ScXML::readFile", "error reading <final> element");
        delete document;
        return NULL;
      }
    }
    // <datamodel> - zero or one time
    else if (strcmp(elementtype, "datamodel") == 0) {
      ScXMLObject * datamodelobj = ScXMLP::readScXMLDatamodel(document, element, xmlns);
      if (datamodelobj) {
        // document->addDatamodel(datamodelobj);
      } else {
        SoDebugError::post("ScXML::readFile", "error reading <datamodel> element");
        delete document;
        return NULL;
      }
    }
    else {
      SoDebugError::post("ScXML::readFile", "unexpected XML element '<%s>' found in <scxml>",
                         elementtype);
      delete document;
      return NULL;
    }
  }

  return document;
} // readFile

// *************************************************************************

// internal - reads in the base state object rooted at 'elt' and recurses for children
ScXMLObject *
ScXMLP::readScXMLState(ScXMLObject * container, cc_xml_elt * elt, const char * xmlns, SbBool isparallel)
{
  // thisw function is multi-purpose, and reads in both <state> and <parallel> elements
  assert(container && elt && xmlns);
  SoType statetype = ScXMLP::getClassType(xmlns, "state");
  assert(statetype.canCreateInstance());
  ScXMLState * state = static_cast<ScXMLState *>(statetype.createInstance());
  assert(state && state->isOfType(ScXMLState::getClassTypeId()));
  state->setIsParallel(isparallel);
  const int numattrs = cc_xml_elt_get_num_attributes(elt);
  const cc_xml_attr ** attrs = cc_xml_elt_get_attributes(elt);
  int c;
  for (c = 0; c < numattrs; ++c) {
    const char * attrname = cc_xml_attr_get_name(attrs[c]);
    const char * attrvalue = cc_xml_attr_get_value(attrs[c]);
    state->setXMLAttribute(attrname, attrvalue);
  }
  if (!state->handleXMLAttributes()) {
    SoDebugError::postInfo("ScXML::readFile", "error in handleXMLAttributes()");
    delete state;
    return NULL;
  }
  state->setContainer(container);

  const int numchildren = cc_xml_elt_get_num_children(elt);
  for (c = 0; c < numchildren; ++c) {
    cc_xml_elt * element = cc_xml_elt_get_child(elt, c);
    const char * elementtype = cc_xml_elt_get_type(element);
    if (strcmp(elementtype, COIN_XML_CDATA_TYPE) == 0) continue;
    // <onentry> - zero or one time
    if (strcmp(elementtype, "onentry") == 0) {
      ScXMLObject * onentryobj = ScXMLP::readScXMLOnEntry(state, element, xmlns);
      if (onentryobj) {
        assert(onentryobj->isOfType(ScXMLOnEntry::getClassTypeId()));
        state->setOnEntry(static_cast<ScXMLOnEntry *>(onentryobj));
      } else {
        SoDebugError::post("ScXML::readFile", "error reading <%s> element", elementtype);
        delete state;
        return NULL;
      }
    }
    // <onexit> - zero or one time
    else if (strcmp(elementtype, "onexit") == 0) {
      ScXMLObject * onexitobj = ScXMLP::readScXMLOnExit(state, element, xmlns);
      if (onexitobj) {
        assert(onexitobj->isOfType(ScXMLOnExit::getClassTypeId()));
        state->setOnExit(static_cast<ScXMLOnExit *>(onexitobj));
      } else {
        SoDebugError::post("ScXML::readFile", "error reading <%s> element", elementtype);
        delete state;
        return NULL;
      }
    }
    // <transition> - zero or more times
    else if (!isparallel && strcmp(elementtype, "transition") == 0) {
      ScXMLObject * transitionobj = ScXMLP::readScXMLTransition(state, element, xmlns);
      if (transitionobj) {
        assert(transitionobj->isOfType(ScXMLTransition::getClassTypeId()));
        state->addTransition(static_cast<ScXMLTransition *>(transitionobj));
      } else {
        SoDebugError::post("ScXML::readFile", "error reading <%s> element", elementtype);
        delete state;
        return NULL;
      }
    }
    // <initial> - must occur iff states+parallels >= 1
    else if (!isparallel && strcmp(elementtype, "initial") == 0) {
      ScXMLObject * initialobj = ScXMLP::readScXMLInitial(state, element, xmlns);
      if (initialobj) {
        assert(initialobj->isOfType(ScXMLInitial::getClassTypeId()));
        state->setInitial(static_cast<ScXMLInitial *>(initialobj));
      } else {
        SoDebugError::post("ScXML::readFile", "error reading <%s> element", elementtype);
        delete state;
        return NULL;
      }
    }
    // <state> - zero or more times
    else if (strcmp(elementtype, "state") == 0) {
      ScXMLObject * stateobj = ScXMLP::readScXMLState(state, element, xmlns);
      if (stateobj) {
        assert(stateobj->isOfType(ScXMLState::getClassTypeId()));
        state->addState(static_cast<ScXMLState *>(stateobj));
      } else {
        SoDebugError::post("ScXML::readFile", "error reading <%s> element", elementtype);
        delete state;
        return NULL;
      }
    }
    // <parallel> - zero or more times
    else if (strcmp(elementtype, "parallel") == 0) {
      ScXMLObject * parallelobj = ScXMLP::readScXMLParallel(state, element, xmlns);
      if (parallelobj) {
        assert(parallelobj->isOfType(ScXMLState::getClassTypeId()));
        state->addParallel(static_cast<ScXMLState *>(parallelobj));
      } else {
        SoDebugError::post("ScXML::readFile", "error reading <%s> element", elementtype);
        delete state;
        return NULL;
      }
    }
    // <final> - zero or more times
    else if (!isparallel && strcmp(elementtype, "final") == 0) {
      ScXMLObject * finalobj = ScXMLP::readScXMLFinal(state, element, xmlns);
      if (finalobj) {
        assert(finalobj->isOfType(ScXMLFinal::getClassTypeId()));
        state->addFinal(static_cast<ScXMLFinal *>(finalobj));
      } else {
        SoDebugError::post("ScXML::readFile", "error reading <%s> element", elementtype);
        delete state;
        return NULL;
      }
    }
    // <history> - zero or more times
    else if (strcmp(elementtype, "history") == 0) {
      ScXMLObject * historyobj = ScXMLP::readScXMLHistory(state, element, xmlns);
      if (historyobj) {
        assert(historyobj->isOfType(ScXMLHistory::getClassTypeId()));
        state->addHistory(static_cast<ScXMLHistory *>(historyobj));
      } else {
        SoDebugError::post("ScXML::readFile", "error reading <%s> element", elementtype);
        delete state;
        return NULL;
      }
    }
    // <anchor> - zero or more times
    else if (strcmp(elementtype, "anchor") == 0) {
      ScXMLObject * anchorobj = ScXMLP::readScXMLAnchor(state, element, xmlns);
      if (anchorobj) {
        assert(anchorobj->isOfType(ScXMLAnchor::getClassTypeId()));
        state->addAnchor(static_cast<ScXMLAnchor *>(anchorobj));
      } else {
        SoDebugError::post("ScXML::readFile", "error reading <%s> element", elementtype);
        delete state;
        return NULL;
      }
    }
    // <datamodel> - zero or one time
    else if (strcmp(elementtype, "datamodel") == 0) {
      ScXMLObject * datamodelobj = ScXMLP::readScXMLDatamodel(state, element, xmlns);
      if (datamodelobj) {
        //assert(datamodelobj->isOfType(ScXMLDatamodel::getClassTypeId()));
        //state->addDatamodel(static_cast<ScXMLDatamodel *>(datamodelobj));
      } else {
        SoDebugError::post("ScXML::readFile", "error reading <%s> element", elementtype);
        delete state;
        return NULL;
      }
    }
    // <invoke> - one time iff states+parallel == 0
    else if (!isparallel && strcmp(elementtype, "invoke") == 0) {
      ScXMLObject * invokeobj = ScXMLP::readScXMLInvoke(state, element, xmlns);
      if (invokeobj) {
        assert(invokeobj->isOfType(ScXMLInvoke::getClassTypeId()));
        state->setInvoke(static_cast<ScXMLInvoke *>(invokeobj));
      } else {
        SoDebugError::post("ScXML::readFile", "error reading <%s> element", elementtype);
        delete state;
        return NULL;
      }
    }
    else {
      SoDebugError::post("ScXML::readFile",
                         "unexpected XML element '<%s>' found in <%s>",
                         elementtype, isparallel ? "parallel" : "state");
      delete state;
      return NULL;
    }
  }

  return state;
} // readScXMLState

// *************************************************************************

// internal
ScXMLObject *
ScXMLP::readScXMLParallel(ScXMLObject * container, cc_xml_elt * elt, const char * xmlns)
{
  return ScXMLP::readScXMLState(container, elt, xmlns, TRUE);
} // readScXMLParallel

// *************************************************************************

// internal
ScXMLObject *
ScXMLP::readScXMLTransition(ScXMLObject * container, cc_xml_elt * elt, const char * xmlns)
{
  assert(container && elt && xmlns);
  SoType transitiontype = ScXMLP::getClassType(xmlns, "transition");
  assert(transitiontype.canCreateInstance());
  ScXMLTransition * transition = static_cast<ScXMLTransition *>(transitiontype.createInstance());
  assert(transition && transition->isOfType(ScXMLTransition::getClassTypeId()));
  const int numattrs = cc_xml_elt_get_num_attributes(elt);
  const cc_xml_attr ** attrs = cc_xml_elt_get_attributes(elt);
  int c;
  for (c = 0; c < numattrs; ++c) {
    const char * attrname = cc_xml_attr_get_name(attrs[c]);
    const char * attrvalue = cc_xml_attr_get_value(attrs[c]);
    transition->setXMLAttribute(attrname, attrvalue);
  }
  if (!transition->handleXMLAttributes()) {
    SoDebugError::postInfo("ScXML::readFile", "error in handleXMLAttributes()");
    delete transition;
    return NULL;
  }
  transition->setContainer(container);

  const int numchildren = cc_xml_elt_get_num_children(elt);
  for (c = 0; c < numchildren; ++c) {
    cc_xml_elt * element = cc_xml_elt_get_child(elt, c);
    const char * elementtype = cc_xml_elt_get_type(element);
    if (strcmp(elementtype, COIN_XML_CDATA_TYPE) == 0) continue;
    // only executable content

    if (strcmp(elementtype, "invoke") == 0) {
      ScXMLObject * invokeobj = ScXMLP::readScXMLInvoke(transition, element, xmlns);
      if (invokeobj) {
        assert(invokeobj->isOfType(ScXMLInvoke::getClassTypeId()));
        transition->addInvoke(static_cast<ScXMLInvoke *>(invokeobj));
      } else {
        SoDebugError::post("ScXML::readFile", "error reading <%s> element",
                           elementtype);
        delete transition;
        return NULL;
      }
    }
    else {
      SoDebugError::post("ScXML::readFile",
                         "unexpected XML element '<%s>' found in <transition>",
                         elementtype);
      delete transition;
      return NULL;
    }

  }
  return transition;
} // readScXMLTransition

// *************************************************************************

// internal
ScXMLObject *
ScXMLP::readScXMLInitial(ScXMLObject * container, cc_xml_elt * elt, const char * xmlns)
{
  assert(container && elt && xmlns);

  SoType initialtype = ScXMLP::getClassType(xmlns, "initial");
  assert(initialtype.canCreateInstance());
  ScXMLInitial * initial = static_cast<ScXMLInitial *>(initialtype.createInstance());
  assert(initial && initial->isOfType(ScXMLInitial::getClassTypeId()));
  const int numattrs = cc_xml_elt_get_num_attributes(elt);
  const cc_xml_attr ** attrs = cc_xml_elt_get_attributes(elt);
  int c;
  for (c = 0; c < numattrs; ++c) {
    const char * attrname = cc_xml_attr_get_name(attrs[c]);
    const char * attrvalue = cc_xml_attr_get_value(attrs[c]);
    initial->setXMLAttribute(attrname, attrvalue);
  }
  if (!initial->handleXMLAttributes()) {
    SoDebugError::postInfo("ScXML::readFile", "error in handleXMLAttributes()");
    delete initial;
    return NULL;
  }
  initial->setContainer(container);

  const int numchildren = cc_xml_elt_get_num_children(elt);
  for (c = 0; c < numchildren; ++c) {
    cc_xml_elt * element = cc_xml_elt_get_child(elt, c);
    const char * elementtype = cc_xml_elt_get_type(element);
    if (strcmp(elementtype, COIN_XML_CDATA_TYPE) == 0) continue;

    // <transition>
    if (strcmp(elementtype, "transition") == 0) {
      ScXMLObject * transitionobj = ScXMLP::readScXMLTransition(initial, element, xmlns);
      if (transitionobj) {
        assert(transitionobj->isOfType(ScXMLTransition::getClassTypeId()));
        assert(initial->getTransition() == NULL);
        initial->setTransition(static_cast<ScXMLTransition *>(transitionobj));
      } else {
        SoDebugError::post("ScXML::readFile", "error reading <%s> element",
                           elementtype);
        delete initial;
        return NULL;
      }
    }
    else {
      SoDebugError::post("ScXML::readFile",
                         "unexpected XML element '<%s>' found in <initial>",
                         elementtype);
      delete initial;
      return NULL;
    }
  }

  return initial;
} // readScXMLInitial

// *************************************************************************

// internal
ScXMLObject *
ScXMLP::readScXMLFinal(ScXMLObject * container, cc_xml_elt * elt, const char * xmlns)
{
  assert(container && elt && xmlns);

  SoType finaltype = ScXMLP::getClassType(xmlns, "final");
  assert(finaltype.canCreateInstance());
  ScXMLFinal * final = static_cast<ScXMLFinal *>(finaltype.createInstance());
  assert(final && final->isOfType(ScXMLFinal::getClassTypeId()));
  const int numattrs = cc_xml_elt_get_num_attributes(elt);
  const cc_xml_attr ** attrs = cc_xml_elt_get_attributes(elt);
  int c;
  for (c = 0; c < numattrs; ++c) {
    const char * attrname = cc_xml_attr_get_name(attrs[c]);
    const char * attrvalue = cc_xml_attr_get_value(attrs[c]);
    final->setXMLAttribute(attrname, attrvalue);
  }
  if (!final->handleXMLAttributes()) {
    SoDebugError::postInfo("ScXML::readFile", "error in handleXMLAttributes()");
    delete final;
    return NULL;
  }
  final->setContainer(container);

  const int numchildren = cc_xml_elt_get_num_children(elt);
  for (c = 0; c < numchildren; ++c) {
    cc_xml_elt * element = cc_xml_elt_get_child(elt, c);
    const char * elementtype = cc_xml_elt_get_type(element);
    if (strcmp(elementtype, COIN_XML_CDATA_TYPE) == 0) continue;

    SoDebugError::post("ScXML::readFile",
                       "unexpected XML element '<%s>' found in <final>",
                       elementtype);
    delete final;
    return NULL;
  }

  return final;
} // readScXMLFinal

// *************************************************************************

// internal
ScXMLObject *
ScXMLP::readScXMLHistory(ScXMLObject * container, cc_xml_elt * elt, const char * xmlns)
{
  assert(container && elt && xmlns);

  SoType historytype = ScXMLP::getClassType(xmlns, "history");
  assert(historytype.canCreateInstance());
  ScXMLHistory * history = static_cast<ScXMLHistory *>(historytype.createInstance());
  assert(history && history->isOfType(ScXMLHistory::getClassTypeId()));
  const int numattrs = cc_xml_elt_get_num_attributes(elt);
  const cc_xml_attr ** attrs = cc_xml_elt_get_attributes(elt);
  int c;
  for (c = 0; c < numattrs; ++c) {
    const char * attrname = cc_xml_attr_get_name(attrs[c]);
    const char * attrvalue = cc_xml_attr_get_value(attrs[c]);
    history->setXMLAttribute(attrname, attrvalue);
  }
  if (!history->handleXMLAttributes()) {
    SoDebugError::postInfo("ScXML::readFile", "error in handleXMLAttributes()");
    delete history;
    return NULL;
  }
  history->setContainer(container);

  const int numchildren = cc_xml_elt_get_num_children(elt);
  for (c = 0; c < numchildren; ++c) {
    cc_xml_elt * element = cc_xml_elt_get_child(elt, c);
    const char * elementtype = cc_xml_elt_get_type(element);
    if (strcmp(elementtype, COIN_XML_CDATA_TYPE) == 0) continue;

    // <transition>
    if (strcmp(elementtype, "transition") == 0) {
      ScXMLObject * transitionobj = ScXMLP::readScXMLTransition(history, element, xmlns);
      if (transitionobj) {
        assert(transitionobj->isOfType(ScXMLTransition::getClassTypeId()));
        assert(history->getTransition() == NULL);
        history->setTransition(static_cast<ScXMLTransition *>(transitionobj));
      } else {
        SoDebugError::post("ScXML::readFile", "error reading <%s> element",
                           elementtype);
        delete history;
        return NULL;
      }
    }
    else {
      SoDebugError::post("ScXML::readFile",
                         "unexpected XML element '<%s>' found in <history>",
                         elementtype);
      delete history;
      return NULL;
    }
  }

  return history;
} // readScXMLHistory

// *************************************************************************

// internal
ScXMLObject *
ScXMLP::readScXMLOnEntry(ScXMLObject * container, cc_xml_elt * elt, const char * xmlns)
{
  assert(container && elt && xmlns);

  SoType onentrytype = ScXMLP::getClassType(xmlns, "onentry");
  assert(onentrytype.canCreateInstance());
  ScXMLOnEntry * onentry = static_cast<ScXMLOnEntry *>(onentrytype.createInstance());
  assert(onentry && onentry->isOfType(ScXMLOnEntry::getClassTypeId()));
  const int numattrs = cc_xml_elt_get_num_attributes(elt);
  const cc_xml_attr ** attrs = cc_xml_elt_get_attributes(elt);
  int c;
  for (c = 0; c < numattrs; ++c) {
    const char * attrname = cc_xml_attr_get_name(attrs[c]);
    const char * attrvalue = cc_xml_attr_get_value(attrs[c]);
    onentry->setXMLAttribute(attrname, attrvalue);
  }
  if (!onentry->handleXMLAttributes()) {
    SoDebugError::postInfo("ScXML::readFile", "error in handleXMLAttributes()");
    delete onentry;
    return NULL;
  }
  onentry->setContainer(container);

  const int numchildren = cc_xml_elt_get_num_children(elt);
  for (c = 0; c < numchildren; ++c) {
    cc_xml_elt * element = cc_xml_elt_get_child(elt, c);
    const char * elementtype = cc_xml_elt_get_type(element);
    if (strcmp(elementtype, COIN_XML_CDATA_TYPE) == 0) continue;

    // invoke
    if (strcmp(elementtype, "invoke") == 0) {
      ScXMLObject * invokeobj = ScXMLP::readScXMLInvoke(onentry, element, xmlns);
      if (invokeobj) {
        assert(invokeobj->isOfType(ScXMLInvoke::getClassTypeId()));
        onentry->addInvoke(static_cast<ScXMLInvoke *>(invokeobj));
      } else {
        SoDebugError::post("ScXML::readFile", "error reading <%s> element", elementtype);
        delete onentry;
        return NULL;
      }
    }
    else {
      SoDebugError::post("ScXML::readFile", "unexpected XML element '<%s>' found in <onentry>",
                         elementtype);
      delete onentry;
      return NULL;
    }
  }
  return onentry;
} // readScXMLOnEntry

// *************************************************************************

// internal
ScXMLObject *
ScXMLP::readScXMLOnExit(ScXMLObject * container, cc_xml_elt * elt, const char * xmlns)
{
  assert(container && elt && xmlns);

  SoType onexittype = ScXMLP::getClassType(xmlns, "onexit");
  assert(onexittype.canCreateInstance());
  ScXMLOnExit * onexit = static_cast<ScXMLOnExit *>(onexittype.createInstance());
  assert(onexit && onexit->isOfType(ScXMLOnExit::getClassTypeId()));
  const int numattrs = cc_xml_elt_get_num_attributes(elt);
  const cc_xml_attr ** attrs = cc_xml_elt_get_attributes(elt);
  int c;
  for (c = 0; c < numattrs; ++c) {
    const char * attrname = cc_xml_attr_get_name(attrs[c]);
    const char * attrvalue = cc_xml_attr_get_value(attrs[c]);
    onexit->setXMLAttribute(attrname, attrvalue);
  }
  if (!onexit->handleXMLAttributes()) {
    SoDebugError::postInfo("ScXML::readFile", "error in handleXMLAttributes()");
    delete onexit;
    return NULL;
  }
  onexit->setContainer(container);

  const int numchildren = cc_xml_elt_get_num_children(elt);
  for (c = 0; c < numchildren; ++c) {
    cc_xml_elt * element = cc_xml_elt_get_child(elt, c);
    const char * elementtype = cc_xml_elt_get_type(element);
    if (strcmp(elementtype, COIN_XML_CDATA_TYPE) == 0) continue;

    // invoke
    if (strcmp(elementtype, "invoke") == 0) {
      ScXMLObject * invokeobj = ScXMLP::readScXMLInvoke(onexit, element, xmlns);
      if (invokeobj) {
        assert(invokeobj->isOfType(ScXMLInvoke::getClassTypeId()));
        onexit->addInvoke(static_cast<ScXMLInvoke *>(invokeobj));
      } else {
        SoDebugError::post("ScXML::readFile", "error reading <%s> element", elementtype);
        delete onexit;
        return NULL;
      }
    }
    else {
      SoDebugError::post("ScXML::readFile", "unexpected XML element '<%s>' found in <onentry>",
                         elementtype);
      delete onexit;
      return NULL;
    }
  }
  return onexit;
} // readScXMLOnExit

// *************************************************************************

// internal
ScXMLObject *
ScXMLP::readScXMLAnchor(ScXMLObject * container, cc_xml_elt * elt, const char * xmlns)
{
  assert(container && elt && xmlns);

  SoType anchortype = ScXMLP::getClassType(xmlns, "anchor");
  assert(anchortype.canCreateInstance());
  ScXMLAnchor * anchor = static_cast<ScXMLAnchor *>(anchortype.createInstance());
  assert(anchor && anchor->isOfType(ScXMLAnchor::getClassTypeId()));
  const int numattrs = cc_xml_elt_get_num_attributes(elt);
  const cc_xml_attr ** attrs = cc_xml_elt_get_attributes(elt);
  int c;
  for (c = 0; c < numattrs; ++c) {
    const char * attrname = cc_xml_attr_get_name(attrs[c]);
    const char * attrvalue = cc_xml_attr_get_value(attrs[c]);
    anchor->setXMLAttribute(attrname, attrvalue);
  }
  if (!anchor->handleXMLAttributes()) {
    SoDebugError::postInfo("ScXML::readFile", "error in handleXMLAttributes()");
    delete anchor;
    return NULL;
  }
  anchor->setContainer(container);

  const int numchildren = cc_xml_elt_get_num_children(elt);
  for (c = 0; c < numchildren; ++c) {
    cc_xml_elt * element = cc_xml_elt_get_child(elt, c);
    const char * elementtype = cc_xml_elt_get_type(element);
    if (strcmp(elementtype, COIN_XML_CDATA_TYPE) == 0) continue;

    SoDebugError::post("ScXML::readFile", "unexpected XML element '<%s>' found in <anchor>",
                       elementtype);
    delete anchor;
    return NULL;
  }

  return anchor;
} // readScXMLAnchor

// *************************************************************************

// internal
ScXMLObject *
ScXMLP::readScXMLInvoke(ScXMLObject * container, cc_xml_elt * elt, const char * xmlns)
{
  const char * targettype = "";
  const char * source = "";
  const int numattrs = cc_xml_elt_get_num_attributes(elt);
  const cc_xml_attr ** attrs = cc_xml_elt_get_attributes(elt);
  int c;
  for (c = 0; c < numattrs; ++c) {
    const char * attrname = cc_xml_attr_get_name(attrs[c]);
    if (strcmp(attrname, "targettype") == 0) {
      targettype = cc_xml_attr_get_value(attrs[c]);
    }
    else if (strcmp(attrname, "src") == 0) {
      source = cc_xml_attr_get_value(attrs[c]);
    }
  }

  SoType invoketype = ScXMLP::getInvokeClassType(xmlns, targettype, source);
  assert(invoketype.canCreateInstance());
  ScXMLInvoke * invoke = static_cast<ScXMLInvoke *>(invoketype.createInstance());
  assert(invoke && invoke->isOfType(ScXMLInvoke::getClassTypeId()));

  for (c = 0; c < numattrs; ++c) {
    const char * attrname = cc_xml_attr_get_name(attrs[c]);
    const char * attrvalue = cc_xml_attr_get_value(attrs[c]);
    invoke->setXMLAttribute(attrname, attrvalue);
  }
  if (!invoke->handleXMLAttributes()) {
    SoDebugError::postInfo("ScXML::readFile", "%s error in handleXMLAttributes()",
                           invoketype.getName().getString());
    delete invoke;
    return NULL;
  }
  invoke->setContainer(container);

  const int numchildren = cc_xml_elt_get_num_children(elt);
  for (c = 0; c < numchildren; ++c) {
    cc_xml_elt * element = cc_xml_elt_get_child(elt, c);
    const char * elementtype = cc_xml_elt_get_type(element);
    if (strcmp(elementtype, COIN_XML_CDATA_TYPE) == 0) continue;

    SoDebugError::post("ScXML::readFile",
                       "unexpected XML element '<%s>' found in <invoke>",
                       elementtype);
    delete invoke;
    return NULL;
  }


  return invoke;
} // readScXMLInvoke

// *************************************************************************

// internal
ScXMLObject *
ScXMLP::readScXMLDatamodel(ScXMLObject * container, cc_xml_elt * elt, const char * xmlns)
{
  if (!ScXMLP::no_datamodel_warning) {
    SoDebugError::postWarning("ScXML::readFile", "<datamodel> elements not supported");
    ScXMLP::no_datamodel_warning = TRUE; // don't repeat warning for this read...
  }
  return NULL;
} // readScXMLDatamodel

void
ScXMLP::init(void)
{
  coin_atexit((coin_atexit_f*) ScXMLP::cleanup, CC_ATEXIT_NORMAL);
  ScXMLP::namespaces = new ScXMLP::NamespaceDict;
  ScXMLP::targettypes = new ScXMLP::TargettypeDict;
}

void
ScXMLP::cleanup(void)
{
  cleanup_namespacedict(ScXMLP::namespaces);
  delete ScXMLP::namespaces;
  ScXMLP::namespaces = NULL;
  cleanup_targettypes();
}

void
ScXMLP::cleanup_namespacedict(NamespaceDict * dict)
{
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
