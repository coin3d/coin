#include <cstddef>

#include <Inventor/C/XML/document.h>

struct cc_xml_path;

extern "C" COIN_DLL_API cc_xml_path * cc_xml_path_new(void);
extern "C" COIN_DLL_API void cc_xml_path_delete_x(cc_xml_path * path);
extern "C" COIN_DLL_API void cc_xml_path_append_x(cc_xml_path * path,
                                                    const char * elt,
                                                    int idx);

// This non-const overload is the historical C++-linkage symbol. It is
// deliberately declared without path.h, whose public declaration is const.
COIN_DLL_API void cc_xml_path_dump(cc_xml_path * path);

static cc_xml_filter_choice
discard_filter(void *, cc_xml_doc *, cc_xml_elt *, int)
{
  return DISCARD;
}

int
main()
{
  int marker = 7;
  cc_xml_doc * doc = cc_xml_doc_new();
  cc_xml_filter_cb * callback = NULL;
  void * userdata = NULL;
  char * buffer = NULL;
  size_t bytes = 0;

  if (doc == NULL) return 1;
  cc_xml_doc_set_filter_cb_x(doc, discard_filter, &marker);
  cc_xml_doc_get_filter_cb(doc, callback, userdata);
  if (callback != discard_filter || userdata != &marker) return 2;

  if (!cc_xml_doc_write_to_buffer(doc, buffer, bytes)) return 3;
  if (buffer == NULL || bytes == 0 || buffer[bytes] != '\0') return 4;
  delete[] buffer;
  cc_xml_doc_delete_x(doc);

  cc_xml_path * path = cc_xml_path_new();
  if (path == NULL) return 5;
  cc_xml_path_append_x(path, "legacy", -1);
  cc_xml_path_dump(path);
  cc_xml_path_delete_x(path);
  return 0;
}
