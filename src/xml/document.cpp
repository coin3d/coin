/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2007 by Systems in Motion.  All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  ("GPL") version 2 as published by the Free Software Foundation.
 *  See the file LICENSE.GPL at the root directory of this source
 *  distribution for additional information about the GNU GPL.
 *
 *  For using Coin with software that can not be combined with the GNU
 *  GPL, and for taking advantage of the additional benefits of our
 *  support services, please contact Systems in Motion about acquiring
 *  a Coin Professional Edition License.
 *
 *  See http://www.coin3d.org/ for more information.
 *
 *  Systems in Motion, Postboks 1283, Pirsenteret, 7462 Trondheim, NORWAY.
 *  http://www.sim.no/  sales@sim.no  coin-support@coin3d.org
 *
\**************************************************************************/

#include <Inventor/C/XML/document.h>
#include "documentp.h"

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif // HAVE_CONFIG_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include <boost/scoped_array.hpp>

#include <Inventor/C/XML/element.h>
#include <Inventor/C/XML/attribute.h>
#include <Inventor/C/XML/path.h>
#include <Inventor/C/XML/parser.h>
#include <Inventor/lists/SbList.h>

#include "expat/expat.h"
#include "utils.h"
#include "elementp.h"

/*!
  \page xmlparsing XML Parsing with Coin

  For Coin 3.0, we added an XML parser to Coin.  This document describes
  how it can be used for generic purposes.

  Why another XML parser, you might ask?  First of all, the XML parser
  is actually a third-party parser, expat.  Coin needed one, and many
  Coin-dependent projects needed one as well.  We therefore needed to
  expose an API for it.  However, integrating a 3rd-party parser into
  Coin, we can not expose its API directly, or other projects also
  using Expat would get conflicts.  We therefore needed to expose the
  XML API with a unique API, hence the API you see here.  It is based
  on a XML DOM API we used in a couple of other projects, but tweaked
  to fit into Coin and be wrapped over Expat (which the original
  implementation wasn't).

  The XML parser is both a streaming parser and a DOM parser.  Being a
  streaming parser means that documentrs can be read in without having
  to be fully contained in memory.  When used as a DOM parser, the
  whole document is fully parsed in first, and then inspected by
  client code by traversing the DOM.  The two modes can actually be
  mixed if ending up with a partial DOM sounds useful.

  The XML parser has both a C API and a C++ API.  The C++ API is just
  a wrapper around the C API, and only serves as convenience if you
  like to read/write C++ code (which is tighter) better than more
  verbose C code.

  The C API naming convention may look a bit strange, unless you have
  written libraries to be wrapped for scheme/lisp-like languages
  before.  Then you might be familiar with the convention of suffixing
  your functions based on their behaviour/usage meaning.  Mutating
  functions are suffixed with "!", or "_x" for (eXclamation point),
  and predicates are suffixed with "?", or "_p" in C.


  \sa cc_xml_doc, cc_xml_elt, cc_xml_attr
*/

/*!
  \var cc_xml_doc

  This type is the container for an XML document structure.
*/

// #define DEV_DEBUG 1

// *************************************************************************
// this is a document / to prove that I was here

struct cc_xml_doc {
  XML_Parser parser;


  cc_xml_filter_cb * filtercb;
  void * filtercbdata;
  
  // document type
  
  char * xmlversion;
  char * xmlencoding;

  char * filename;
  cc_xml_elt * root;
  cc_xml_elt * current;

  SbList<cc_xml_elt *> parsestack;
};

// *************************************************************************
// internal functions

