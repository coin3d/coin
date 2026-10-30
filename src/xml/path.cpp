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

#include <Inventor/C/XML/path.h>

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif // HAVE_CONFIG_H

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cassert>

#include <Inventor/C/base/string.h>
#include <Inventor/C/XML/element.h>
#include "utils.h"

/* ********************************************************************** */

struct cc_xml_path {
  struct path_node * head;
};

/* ********************************************************************** */

struct path_node {
  char * element;
  int idx;
  struct path_node * next;
  // make list doubly-linked for match_p use?
};

static
path_node *
path_node_new(const char * element, int idx)
{
  path_node * node = new path_node;
  node->element = cc_xml_strdup(element);
  node->idx = idx;
  node->next = NULL;
  return node;
}

static
path_node *
path_node_clone(const path_node * node)
{
  return path_node_new(node->element, node->idx);
}

static
path_node *
path_node_clone_chain(const path_node * node, path_node ** tail)
{
  path_node * head = NULL;
  path_node * currenttail = NULL;
  while (node != NULL) {
    path_node * clone = path_node_clone(node);
    if (currenttail == NULL) head = clone;
    else currenttail->next = clone;
    currenttail = clone;
    node = node->next;
  }
  if (tail != NULL) *tail = currenttail;
  return head;
}

static
void
path_node_delete_chain(path_node * head)
{
  while ( head != NULL ) {
    path_node * prev = head;
    head = head->next;
    delete[] prev->element;
    delete prev;
  }
}

static
int
path_node_match_p(path_node * node, const cc_xml_elt * elt)
{
  if ( strcmp(node->element, cc_xml_elt_get_type(elt)) != 0 ) return FALSE;
  if ( node->idx != -1 ) {
    cc_xml_elt * parent = cc_xml_elt_get_parent(elt);
    if ( parent == NULL ) return (node->idx == 0) ? TRUE : FALSE;
    int i = cc_xml_elt_get_child_type_index(parent, elt);
    return (i == node->idx) ? TRUE : FALSE;
  }
  return TRUE;
}

/* ********************************************************************** */

cc_xml_path *
cc_xml_path_new(void)
{
  cc_xml_path * path = new cc_xml_path;
  path->head = NULL;
  assert(path);
  return path;
} // cc_xml_path_new()

void
cc_xml_path_delete_x(cc_xml_path * path)
{
  assert(path);
  path_node_delete_chain(path->head);
  delete path;
} // cc_xml_path_delete_x()

void
cc_xml_path_clear_x(cc_xml_path * path)
{
  assert(path);
  path_node_delete_chain(path->head);
  path->head = NULL;
} // cc_xml_path_clear_x()

static
void
cc_xml_path_set_x_va(cc_xml_path * path, va_list args)
{
  path_node_delete_chain(path->head);
  path->head = NULL;
  char * arg;
  while ( (arg = va_arg(args, char *)) != NULL ) {
    char * ptr;
    if ( (ptr = strchr(arg, '[')) == NULL ) {
      cc_xml_path_prepend_x(path, arg, -1);
    } else {
      int idx = atoi(ptr + 1);
      assert(idx >= -1);
      char * str = cc_xml_strndup(arg, static_cast<int>(ptr - arg));
      cc_xml_path_prepend_x(path, str, idx);
      delete [] str;
    }
  }
  cc_xml_path_reverse_x(path);
} // cc_xml_path_set_x_va()

void
cc_xml_path_copy_x(cc_xml_path * path, cc_xml_path * path2)
{
  assert(path && path2);
  path_node * head = path_node_clone_chain(path2->head, NULL);
  path_node_delete_chain(path->head);
  path->head = head;
} // cc_xml_path_copy_x()

void
cc_xml_path_set_x(cc_xml_path * path, ...)
{
  assert(path);
  va_list args;
  va_start(args, path);
  cc_xml_path_set_x_va(path, args);
  va_end(args);
} // cc_xml_path_set_x()

void
cc_xml_path_reverse_x(cc_xml_path * path)
{
  assert(path);
  struct path_node * prev = NULL;
  struct path_node * node = path->head;
  while ( node != NULL ) {
    struct path_node * next = node->next;
    node->next = prev;
    prev = node;
    node = next;
  }
  path->head = prev;
} // cc_xml_path_reverse_x()

int
cc_xml_path_get_length(const cc_xml_path * path)
{
  assert(path);
  int len = 0;
  struct path_node * node = path->head;
  while ( node != NULL ) {
    len++;
    node = node->next;
  }
  return len;
} // cc_xml_path_get_length()

