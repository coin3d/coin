#ifndef COIN_XML_ATTRIBUTE_H
#define COIN_XML_ATTRIBUTE_H

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

cc_xml_attr * cc_xml_attr_new(void);
cc_xml_attr * cc_xml_attr_new_from_values(const char * name, const char * value);
void          cc_xml_attr_delete_x(cc_xml_attr * attr);

void          cc_xml_attr_set_name_x(cc_xml_attr * attr, const char * name);
const char *  cc_xml_attr_get_name(const cc_xml_attr * attr);
void          cc_xml_attr_set_value_x(cc_xml_attr * attr, const char * value);
const char *  cc_xml_attr_get_value(const cc_xml_attr * attr);

int           cc_xml_attr_is_int32(const cc_xml_attr * attr);
int32_t       cc_xml_attr_get_int32(const cc_xml_attr * attr);
void          cc_xml_attr_set_int32_x(const cc_xml_attr * attr, int32_t intval);

#ifdef __cplusplus
} /* extern "C" */
#endif /* __cplusplus */

/* ********************************************************************** */

#endif /* !COIN_XML_ATTRIBUTE_H */