namespace {

void
cc_xml_doc_expat_element_start_handler_cb(void * userdata, const XML_Char * elementtype, const XML_Char ** attributes)
{
  XML_Parser parser = static_cast<XML_Parser>(userdata);
  cc_xml_doc * doc = static_cast<cc_xml_doc *>(XML_GetUserData(parser));

  cc_xml_elt * elt = cc_xml_elt_new_from_data(elementtype, NULL);
  assert(elt);

  // FIXME: check if attribute values are automatically dequoted or not...
  // (dequote if not)
  if (attributes) {
    for (int c = 0; attributes[c] != NULL; c += 2) {
      cc_xml_attr * attr = cc_xml_attr_new_from_data(attributes[c], attributes[c+1]);
      cc_xml_elt_set_attribute_x(elt, attr);
    }
  }

  if (doc->parsestack.getLength() > 0) {
    cc_xml_elt * parent = doc->parsestack[doc->parsestack.getLength()-1];
    cc_xml_elt_add_child_x(parent, elt);
  }

  doc->parsestack.push(elt);
  if (doc->filtercb) {
    doc->filtercb(doc->filtercbdata, doc, elt, TRUE);
  }
}

void
cc_xml_doc_expat_element_end_handler_cb(void * userdata, const XML_Char * element)
{
  XML_Parser parser = static_cast<XML_Parser>(userdata);
  cc_xml_doc * doc = static_cast<cc_xml_doc *>(XML_GetUserData(parser));

  const int stackdepth = doc->parsestack.getLength();
  if (stackdepth == 0) {
    // flag error
    return;
  }

  cc_xml_elt * topelt = doc->parsestack.pop();
  if (strcmp(cc_xml_elt_get_type(topelt), element) != 0) {
    // this means XML input is closing a tag that was not the one opened at
    // this level. flag error
  }

  if (doc->filtercb) {
    switch (doc->filtercb(doc->filtercbdata, doc, topelt, FALSE)) {
    case DISCARD:
      {
        cc_xml_elt * parent = cc_xml_elt_get_parent(topelt);
        cc_xml_elt_remove_child_x(parent, topelt);
        cc_xml_elt_delete_x(topelt);
      }
      break;
    case KEEP:
      break;
    default:
      assert(!"invalid filter choice returned from client code");
      break;
    }
  }
}

void
cc_xml_doc_expat_character_data_handler_cb(void * userdata, const XML_Char * cdata, int len)
{
#ifdef DEV_DEBUG
  fprintf(stdout, "cc_xml_doc_expat_character_data_handler_cb()\n");
#endif // DEV_DEBUG
  XML_Parser parser = static_cast<XML_Parser>(userdata);
  cc_xml_doc * doc = static_cast<cc_xml_doc *>(XML_GetUserData(parser));

  cc_xml_elt * elt = cc_xml_elt_new();
  assert(elt);

  // need a temporary buffer for the cdata to make a nullterminated string.
  boost::scoped_array<char> buffer;
  buffer.reset(new char [len + 1]);
  memcpy(buffer.get(), cdata, len);
  buffer[len] = '\0';
  cc_xml_elt_set_type_x(elt, COIN_XML_CDATA_TYPE);
  cc_xml_elt_set_cdata_x(elt, buffer.get());

  if (doc->filtercb) {
    doc->filtercb(doc->filtercbdata, doc, elt, TRUE);
    cc_xml_filter_choice choice = doc->filtercb(doc->filtercbdata, doc, elt, FALSE);
    switch (choice) {
    case KEEP:
      if (doc->parsestack.getLength() > 0) {
        cc_xml_elt * parent = doc->parsestack[doc->parsestack.getLength()-1];
        cc_xml_elt_add_child_x(parent, elt);
      }
      break;
    case DISCARD:
      cc_xml_elt_delete_x(elt);
      elt = NULL;
      break;
    default:
      assert(!"invalid filter choice returned from client code");
      break;
    }
  }
}

void
cc_xml_doc_expat_processing_instruction_handler_cb(void * userdata, const XML_Char * target, const XML_Char * pidata)
{
  XML_Parser parser = static_cast<XML_Parser>(userdata);
  cc_xml_doc * doc = static_cast<cc_xml_doc *>(XML_GetUserData(parser));
#ifdef DEV_DEBUG
  fprintf(stdout, "received processing Instruction...\n");
#endif // DEV_DEBUG
}


void
cc_xml_doc_create_parser_x(cc_xml_doc * doc)
{
  assert(doc && !doc->parser);
  doc->parser = XML_ParserCreate(NULL);
  assert(doc->parser);
  if (XML_GetErrorCode(doc->parser) != 0) {
    fprintf(stdout, "%s\n", XML_ErrorString(XML_GetErrorCode(doc->parser)));
  }

  XML_UseParserAsHandlerArg(doc->parser);
  XML_SetUserData(doc->parser, doc);
  XML_SetElementHandler(doc->parser,
                        cc_xml_doc_expat_element_start_handler_cb,
                        cc_xml_doc_expat_element_end_handler_cb);
  XML_SetCharacterDataHandler(doc->parser,
                              cc_xml_doc_expat_character_data_handler_cb);
  XML_SetProcessingInstructionHandler(doc->parser, cc_xml_doc_expat_processing_instruction_handler_cb);
}

void
cc_xml_doc_delete_parser_x(cc_xml_doc * doc)
{
  assert(doc && doc->parser);
  XML_ParserFree(doc->parser);
  doc->parser = NULL;
}

} // anonymous namespace

