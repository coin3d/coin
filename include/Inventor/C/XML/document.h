#ifndef COIN_XMLDOCUMENT_H
#define COIN_XMLDOCUMENT_H

/**************************************************************************\
 * Copyright (c) Kongsberg Oil & Gas Technologies AS
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 * 
 * Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 * 
 * Redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the distribution.
 * 
 * Neither the name of the copyright holder nor the names of its
 * contributors may be used to endorse or promote products derived from
 * this software without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
\**************************************************************************/

#include <Inventor/C/XML/types.h>

/* ********************************************************************** */

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* basic construction
   A document returned by new() is caller-owned.  delete_x() also deletes the
   root tree still owned by the document. */
COIN_DLL_API cc_xml_doc * cc_xml_doc_new(void);
COIN_DLL_API void cc_xml_doc_delete_x(cc_xml_doc * doc);

/* parser configuration */
COIN_DLL_API void cc_xml_doc_set_filter_cb_x(cc_xml_doc * doc, cc_xml_filter_cb * cb, void * userdata);
COIN_DLL_API void cc_xml_doc_get_filter_cb(const cc_xml_doc * doc, cc_xml_filter_cb ** cb, void ** userdata);

/*! \brief Optional per-document XML parsing limits.

  These limits let an application bound selected resource growth while parsing
  XML. Every member is a quota for one complete parse session. A value of zero
  disables that quota, and a newly created document has all quotas disabled for
  compatibility with the legacy API.

  Counters restart when a new file, buffer, or partial parsing session begins.
  Elements and attributes are counted before filtering, so discarding an
  element does not refund the budget.

  These quotas are not a universal security profile. Applications should
  select values appropriate for the trust level and expected size of their
  documents.

  \ingroup coin_XML
  \relates cc_xml_doc
*/
typedef struct cc_xml_limits {
  /*! Cumulative input bytes supplied during the complete parse session,
      including all file blocks or partial parsing chunks. */
  size_t input_bytes;

  /*! Bytes reported by Expat for element and attribute names, attribute
      values, and character data. Closing tags are not counted. */
  size_t expanded_bytes;

  /*! Total element start events, including elements later discarded by the
      document filter. Character-data nodes are not counted as elements. */
  size_t elements;

  /*! Total attributes reported on element start events, including attributes
      on elements later discarded by the document filter. */
  size_t attributes;

  /*! Maximum element nesting depth. The document root has depth 1. */
  size_t depth;
} cc_xml_limits;

/*! \brief Identifies the first XML parsing limit reached in a session.

  \ingroup coin_XML
  \relates cc_xml_doc
*/
typedef enum cc_xml_limit_hit {
  /*! No configured limit has been reached in the current or most recent
      parse session. */
  CC_XML_LIMIT_NONE,
  /*! The cumulative input byte quota was reached. */
  CC_XML_LIMIT_INPUT_BYTES,
  /*! The expanded byte quota was reached. */
  CC_XML_LIMIT_EXPANDED_BYTES,
  /*! The element quota was reached. */
  CC_XML_LIMIT_ELEMENTS,
  /*! The attribute quota was reached. */
  CC_XML_LIMIT_ATTRIBUTES,
  /*! The element nesting depth quota was reached. */
  CC_XML_LIMIT_DEPTH
} cc_xml_limit_hit;

COIN_DLL_API SbBool cc_xml_doc_set_limits_x(cc_xml_doc * doc, const cc_xml_limits * limits);
COIN_DLL_API void cc_xml_doc_get_limits(const cc_xml_doc * doc, cc_xml_limits * limits);
COIN_DLL_API cc_xml_limit_hit cc_xml_doc_get_limit_hit(const cc_xml_doc * doc);

/* document io */
COIN_DLL_API SbBool cc_xml_doc_read_file_x(cc_xml_doc * doc, const char * path);
COIN_DLL_API SbBool cc_xml_doc_read_buffer_x(cc_xml_doc * doc, const char * buffer, size_t buflen);

COIN_DLL_API SbBool cc_xml_doc_parse_buffer_partial_x(cc_xml_doc * doc, const char * buffer, size_t buflen);
COIN_DLL_API SbBool cc_xml_doc_parse_buffer_partial_done_x(cc_xml_doc * doc, const char * buffer, size_t buflen);

COIN_DLL_API SbBool cc_xml_doc_write_to_buffer(const cc_xml_doc * doc, char ** buffer, size_t * bytes);
COIN_DLL_API SbBool cc_xml_doc_write_to_file(const cc_xml_doc * doc, const char * path);

COIN_DLL_API cc_xml_path * cc_xml_doc_diff(const cc_xml_doc * doc, const cc_xml_doc * other);

/* document attributes */
COIN_DLL_API void cc_xml_doc_set_filename_x(cc_xml_doc * doc, const char * path);
COIN_DLL_API const char * cc_xml_doc_get_filename(const cc_xml_doc * doc);

/* misc... */

/* Root ownership:
   - set_root_x() transfers ownership of root to doc.
   - get_root() returns a borrowed pointer.
   - release_root_x() transfers ownership from doc to the caller.
   Replacing a non-NULL root with set_root_x() deletes the previous tree.
   Call release_root_x() first if the previous tree must be retained. */
COIN_DLL_API cc_xml_elt *   cc_xml_doc_get_root(const cc_xml_doc * doc);
COIN_DLL_API cc_xml_elt *   cc_xml_doc_release_root_x(cc_xml_doc * doc);
COIN_DLL_API void           cc_xml_doc_set_current_x(cc_xml_doc * doc, cc_xml_elt * elt);
COIN_DLL_API cc_xml_elt *   cc_xml_doc_get_current(const cc_xml_doc * doc);
COIN_DLL_API void           cc_xml_doc_strip_whitespace_x(cc_xml_doc * doc);

COIN_DLL_API void           cc_xml_doc_set_root_x(cc_xml_doc * doc, cc_xml_elt * root);

COIN_DLL_API const cc_xml_elt * cc_xml_doc_find_element(const cc_xml_doc * doc, cc_xml_path * path);
COIN_DLL_API const cc_xml_elt * cc_xml_doc_find_next_element(const cc_xml_doc * doc, cc_xml_elt * elt, cc_xml_path * path);
COIN_DLL_API cc_xml_elt *       cc_xml_doc_create_element_x(cc_xml_doc * doc, cc_xml_path * path);


#ifdef __cplusplus
} /* extern "C" */

inline void
cc_xml_doc_get_filter_cb(const cc_xml_doc * doc, cc_xml_filter_cb *& cb, void *& userdata)
{
  cc_xml_doc_get_filter_cb(doc, &cb, &userdata);
}

inline SbBool
cc_xml_doc_write_to_buffer(const cc_xml_doc * doc, char *& buffer, size_t & bytes)
{
  return cc_xml_doc_write_to_buffer(doc, &buffer, &bytes);
}
#endif /* __cplusplus */

#endif /* !COIN_XML_DOCUMENT_H */
