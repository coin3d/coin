#ifndef COIN_XML_PARSER_H
#define COIN_XML_PARSER_H

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

#ifndef COIN_INTERNAL
#error this header file is not public
#endif // !COIN_INTERNAL

#include <Inventor/C/XML/types.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


typedef int cc_xml_filter_cb(cc_xml_doc * doc, cc_xml_path * where, cc_xml_elt * elt, void * closure);

void cc_xml_set_filter_callback(cc_xml_filter_cb * filtercb, void * closure);

int cc_xml_parser_scan_string(cc_xml_doc * doc, const char * buf);

#ifdef __cplusplus
} /* extern "C" */
#endif /* __cplusplus */

#endif /* !COIN_XML_PARSER_H */
