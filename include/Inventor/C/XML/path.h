#ifndef COIN_XML_PATH_H
#define COIN_XML_PATH_H

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

#include <stdarg.h>

#include <Inventor/C/XML/types.h>

/* ********************************************************************** */

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

cc_xml_path * cc_xml_path_new(void);
void          cc_xml_path_delete_x(cc_xml_path * path);

void          cc_xml_path_clear_x(cc_xml_path * path);
void          cc_xml_path_set_x(cc_xml_path * path, ...); /* use 'v' in signature? */
void          cc_xml_path_copy_x(cc_xml_path * path, cc_xml_path * path2);
void          cc_xml_path_reverse_x(cc_xml_path * path);

int           cc_xml_path_get_length(const cc_xml_path * path);
const char *  cc_xml_path_get_type(const cc_xml_path * path, int idx);
int           cc_xml_path_get_index(const cc_xml_path * path, int idx);
int           cc_xml_path_match_p(const cc_xml_path * path, const cc_xml_elt * elt);

void          cc_xml_path_append_x(cc_xml_path * path, const char * elt, int idx);
void          cc_xml_path_append_path_x(cc_xml_path * path, cc_xml_path * path2);
void          cc_xml_path_prepend_x(cc_xml_path * path, const char * elt, int idx);
void          cc_xml_path_prepend_path_x(cc_xml_path * path, cc_xml_path * path2);
void          cc_xml_path_truncate_x(cc_xml_path * path, int length);

void          cc_xml_path_dump(const cc_xml_path * path);

#ifdef __cplusplus
} /* extern "C" */
#endif /* __cplusplus */

/* ********************************************************************** */

#endif /* !COIN_XML_PATH_H */