// *************************************************************************

cc_xml_doc *
cc_xml_doc_new(void)
{
  cc_xml_doc * doc = new cc_xml_doc;
  assert(doc);
  doc->parser = NULL;
  doc->filtercb = NULL;
  doc->filtercbdata = NULL;
  doc->filename = NULL;
  doc->root = NULL;
  doc->current = NULL;
  return doc;
}

void
cc_xml_doc_delete_x(cc_xml_doc * doc)
{
  assert(doc);
  if (doc->filename) delete [] doc->filename;
  if (doc->parser) { cc_xml_doc_delete_parser_x(doc); }
  if (doc->root) cc_xml_elt_delete_x(doc->root);
  delete doc;
}

/* *************************************************************************

/*!
  Sets the filter callback for document parsing.  This makes it
  possible to use the parser as a streaming parser, by making the
  parser discard all elements it has read in.
*/

void
cc_xml_doc_set_filter_cb_x(cc_xml_doc * doc, cc_xml_filter_cb * cb, void * userdata)
{
  doc->filtercb = cb;
  doc->filtercbdata = userdata;
}


void
cc_xml_doc_get_filter_cb(const cc_xml_doc * doc, cc_xml_filter_cb *& cb, void *& userdata)
{
  cb = doc->filtercb;
  userdata = doc->filtercbdata;
}

// *************************************************************************

cc_xml_doc *
cc_xml_read_file(const char * path) // parser.h convenience function
{
  cc_xml_doc * doc = cc_xml_doc_new();
  assert(doc);
  if (!cc_xml_doc_read_file_x(doc, path)) {
    cc_xml_doc_delete_x(doc);
    return NULL;
  }
  return doc;
}

SbBool
cc_xml_doc_read_file_x(cc_xml_doc * doc, const char * path)
{
  if (!doc->parser) {
    cc_xml_doc_create_parser_x(doc);
  }

  FILE * fp = fopen(path, "rb");
  if (!fp) {
    // FIXME: error condition
    cc_xml_doc_delete_parser_x(doc);
    return FALSE;
  }

  // read in file in 8K chunks, buffers kept by expat
  void * buf = XML_GetBuffer(doc->parser, 8192);
  if (!buf) {
  }

  SbBool error = FALSE;
  SbBool final = FALSE;

  while (!final && !error) {
    int bytes = static_cast<int>(fread(buf, 1, 8192, fp));
    final = feof(fp);
  
    XML_ParseBuffer(doc->parser, bytes, final);
  }

  fclose(fp);

  cc_xml_doc_set_filename_x(doc, path);

  return !error;
}

cc_xml_doc *
cc_xml_read_buffer(const char * buffer) // parser.h convenience function
{
  cc_xml_doc * doc = cc_xml_doc_new();
  assert(doc);
  assert(buffer);
  size_t buflen = strlen(buffer);
  if (!cc_xml_doc_read_buffer_x(doc, buflen, buffer)) {
    cc_xml_doc_delete_x(doc);
    return NULL;
  }
  cc_xml_doc_set_filename_x(doc, "<memory buffer>");
  return doc;
}

namespace {
void cc_xml_doc_parse_buffer_partial_init_x(cc_xml_doc * doc);
}

SbBool
cc_xml_doc_read_buffer_x(cc_xml_doc * doc, size_t buflen, const char * buffer)
{
#ifdef DEV_DEBUG
  fprintf(stdout, "cc_xml_doc_read_buffer_x(%p, %d, %p)\n", doc, (int) buflen, buffer);
#endif // DEV_DEBUG
  cc_xml_doc_parse_buffer_partial_init_x(doc);
  if (!cc_xml_doc_parse_buffer_partial_x(doc, buflen, buffer)) {
    return FALSE;
  }
  return cc_xml_doc_parse_buffer_partial_done_x(doc);
}

// xml_doc_parse_ vs xml_doc_read_

namespace {
void
cc_xml_doc_parse_buffer_partial_init_x(cc_xml_doc * doc) // maybe expose and require explicit?
{
#ifdef DEV_DEBUG
  fprintf(stdout, "cc_xml_doc_parse_buffer_partial_init_x()\n");
#endif // DEV_DEBUG
  assert(doc->parser == NULL);
  cc_xml_doc_create_parser_x(doc);
}
}

