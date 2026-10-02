#include <string.h>

#include <Inventor/C/XML/document.h>
#include <Inventor/C/XML/path.h>

#ifdef __cplusplus
#error "This regression client must be compiled as C"
#endif

static cc_xml_filter_choice
keep_filter(void * userdata, cc_xml_doc * doc, cc_xml_elt * elt, int pushing)
{
  (void)userdata;
  (void)doc;
  (void)elt;
  (void)pushing;
  return KEEP;
}

int
main(void)
{
  int marker = 42;
  cc_xml_doc * doc = cc_xml_doc_new();
  cc_xml_filter_cb * callback = NULL;
  void * userdata = NULL;
  cc_xml_path * path;
  const cc_xml_path * constpath;
  SbBool (*write_to_buffer)(const cc_xml_doc *, char **, size_t *);

  if (doc == NULL) return 1;
  cc_xml_doc_set_filter_cb_x(doc, keep_filter, &marker);

  cc_xml_doc_get_filter_cb(doc, &callback, &userdata);
  if (callback != keep_filter || userdata != &marker) return 2;

  userdata = NULL;
  cc_xml_doc_get_filter_cb(doc, NULL, &userdata);
  if (userdata != &marker) return 3;

  callback = NULL;
  cc_xml_doc_get_filter_cb(doc, &callback, NULL);
  if (callback != keep_filter) return 4;

  cc_xml_doc_get_filter_cb(doc, NULL, NULL);

  // Taking this address verifies the C form of both output parameters
  // without allocating a buffer whose ownership is outside this test's scope.
  write_to_buffer = cc_xml_doc_write_to_buffer;
  if (write_to_buffer == NULL) return 5;

  path = cc_xml_path_new();
  if (path == NULL) return 6;
  cc_xml_path_append_x(path, "root", 0);
  if (cc_xml_path_get_length(path) != 1) return 7;
  if (strcmp(cc_xml_path_get_type(path, 0), "root") != 0) return 8;
  if (cc_xml_path_get_index(path, 0) != 0) return 9;

  constpath = path;
  cc_xml_path_dump(constpath);

  cc_xml_path_delete_x(path);
  cc_xml_doc_delete_x(doc);
  return 0;
}