const char *
cc_xml_path_get_type(const cc_xml_path * path, int idx)
{
  assert(path && path->head && idx >= 0);
  struct path_node * node = path->head;
  int i;
  for ( i = 0; i < idx; i++ ) {
    assert(node->next != NULL);
    node = node->next;
  }
  return node->element;
} // cc_xml_path_get_type()

int
cc_xml_path_get_index(const cc_xml_path * path, int idx)
{
  assert(path && path->head && idx >= 0);
  struct path_node * node = path->head;
  int i;
  for ( i = 0; i < idx; i++ ) {
    assert(node->next != NULL);
    node = node->next;
  }
  return node->idx;
} // cc_xml_path_get_index()

int
cc_xml_path_match_p(const cc_xml_path * path, const cc_xml_elt * elt)
{
  // consider using a doubly-linked list instead of continuous head->tail traversal
  int length = cc_xml_path_get_length(path);
  struct path_node * head = path->head;
  int i, j;
  for ( i = length - 1; i >= 0; i-- ) {
    struct path_node * node = head;
    for ( j = 0; j < i; j++ ) {
      node = node->next;
      assert(node);
    }
    if ( !path_node_match_p(node, elt) ) return FALSE;
    elt = cc_xml_elt_get_parent(elt);
    assert(elt);
  }
  return TRUE;
} // cc_xml_path_match_p()

void
cc_xml_path_append_x(cc_xml_path * path, const char * elt, int idx)
{
  assert(path);
  struct path_node * node = path->head;
  if ( node == NULL ) {
    path->head = path_node_new(elt, idx);
  } else {
    while ( node->next != NULL )
      node = node->next;
    node->next = path_node_new(elt, idx);
  }
} // cc_xml_path_append_x()

void
cc_xml_path_append_path_x(cc_xml_path * path, cc_xml_path * path2)
{
  assert(path && path2);
  path_node * sourcehead = path_node_clone_chain(path2->head, NULL);
  if (sourcehead == NULL) return;

  path_node * tail = path->head;
  if (tail == NULL) path->head = sourcehead;
  else {
    while (tail->next != NULL) tail = tail->next;
    tail->next = sourcehead;
  }
} // cc_xml_path_append_path_x()

void
cc_xml_path_prepend_x(cc_xml_path * path, const char * elt, int idx)
{
  assert(path);
  struct path_node * node = path->head;
  path->head = path_node_new(elt, idx);
  path->head->next = node;
} // cc_xml_path_prepend_x()

void
cc_xml_path_prepend_path_x(cc_xml_path * path, cc_xml_path * path2)
{
  assert(path && path2);
  path_node * sourcetail = NULL;
  path_node * sourcehead = path_node_clone_chain(path2->head, &sourcetail);
  if (sourcehead != NULL) {
    sourcetail->next = path->head;
    path->head = sourcehead;
  }
} // cc_xml_path_prepend_path_x()

void
cc_xml_path_truncate_x(cc_xml_path * path, int length)
{
  assert(path);
  assert(length >= 0);
  if (length <= 0) {
    cc_xml_path_clear_x(path);
    return;
  }

  int len = 1;
  struct path_node * node = path->head;
  while ( (node != NULL) && (len < length) ) {
    len++;
    node = node->next;
  }
  if ( node != NULL ) {
    path_node * discarded = node->next;
    node->next = NULL;
    path_node_delete_chain(discarded);
  }
} // cc_xml_path_truncate_x()

/* ********************************************************************** */

COIN_DLL_API void
cc_xml_path_dump(cc_xml_path * path)
{
  assert(path);
  struct path_node * node = path->head;
  while ( node != NULL ) {
    if ( node != path->head )
      fprintf(stderr, ".");
    fprintf(stderr, "%s", node->element);
    if ( node->idx != -1 ) {
      fprintf(stderr, "[%d]", node->idx);
    }
    node = node->next;
  }
  fprintf(stderr, "\n");
}

void
cc_xml_path_dump(const cc_xml_path * path)
{
  cc_xml_path_dump(const_cast<cc_xml_path *>(path));
}

/* ********************************************************************** */

#ifdef COIN_TEST_SUITE

#include <cstring>

