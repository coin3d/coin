#ifndef COIN_XML_DOCUMENT_H
#define COIN_XML_DOCUMENT_H

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

#include <Inventor/C/XML/types.h>

/* ********************************************************************** */

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

cc_xml_doc *   cc_xml_doc_new(void);
void           cc_xml_doc_delete_x(cc_xml_doc * doc);

cc_xml_doc *   cc_xml_doc_read_file(const char * path);
cc_xml_doc *   cc_xml_doc_read_buffer(const char * buffer);

void           cc_xml_doc_set_filename_x(cc_xml_doc * doc, const char * path);
const char *   cc_xml_doc_get_filename(const cc_xml_doc * doc);

cc_xml_elt *   cc_xml_doc_get_root(const cc_xml_doc * doc);
void           cc_xml_doc_set_current_x(cc_xml_doc * doc, cc_xml_elt * elt);
cc_xml_elt *   cc_xml_doc_get_current(const cc_xml_doc * doc);
void           cc_xml_doc_strip_whitespace_x(cc_xml_doc * doc);

void           cc_xml_doc_set_root_x(cc_xml_doc * doc, cc_xml_elt * root);

const cc_xml_elt * cc_xml_doc_find_element(const cc_xml_doc * doc, cc_xml_path * path);
const cc_xml_elt * cc_xml_doc_find_next_element(const cc_xml_doc * doc, cc_xml_elt * elt, cc_xml_path * path);
cc_xml_elt *   cc_xml_doc_create_element_x(cc_xml_doc * doc, cc_xml_path * path);

void           cc_xml_doc_dump_to_file(const cc_xml_doc * doc, const char * path);

#ifdef __cplusplus
} /* extern "C" */
#endif /* __cplusplus */

#endif /* !COIN_XML_DOCUMENT_H */