SbBool
cc_xml_doc_parse_buffer_partial_x(cc_xml_doc * doc, size_t buflen, const char * buffer)
{
#ifdef DEV_DEBUG
  fprintf(stdout, "cc_xml_doc_parse_buffer_partial_x()\n");
#endif // DEV_DEBUG
  if (!doc->parser) {
    cc_xml_doc_parse_buffer_partial_init_x(doc);
  }

  XML_Status ok = XML_Parse(doc->parser, buffer, static_cast<int>(buflen), TRUE);
  if (!ok) {
    fprintf(stdout, "parse error: %s\n", XML_ErrorString(XML_GetErrorCode(doc->parser)));
  }

  return TRUE;
}

SbBool
cc_xml_doc_parse_buffer_partial_done_x(cc_xml_doc * doc)
{
#ifdef DEV_DEBUG
  fprintf(stdout, "cc_xml_doc_parse_buffer_partial_done_x()\n");
#endif // DEV_DEBUG
  XML_Status ok = XML_Parse(doc->parser, "", 0, TRUE);
  if (!ok) {
    fprintf(stdout, "parse error: %s\n", XML_ErrorString(XML_GetErrorCode(doc->parser)));
  }
  cc_xml_doc_delete_parser_x(doc);
  return TRUE;
}

// *************************************************************************

void
cc_xml_doc_set_filename_x(cc_xml_doc * doc, const char * path)
{
  assert(doc);
  if (doc->filename) {
    delete [] doc->filename;
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

SbBool
cc_xml_doc_write_to_buffer(const cc_xml_doc * doc, char *& buffer, size_t & bytes)
{
  assert(doc);
  bytes = static_cast<int>(cc_xml_doc_calculate_size(doc));
  buffer = new char [ bytes + 1 ];

  // buffer += 
  if (doc->root)
    cc_xml_elt_write_to_buffer(doc->root, buffer, bytes, 0, 2);

  return TRUE;
}

SbBool
cc_xml_doc_write_to_file(cc_xml_doc * doc, const char * path)
{
  assert(doc);
  assert(path);;

  size_t bufsize = 0;
  boost::scoped_array<char> buffer;
  {
    char * bufptr = NULL;
    if (!cc_xml_doc_write_to_buffer(doc, bufptr, bufsize)) {
      return FALSE;
    }
    assert(bufptr);
    buffer.reset(bufptr);
  }

  const size_t bytes = strlen(buffer.get());
  assert(bufsize == bytes);
  FILE * fp = NULL;
  if (strcmp(path, "-") == 0)
    fp = stdout;
  else
    fp = fopen(path, "wb");
  assert(fp != NULL);
  fwrite(buffer.get(), 1, bufsize, fp);
  if (strcmp(path, "-") != 0)
    fclose(fp);

  return TRUE;
}

// *************************************************************************

/*!
  Compare document DOM against other document DOM, and return path to first
  difference.  Returns NULL if documents are equal.  To be used mostly for
  testing the XML I/O code.
*/

cc_xml_path *
cc_xml_doc_diff(const cc_xml_doc * doc, const cc_xml_doc * other)
{
  return NULL;
}

// In documentp.h
size_t
cc_xml_doc_calculate_size(const cc_xml_doc * doc)
{
  size_t bytes = 0;
  // FIXME: sum up XML header
  if (doc->root) {
    bytes += cc_xml_elt_calculate_size(doc->root, 0, 2);
  }
  return bytes;
}

// *************************************************************************

#ifdef COIN_TEST_SUITE

#include <boost/scoped_array.hpp>
#include <Inventor/C/XML/parser.h>
#include <Inventor/C/XML/path.h>

BOOST_AUTO_TEST_CASE(bufread)
{
  const char * buffer =
"<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
"<test value=\"one\" compact=\"\">\n"
"  <b>hei</b>\n"
"</test>\n";
  cc_xml_doc * doc1 = cc_xml_read_buffer(buffer);
  BOOST_CHECK_MESSAGE(doc1 != NULL, "cc_xml_doc_read_buffer() failed");

  boost::scoped_array<char> buffer2;
  size_t bytecount = 0;
  {
    char * bufptr = NULL;
    cc_xml_doc_write_to_buffer(doc1, bufptr, bytecount);
    buffer2.reset(bufptr);
  }
  cc_xml_doc * doc2 = cc_xml_read_buffer(buffer2.get());

  cc_xml_path * diffpath = cc_xml_doc_diff(doc1, doc2);
  BOOST_CHECK_MESSAGE(diffpath == NULL, "document read->write->read DOM differences");

  cc_xml_doc_delete_x(doc1);
  cc_xml_doc_delete_x(doc2);
}

#endif // !COIN_TEST_SUITE
