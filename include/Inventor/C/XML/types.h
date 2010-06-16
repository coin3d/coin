#ifndef COIN_XML_TYPES_H
#define COIN_XML_TYPES_H

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

#include <Inventor/C/basic.h>

/* ********************************************************************** */

#define COIN_XML_CDATA_TYPE "cdata"
#define COIN_XML_ROOT_TYPE  "root"

typedef  struct cc_xml_doc       cc_xml_doc;
typedef  struct cc_xml_elt       cc_xml_elt;
typedef  struct cc_xml_attr      cc_xml_attr;
typedef  struct cc_xml_ent       cc_xml_ent;
typedef  struct cc_xml_path      cc_xml_path;

/* non-abbreviated aliases */
typedef  cc_xml_doc              cc_xml_document;
typedef  cc_xml_elt              cc_xml_element;
typedef  cc_xml_attr             cc_xml_attribute;
typedef  cc_xml_ent              cc_xml_entity;

/* streaming parsing */
enum cc_xml_filter_choice {
  KEEP,
  DISCARD
};

typedef cc_xml_filter_choice cc_xml_filter_cb(void * userdata, cc_xml_doc * doc, cc_xml_elt * elt, int pushing);

/* ********************************************************************** */

#ifdef __cplusplus

#if 0
class SbXmlDocument;
class SbXmlElement;
class SbXmlAttribute;
class SbXmlEntity;
class SbXmlPath;

SbXmlDocument *  COIN_DLL_API cc_xml_wrap(cc_xml_doc * doc, int pass_ownership = FALSE);
SbXmlElement *   COIN_DLL_API cc_xml_wrap(cc_xml_elt * elt, int pass_ownership = FALSE);
SbXmlAttribute * COIN_DLL_API cc_xml_wrap(cc_xml_attr * attr, int pass_ownership = FALSE);
SbXmlEntity *    COIN_DLL_API cc_xml_wrap(cc_xml_ent * ent, int pass_ownership = FALSE);
SbXmlPath *      COIN_DLL_API cc_xml_wrap(cc_xml_path * path, int pass_ownership = FALSE);

cc_xml_doc *     COIN_DLL_API cc_xml_unwrap(SbXmlDocument * doc, int pass_ownership = FALSE);
cc_xml_elt *     COIN_DLL_API cc_xml_unwrap(SbXmlElement * elt, int pass_ownership = FALSE);
cc_xml_attr *    COIN_DLL_API cc_xml_unwrap(SbXmlAttribute * attr, int pass_ownership = FALSE);
cc_xml_ent *     COIN_DLL_API cc_xml_unwrap(SbXmlEntity * ent, int pass_ownership = FALSE);
cc_xml_path *    COIN_DLL_API cc_xml_unwrap(SbXmlPath * path, int pass_ownership = FALSE);
#endif /* 0 */

#endif /* __cplusplus */

/* ********************************************************************** */

#endif /* !COIN_XML_TYPES_H */
