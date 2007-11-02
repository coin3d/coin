#ifndef COIN_XML_TYPES_H
#define COIN_XML_TYPES_H

/**************************************************************************\
 *
 *  This file is part of the SIM Scenery library.
 *  Copyright (C) 2000-2007 by Systems in Motion.  All rights reserved.
 *
 *  This software is proprietary to and embodies the confidential 
 *  technology of Systems in Motion.  Possession, use, or copying of this
 *  software and media is authorized only pursuant to a valid written
 *  license from Systems in Motion or an authorized sublicensor. 
 *
 *  For more information, contact SIM <http://www.sim.no/> by means of:
 *  Mail: Systems in Motion AS, Bygdøy allé 5, N-0257 Oslo, Norway;
 *  Email: <sales@sim.no>; Voice: +47 23 27 25 10; Fax: +47 23 27 25 11.
 *
\**************************************************************************/

#include <Inventor/C/basic.h>

/* ********************************************************************** */

#define COIN_XML_CDATA_TYPE "cdata"
#define COIN_XML_ROOT_TYPE  "root"

typedef  struct cc_xml_doc       cc_xml_doc;
typedef  struct cc_xml_elt       cc_xml_elt;
typedef  struct cc_xml_attr      cc_xml_attr;
typedef  struct cc_xml_ent       cc_xml_ent;
typedef  struct cc_xml_path      cc_xml_path;
typedef  struct cc_xml_bin       cc_xml_bin;
typedef  struct cc_xml_bin_atom  cc_xml_bin_atom;

/* non-abbreviated aliases */
typedef  cc_xml_doc              cc_xml_document;
typedef  cc_xml_elt              cc_xml_element;
typedef  cc_xml_attr             cc_xml_attribute;
typedef  cc_xml_ent              cc_xml_entity;

/* ********************************************************************** */

#ifdef __cplusplus

#if 0
class SbXmlDocument;
class SbXmlElement;
class SbXmlAttribute;
class SbXmlEntity;
class SbXmlPath;

SbXmlDocument *  cc_xml_wrap(cc_xml_doc * doc, int pass_ownership = FALSE);
SbXmlElement *   cc_xml_wrap(cc_xml_elt * elt, int pass_ownership = FALSE);
SbXmlAttribute * cc_xml_wrap(cc_xml_attr * attr, int pass_ownership = FALSE);
SbXmlEntity *    cc_xml_wrap(cc_xml_ent * ent, int pass_ownership = FALSE);
SbXmlPath *      cc_xml_wrap(cc_xml_path * path, int pass_ownership = FALSE);

cc_xml_doc *     cc_xml_unwrap(SbXmlDocument * doc, int pass_ownership = FALSE);
cc_xml_elt *     cc_xml_unwrap(SbXmlElement * elt, int pass_ownership = FALSE);
cc_xml_attr *    cc_xml_unwrap(SbXmlAttribute * attr, int pass_ownership = FALSE);
cc_xml_ent *     cc_xml_unwrap(SbXmlEntity * ent, int pass_ownership = FALSE);
cc_xml_path *    cc_xml_unwrap(SbXmlPath * path, int pass_ownership = FALSE);
#endif // 0

#endif /* __cplusplus */

/* ********************************************************************** */

#endif /* !COIN_XML_TYPES_H */
