#ifndef COIN_XMLDOCUMENT_H
#define COIN_XMLDOCUMENT_H

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

#include <Inventor/C/XML/types.h>

/* ********************************************************************** */

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* basic construction */
COIN_DLL_API cc_xml_doc * cc_xml_doc_new(void);
COIN_DLL_API void cc_xml_doc_delete_x(cc_xml_doc * doc);

/* parser configuration */
COIN_DLL_API void cc_xml_doc_set_filter_cb_x(cc_xml_doc * doc, cc_xml_filter_cb * cb, void * userdata);
COIN_DLL_API void cc_xml_doc_get_filter_cb(const cc_xml_doc * doc, cc_xml_filter_cb *& cb, void *& userdata);

/* document io */
COIN_DLL_API SbBool cc_xml_doc_read_file_x(cc_xml_doc * doc, const char * path);
COIN_DLL_API SbBool cc_xml_doc_read_buffer_x(cc_xml_doc * doc, const char * buffer, size_t buflen);

COIN_DLL_API SbBool cc_xml_doc_parse_buffer_partial_x(cc_xml_doc * doc, const char * buffer, size_t buflen);
COIN_DLL_API SbBool cc_xml_doc_parse_buffer_partial_done_x(cc_xml_doc * doc, const char * buffer, size_t buflen);

COIN_DLL_API SbBool cc_xml_doc_write_to_buffer(const cc_xml_doc * doc, char *& buffer, size_t & bytes);
COIN_DLL_API SbBool cc_xml_doc_write_to_file(const cc_xml_doc * doc, const char * path);

COIN_DLL_API cc_xml_path * cc_xml_doc_diff(const cc_xml_doc * doc, const cc_xml_doc * other);

/* document attributes */
COIN_DLL_API void cc_xml_doc_set_filename_x(cc_xml_doc * doc, const char * path);
COIN_DLL_API const char * cc_xml_doc_get_filename(const cc_xml_doc * doc);

/* misc... */

COIN_DLL_API cc_xml_elt *   cc_xml_doc_get_root(const cc_xml_doc * doc);
COIN_DLL_API void           cc_xml_doc_set_current_x(cc_xml_doc * doc, cc_xml_elt * elt);
COIN_DLL_API cc_xml_elt *   cc_xml_doc_get_current(const cc_xml_doc * doc);
COIN_DLL_API void           cc_xml_doc_strip_whitespace_x(cc_xml_doc * doc);

COIN_DLL_API void           cc_xml_doc_set_root_x(cc_xml_doc * doc, cc_xml_elt * root);

COIN_DLL_API const cc_xml_elt * cc_xml_doc_find_element(const cc_xml_doc * doc, cc_xml_path * path);
COIN_DLL_API const cc_xml_elt * cc_xml_doc_find_next_element(const cc_xml_doc * doc, cc_xml_elt * elt, cc_xml_path * path);
COIN_DLL_API cc_xml_elt *       cc_xml_doc_create_element_x(cc_xml_doc * doc, cc_xml_path * path);


#ifdef __cplusplus
} /* extern "C" */
#endif /* __cplusplus */

#endif /* !COIN_XML_DOCUMENT_H */