BOOST_AUTO_TEST_CASE(cc_xml_path_copy_and_combine)
{
  cc_xml_path * source = cc_xml_path_new();
  cc_xml_path_append_x(source, "a", 0);
  cc_xml_path_append_x(source, "b", 1);

  cc_xml_path * destination = cc_xml_path_new();
  cc_xml_path_append_x(destination, "old", 2);
  cc_xml_path_copy_x(destination, source);
  BOOST_REQUIRE_EQUAL(cc_xml_path_get_length(destination), 2);
  BOOST_CHECK_EQUAL(strcmp(cc_xml_path_get_type(destination, 0), "a"), 0);
  BOOST_CHECK_EQUAL(cc_xml_path_get_index(destination, 1), 1);

  cc_xml_path_copy_x(destination, destination);
  BOOST_REQUIRE_EQUAL(cc_xml_path_get_length(destination), 2);
  BOOST_CHECK_EQUAL(strcmp(cc_xml_path_get_type(destination, 1), "b"), 0);

  cc_xml_path_append_path_x(destination, source);
  BOOST_REQUIRE_EQUAL(cc_xml_path_get_length(destination), 4);
  BOOST_CHECK_EQUAL(strcmp(cc_xml_path_get_type(destination, 2), "a"), 0);
  BOOST_CHECK_EQUAL(strcmp(cc_xml_path_get_type(destination, 3), "b"), 0);
  BOOST_CHECK_EQUAL(cc_xml_path_get_length(source), 2);

  cc_xml_path_append_path_x(source, source);
  BOOST_REQUIRE_EQUAL(cc_xml_path_get_length(source), 4);
  BOOST_CHECK_EQUAL(strcmp(cc_xml_path_get_type(source, 2), "a"), 0);
  BOOST_CHECK_EQUAL(strcmp(cc_xml_path_get_type(source, 3), "b"), 0);

  cc_xml_path_prepend_path_x(destination, source);
  BOOST_REQUIRE_EQUAL(cc_xml_path_get_length(destination), 8);
  BOOST_CHECK_EQUAL(strcmp(cc_xml_path_get_type(destination, 0), "a"), 0);
  BOOST_CHECK_EQUAL(strcmp(cc_xml_path_get_type(destination, 4), "a"), 0);

  cc_xml_path_prepend_path_x(source, source);
  BOOST_REQUIRE_EQUAL(cc_xml_path_get_length(source), 8);
  BOOST_CHECK_EQUAL(strcmp(cc_xml_path_get_type(source, 4), "a"), 0);

  cc_xml_path * empty = cc_xml_path_new();
  cc_xml_path_append_path_x(destination, empty);
  cc_xml_path_prepend_path_x(destination, empty);
  BOOST_CHECK_EQUAL(cc_xml_path_get_length(destination), 8);

  cc_xml_path_append_path_x(empty, source);
  BOOST_CHECK_EQUAL(cc_xml_path_get_length(empty), 8);
  cc_xml_path_clear_x(empty);
  cc_xml_path_prepend_path_x(empty, source);
  BOOST_CHECK_EQUAL(cc_xml_path_get_length(empty), 8);

  cc_xml_path * blank = cc_xml_path_new();
  cc_xml_path_copy_x(empty, blank);
  BOOST_CHECK_EQUAL(cc_xml_path_get_length(empty), 0);

  cc_xml_path_delete_x(blank);
  cc_xml_path_delete_x(empty);
  cc_xml_path_delete_x(destination);
  cc_xml_path_delete_x(source);
}

BOOST_AUTO_TEST_CASE(cc_xml_path_truncate_exact_length)
{
  cc_xml_path * path = cc_xml_path_new();
  for (int i = 0; i < 128; i++) {
    cc_xml_path_append_x(path, "node", i);
  }
  BOOST_REQUIRE_EQUAL(cc_xml_path_get_length(path), 128);

  cc_xml_path_truncate_x(path, 128);
  BOOST_CHECK_EQUAL(cc_xml_path_get_length(path), 128);
  cc_xml_path_truncate_x(path, 256);
  BOOST_CHECK_EQUAL(cc_xml_path_get_length(path), 128);

  cc_xml_path_truncate_x(path, 64);
  BOOST_REQUIRE_EQUAL(cc_xml_path_get_length(path), 64);
  BOOST_CHECK_EQUAL(cc_xml_path_get_index(path, 63), 63);

  cc_xml_path_truncate_x(path, 1);
  BOOST_REQUIRE_EQUAL(cc_xml_path_get_length(path), 1);
  BOOST_CHECK_EQUAL(cc_xml_path_get_index(path, 0), 0);

  cc_xml_path_truncate_x(path, 0);
  BOOST_CHECK_EQUAL(cc_xml_path_get_length(path), 0);
  cc_xml_path_truncate_x(path, 0);
  BOOST_CHECK_EQUAL(cc_xml_path_get_length(path), 0);

  cc_xml_path_delete_x(path);
}

#endif // COIN_TEST_SUITE
