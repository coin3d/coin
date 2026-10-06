#include <Inventor/C/XML/document.h>
#include <Inventor/C/XML/element.h>

#include <cstdio>
#include <cstring>

#define CHECK(condition, code)                                            \
  do { if (!(condition)) {                                                \
    std::fprintf(stderr, "FAIL[%d]: %s\n", code, #condition);           \
    result = code; goto cleanup;                                          \
  } } while (0)

static SbBool
read_xml(cc_xml_doc * doc, const char * xml)
{
  return cc_xml_doc_read_buffer_x(doc, xml, std::strlen(xml));
}

int
main(void)
{
  int result = 0;
  int closestatus = 0;
  cc_xml_doc * doc = cc_xml_doc_new();
  cc_xml_elt * oldroot = NULL;
  cc_xml_limits limits = { 0, 0, 0, 0, 0 };
  cc_xml_limits observed = { 0, 0, 0, 0, 0 };
  char filepath[1024] = { 0 };
  FILE * fp = NULL;

  CHECK(doc != NULL, 1);
  CHECK(read_xml(doc, "<old/>"), 2);
  oldroot = cc_xml_doc_get_root(doc);

  limits.input_bytes = 4;
  CHECK(cc_xml_doc_set_limits_x(doc, &limits), 3);
  cc_xml_doc_get_limits(doc, &observed);
  CHECK(observed.input_bytes == 4, 4);
  CHECK(read_xml(doc, "<a/>"), 5);
  oldroot = cc_xml_doc_get_root(doc);
  CHECK(cc_xml_doc_get_limit_hit(doc) == CC_XML_LIMIT_NONE, 6);
  CHECK(!read_xml(doc, "<ab/>"), 7);
  CHECK(cc_xml_doc_get_limit_hit(doc) == CC_XML_LIMIT_INPUT_BYTES, 8);
  CHECK(cc_xml_doc_get_root(doc) == oldroot, 9);

  CHECK(cc_xml_doc_parse_buffer_partial_x(doc, "<a", 2), 10);
  CHECK(cc_xml_doc_parse_buffer_partial_done_x(doc, "/>", 2), 11);
  oldroot = cc_xml_doc_get_root(doc);
  CHECK(cc_xml_doc_parse_buffer_partial_x(doc, "<a", 2), 12);
  CHECK(!cc_xml_doc_parse_buffer_partial_done_x(doc, " />", 3), 13);
  CHECK(cc_xml_doc_get_limit_hit(doc) == CC_XML_LIMIT_INPUT_BYTES, 14);
  CHECK(cc_xml_doc_get_root(doc) == oldroot, 15);

  limits.input_bytes = 0;
  limits.elements = 2;
  CHECK(cc_xml_doc_set_limits_x(doc, &limits), 16);
  CHECK(read_xml(doc, "<a><b/></a>"), 17);
  oldroot = cc_xml_doc_get_root(doc);
  limits.elements = 1;
  CHECK(cc_xml_doc_set_limits_x(doc, &limits), 18);
  CHECK(!read_xml(doc, "<a><b/></a>"), 19);
  CHECK(cc_xml_doc_get_limit_hit(doc) == CC_XML_LIMIT_ELEMENTS, 20);
  CHECK(cc_xml_doc_get_root(doc) == oldroot, 21);

  limits.elements = 0;
  limits.depth = 2;
  CHECK(cc_xml_doc_set_limits_x(doc, &limits), 22);
  CHECK(read_xml(doc, "<a><b/></a>"), 23);
  oldroot = cc_xml_doc_get_root(doc);
  limits.depth = 1;
  CHECK(cc_xml_doc_set_limits_x(doc, &limits), 24);
  CHECK(!read_xml(doc, "<a><b/></a>"), 25);
  CHECK(cc_xml_doc_get_limit_hit(doc) == CC_XML_LIMIT_DEPTH, 26);
  CHECK(cc_xml_doc_get_root(doc) == oldroot, 27);

  limits.depth = 0;
  limits.attributes = 2;
  CHECK(cc_xml_doc_set_limits_x(doc, &limits), 28);
  CHECK(read_xml(doc, "<a x=\"1\" y=\"2\"/>"), 29);
  oldroot = cc_xml_doc_get_root(doc);
  limits.attributes = 1;
  CHECK(cc_xml_doc_set_limits_x(doc, &limits), 30);
  CHECK(!read_xml(doc, "<a x=\"1\" y=\"2\"/>"), 31);
  CHECK(cc_xml_doc_get_limit_hit(doc) == CC_XML_LIMIT_ATTRIBUTES, 32);
  CHECK(cc_xml_doc_get_root(doc) == oldroot, 33);

  limits.attributes = 0;
  limits.expanded_bytes = 3; // element name and expanded character data
  CHECK(cc_xml_doc_set_limits_x(doc, &limits), 34);
  CHECK(read_xml(doc, "<a>ab</a>"), 35);
  oldroot = cc_xml_doc_get_root(doc);
  limits.expanded_bytes = 2;
  CHECK(cc_xml_doc_set_limits_x(doc, &limits), 36);
  CHECK(!read_xml(doc, "<a>ab</a>"), 37);
  CHECK(cc_xml_doc_get_limit_hit(doc) == CC_XML_LIMIT_EXPANDED_BYTES, 38);
  CHECK(cc_xml_doc_get_root(doc) == oldroot, 39);

  limits.expanded_bytes = 3;
  CHECK(cc_xml_doc_set_limits_x(doc, &limits), 40);
  CHECK(read_xml(doc, "<!DOCTYPE a [<!ENTITY x 'xy'>]><a>&x;</a>"), 41);
  oldroot = cc_xml_doc_get_root(doc);
  limits.expanded_bytes = 2;
  CHECK(cc_xml_doc_set_limits_x(doc, &limits), 42);
  CHECK(!read_xml(doc, "<!DOCTYPE a [<!ENTITY x 'xy'>]><a>&x;</a>"), 43);
  CHECK(cc_xml_doc_get_limit_hit(doc) == CC_XML_LIMIT_EXPANDED_BYTES, 44);
  CHECK(cc_xml_doc_get_root(doc) == oldroot, 45);

  limits.expanded_bytes = 3;
  CHECK(cc_xml_doc_set_limits_x(doc, &limits), 60);
  CHECK(cc_xml_doc_parse_buffer_partial_x(doc, "<a>a", 4), 61);
  CHECK(cc_xml_doc_parse_buffer_partial_done_x(doc, "b</a>", 5), 62);
  oldroot = cc_xml_doc_get_root(doc);
  limits.expanded_bytes = 2;
  CHECK(cc_xml_doc_set_limits_x(doc, &limits), 63);
  CHECK(cc_xml_doc_parse_buffer_partial_x(doc, "<a>a", 4), 64);
  CHECK(!cc_xml_doc_parse_buffer_partial_done_x(doc, "b</a>", 5), 65);
  CHECK(cc_xml_doc_get_limit_hit(doc) == CC_XML_LIMIT_EXPANDED_BYTES, 66);
  CHECK(cc_xml_doc_get_root(doc) == oldroot, 67);

  limits.expanded_bytes = 3; // element, attribute name, attribute value
  CHECK(cc_xml_doc_set_limits_x(doc, &limits), 68);
  CHECK(read_xml(doc, "<a x='b'/>"), 69);
  oldroot = cc_xml_doc_get_root(doc);
  limits.expanded_bytes = 2;
  CHECK(cc_xml_doc_set_limits_x(doc, &limits), 70);
  CHECK(!read_xml(doc, "<a x='b'/>"), 71);
  CHECK(cc_xml_doc_get_limit_hit(doc) == CC_XML_LIMIT_EXPANDED_BYTES, 72);
  CHECK(cc_xml_doc_get_root(doc) == oldroot, 73);

  limits.expanded_bytes = 0;
  limits.input_bytes = 4;
  CHECK(cc_xml_doc_set_limits_x(doc, &limits), 46);
  CHECK(cc_xml_doc_parse_buffer_partial_x(doc, "<a", 2), 47);
  CHECK(!cc_xml_doc_set_limits_x(doc, &limits), 48);
  CHECK(cc_xml_doc_parse_buffer_partial_done_x(doc, "/>", 2), 49);

  CHECK(std::snprintf(filepath, sizeof(filepath), "%s/limited.xml",
                      COIN_XML_TEST_DIRECTORY) > 0, 50);
  fp = std::fopen(filepath, "wb");
  CHECK(fp != NULL, 51);
  CHECK(std::fwrite("<ab/>", 1, 5, fp) == 5, 52);
  closestatus = std::fclose(fp);
  fp = NULL;
  CHECK(closestatus == 0, 53);
  oldroot = cc_xml_doc_get_root(doc);
  CHECK(!cc_xml_doc_read_file_x(doc, filepath), 54);
  CHECK(cc_xml_doc_get_limit_hit(doc) == CC_XML_LIMIT_INPUT_BYTES, 55);
  CHECK(cc_xml_doc_get_root(doc) == oldroot, 56);
  limits.input_bytes = 5;
  CHECK(cc_xml_doc_set_limits_x(doc, &limits), 57);
  CHECK(cc_xml_doc_read_file_x(doc, filepath), 58);
  CHECK(cc_xml_doc_get_limit_hit(doc) == CC_XML_LIMIT_NONE, 59);

  fp = std::fopen(filepath, "wb");
  CHECK(fp != NULL, 74);
  CHECK(std::fwrite("<a>", 1, 3, fp) == 3, 75);
  for (int i = 0; i < 9000; ++i) {
    CHECK(std::fputc('x', fp) != EOF, 76);
  }
  CHECK(std::fwrite("</a>", 1, 4, fp) == 4, 77);
  closestatus = std::fclose(fp);
  fp = NULL;
  CHECK(closestatus == 0, 78);
  limits.input_bytes = 9007;
  CHECK(cc_xml_doc_set_limits_x(doc, &limits), 79);
  CHECK(cc_xml_doc_read_file_x(doc, filepath), 80);
  oldroot = cc_xml_doc_get_root(doc);
  limits.input_bytes = 9006;
  CHECK(cc_xml_doc_set_limits_x(doc, &limits), 81);
  CHECK(!cc_xml_doc_read_file_x(doc, filepath), 82);
  CHECK(cc_xml_doc_get_limit_hit(doc) == CC_XML_LIMIT_INPUT_BYTES, 83);
  CHECK(cc_xml_doc_get_root(doc) == oldroot, 84);

cleanup:
  if (fp) std::fclose(fp);
  if (filepath[0] != '\0') std::remove(filepath);
  if (doc) cc_xml_doc_delete_x(doc);
  return result;
}
