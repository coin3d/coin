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

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif // HAVE_CONFIG_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include <Inventor/C/XML/document.h>
#include <Inventor/C/XML/element.h>
#include <Inventor/C/XML/path.h>
#include <Inventor/C/XML/utils.h>
#include <Inventor/C/XML/parser.h>

// #include <Inventor/C/XML/bux.h>

// *************************************************************************
// this is a document / to prove that I was here

struct cc_xml_doc {
  char *        filename;
  cc_xml_elt *     root;
  cc_xml_elt *     current;
};

// *************************************************************************

cc_xml_doc *
cc_xml_doc_new(void)
{
  cc_xml_doc * doc = (cc_xml_doc *) malloc(sizeof(struct cc_xml_doc));
  assert(doc);
  doc->filename = NULL;
  doc->root = (cc_xml_elt *) NULL;
  doc->current = (cc_xml_elt *) NULL;
  return doc;
}

void
cc_xml_doc_delete_x(cc_xml_doc * doc)
{
  assert(doc);
  if ( doc->filename ) free(doc->filename);
  if ( doc->root ) cc_xml_elt_delete_x(doc->root);
  free(doc);
}

cc_xml_doc *
cc_xml_doc_read_file(const char * path)
{
  char * buffer = cc_xml_load_file(path);
  if ( buffer == NULL ) return NULL;
  cc_xml_doc * doc = cc_xml_doc_read_buffer(buffer);
  doc->filename = cc_xml_strdup(path);
  free(buffer);
  return doc;
}

cc_xml_doc *
cc_xml_doc_read_buffer(const char * buffer)
{
  cc_xml_doc * doc = cc_xml_doc_new();
  cc_xml_parser_scan_string(doc, buffer);
  return doc;
}

void
cc_xml_doc_set_filename_x(cc_xml_doc * doc, const char * path)
{
  assert(doc);
  if ( doc->filename ) {
    free(doc->filename);
    doc->filename = NULL;
  }
  doc->filename = cc_xml_strdup(path);
}

const char *
cc_xml_doc_get_filename(const cc_xml_doc * doc)
{
  assert(doc && doc->filename);
  return doc->filename;
}

void
cc_xml_doc_set_current_x(cc_xml_doc * doc, cc_xml_elt * elt)
{
  assert(doc);
  doc->current = elt;
}

cc_xml_elt *
cc_xml_doc_get_current(const cc_xml_doc * doc)
{
  assert(doc);
  return doc->current;
}

void
cc_xml_doc_set_root_x(cc_xml_doc * doc, cc_xml_elt * root)
{
  assert(doc);
  doc->root = root;
}

cc_xml_elt *
cc_xml_doc_get_root(const cc_xml_doc * doc)
{
  assert(doc);
  return doc->root;
}

// *************************************************************************

void
cc_xml_doc_strip_whitespace_x(cc_xml_doc * doc)
{
  assert(doc);
  return;
#if 0 // FIXME
  cc_xml_path * path = cc_xml_path_new();
  cc_xml_path_set_x(path, CC_XML_CDATA_TYPE, NULL);
  cc_xml_elt * elt = cc_xml_doc_find_element(doc, path);
  while ( elt != NULL ) {
    cc_xml_elt * next = cc_xml_doc_find_next_element(doc, elt, path);
    if ( sc_whitespace_p(cc_xml_elt_get_data(elt)) )  {
      cc_xml_elt_remove_child_x(cc_xml_elt_get_parent(elt), elt);
      cc_xml_elt_delete_x(elt);
    } else {
      cc_xml_elt_strip_whitespace_x(elt);
    }
    elt = next;
  }
  cc_xml_path_delete_x(path);
#endif
}

// *************************************************************************

const cc_xml_elt *
cc_xml_doc_find_element(const cc_xml_doc * doc, cc_xml_path * path)
{
  assert(doc && path);
  return cc_xml_elt_find(doc->root, path);
} // cc_xml_doc_find_element()

const cc_xml_elt *
cc_xml_doc_find_next_element(const cc_xml_doc * doc, cc_xml_elt * prev, cc_xml_path * path)
{
  assert(doc && prev && path);
  return cc_xml_elt_find_next(doc->root, prev, path);
} // cc_xml_doc_find_next_element()

cc_xml_elt *
cc_xml_doc_create_element_x(cc_xml_doc * doc, cc_xml_path * path)
{
  assert(doc && path);
  cc_xml_elt * root = cc_xml_doc_get_root(doc);
  assert(root);
  return cc_xml_elt_create_x(root, path);
} // cc_xml_doc_create_element_x()

// *************************************************************************

void
cc_xml_doc_dump_to_file(cc_xml_doc * doc, const char * path)
{
  assert(doc && path);
  FILE * fp = NULL;
  if ( strcmp(path, "-") == 0 ) fp = stdout;
  else fp = fopen(path, "wb");
  assert(fp != NULL);
  if ( doc->root )
    cc_xml_elt_dump_to_file(doc->root, 0, fp);
  fclose(fp);
}

// *************************************************************************
