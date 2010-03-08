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

#include <Inventor/C/XML/attribute.h>

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif // HAVE_CONFIG_H

#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "utils.h"

// TODO:
// - optimize empty strings to use a static, nonfreeable buffer?

// *************************************************************************

struct cc_xml_attr {
  char * name;
  char * value;
};

// *************************************************************************

/*!
  Creates a new cc_xml_attr object, with no name or value information.
*/

cc_xml_attr *
cc_xml_attr_new(void)
{
  cc_xml_attr * attr = new cc_xml_attr;
  assert(attr);
  attr->name = NULL;
  attr->value = NULL;
  return attr;
}

/*!
  Creates a new cc_xml_attr object, with the given name and value information.
  The \a value argument can be NULL.
*/

cc_xml_attr *
cc_xml_attr_new_from_data(const char * name, const char * value)
{
  cc_xml_attr * attr = cc_xml_attr_new();
  attr->name = cc_xml_strdup(name);
  if (value) { attr->value = cc_xml_strdup(value); }
  return attr;
}

/*!
  Returns a clone of the given attribute.
*/

cc_xml_attr *
cc_xml_attr_clone(const cc_xml_attr * attr)
{
  assert(attr);
  return cc_xml_attr_new_from_data(attr->name, attr->value);
}

/*!
  Deletes the cc_xml_Attr object, including its internals.
*/

void
cc_xml_attr_delete_x(cc_xml_attr * attr)
{
  assert(attr);
  if (attr->name) delete [] attr->name;
  if (attr->value) delete [] attr->value;
  delete attr;
}

// *************************************************************************

/*!
  Sets the name-part of the attribute.  Old name-information will be freed.
  The \a name argument can be NULL to clear the setting.
*/

void
cc_xml_attr_set_name_x(cc_xml_attr * attr, const char * name)
{
  if (attr->name) {
    delete [] attr->name;
    attr->name = NULL;
  }
  if (name) attr->name = cc_xml_strdup(name);
}

/*!
  Returns the name-part of the attribute if one is set, and NULL otherwise.
*/

const char *
cc_xml_attr_get_name(const cc_xml_attr * attr)
{
  return attr->name;
}

/*!
  Sets the value-part of the attribute.  Old information will be freed.
  The \a value argument can be NULL to clear the information.
*/

void
cc_xml_attr_set_value_x(cc_xml_attr * attr, const char * value)
{
  if (attr->value) {
    delete [] attr->value;
    attr->value = NULL;
  }
  if (value) attr->value = cc_xml_strdup(value);
}

/*!
  Returns the value part of the attribute if one is set, or NULL otherwise.
*/

const char *
cc_xml_attr_get_value(const cc_xml_attr * attr)
{
  return attr->value;
}

// *************************************************************************

// TODO: type-specific accessor methods...

// *************************************************************************

size_t
cc_xml_attr_calculate_size(const cc_xml_attr * attr)
{
  size_t bytes = 0;
  bytes += strlen(attr->name);
  bytes += 2; // ="
  // FIXME: count quotables in string value, and add up quote count
  if (attr->value) bytes += strlen(attr->value);
  bytes += 1; // "
  return bytes;
}

size_t
cc_xml_attr_write_to_buffer(const cc_xml_attr * attr, char * buffer, size_t bufsize)
{
  // We assert on mismatches between calculated memory usage and actual memory
  // usage, since this must be calculated correctly for not getting memory corruption
  // on invalid buffer size allocation before writing.
  const char * const origbufferptr = buffer;
  const size_t assumed = cc_xml_attr_calculate_size(attr);
  assert(assumed < bufsize);
  int namelen = strlen(attr->name);
  strcpy(buffer, attr->name);
  buffer += namelen;
  strcpy(buffer, "=\"");
  buffer += 2;
  if (attr->value) {
    int valuelen = strlen(attr->value);
    // FIXME: count quotables, and insert quoting in value string if needed
    strcpy(buffer, attr->value);
    buffer += valuelen;
  }
  buffer[0] = '"';
  buffer += 1;
  size_t used = buffer - origbufferptr;
  assert(used == assumed);
  return used;
}

// *************************************************************************
