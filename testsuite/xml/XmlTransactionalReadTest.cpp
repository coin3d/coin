#include <Inventor/C/XML/document.h>
#include <Inventor/C/XML/element.h>

#include <cstdio>
#include <cstring>
#include <climits>

#define REQUIRE(condition, code)                                           \
  do {                                                                     \
    if (!(condition)) {                                                    \
      std::fprintf(stderr, "FAIL[%d]: %s\n", code, #condition);           \
      result = code;                                                       \
      goto cleanup;                                                        \
    }                                                                      \
  } while (0)

static SbBool
root_has_type(const cc_xml_doc * doc, const char * type)
{
  const cc_xml_elt * root = cc_xml_doc_get_root(doc);
  return root != NULL && std::strcmp(cc_xml_elt_get_type(root), type) == 0;
}

static cc_xml_filter_choice
set_current_filter(void *, cc_xml_doc * doc, cc_xml_elt * elt, int pushing)
{
  if (pushing) {
    cc_xml_doc_set_current_x(doc, elt);
  }
  return KEEP;
}

static SbBool
write_text_file(const char * path, const char * text)
{
  FILE * fp = std::fopen(path, "wb");
  if (fp == NULL) return FALSE;
  const size_t length = std::strlen(text);
  const size_t written = std::fwrite(text, 1, length, fp);
  const int close_status = std::fclose(fp);
  return written == length && close_status == 0;
}

static SbBool
write_large_file(const char * path, const char * closing)
{
  FILE * fp = std::fopen(path, "wb");
  if (fp == NULL) return FALSE;

  SbBool ok = std::fputs("<large>", fp) >= 0;
  for (int i = 0; i < 9000 && ok; ++i) {
    if (std::fputc('x', fp) == EOF) ok = FALSE;
  }
  if (ok && std::fputs(closing, fp) < 0) ok = FALSE;
  if (std::ferror(fp)) ok = FALSE;
  if (std::fclose(fp) != 0) ok = FALSE;
  return ok;
}

int
main(void)
{
  int result = 0;
  int length;
  const size_t too_large = static_cast<size_t>(INT_MAX) + 1;
  cc_xml_doc * doc = NULL;
  cc_xml_elt * committed = NULL;
  char validpath[1024];
  char malformedpath[1024];
  char emptypath[1024];
  char missingpath[1024];
  char largemalformedpath[1024];
  char largevalidpath[1024];

  length = std::snprintf(validpath, sizeof(validpath),
                         "%s/valid.xml", COIN_XML_TEST_DIRECTORY);
  if (length < 0 || static_cast<size_t>(length) >= sizeof(validpath)) return 90;
  length = std::snprintf(malformedpath, sizeof(malformedpath),
                         "%s/malformed.xml", COIN_XML_TEST_DIRECTORY);
  if (length < 0 || static_cast<size_t>(length) >= sizeof(malformedpath)) return 91;
  length = std::snprintf(emptypath, sizeof(emptypath),
                         "%s/empty.xml", COIN_XML_TEST_DIRECTORY);
  if (length < 0 || static_cast<size_t>(length) >= sizeof(emptypath)) return 92;
  length = std::snprintf(missingpath, sizeof(missingpath),
                         "%s/does-not-exist.xml", COIN_XML_TEST_DIRECTORY);
  if (length < 0 || static_cast<size_t>(length) >= sizeof(missingpath)) return 93;
  length = std::snprintf(largemalformedpath, sizeof(largemalformedpath),
                         "%s/large-malformed.xml", COIN_XML_TEST_DIRECTORY);
  if (length < 0 || static_cast<size_t>(length) >= sizeof(largemalformedpath)) return 93;
  length = std::snprintf(largevalidpath, sizeof(largevalidpath),
                         "%s/large-valid.xml", COIN_XML_TEST_DIRECTORY);
  if (length < 0 || static_cast<size_t>(length) >= sizeof(largevalidpath)) return 94;

  REQUIRE(write_text_file(validpath, "<file-old/>"), 1);
  REQUIRE(write_text_file(malformedpath, "<file-new>"), 2);
  REQUIRE(write_text_file(emptypath, ""), 3);
  REQUIRE(write_large_file(largemalformedpath, "</wrong>"), 4);
  REQUIRE(write_large_file(largevalidpath, "</large>"), 5);

  doc = cc_xml_doc_new();
  REQUIRE(doc != NULL, 6);

  // A failed whole-buffer read must preserve the committed tree and current.
  REQUIRE(cc_xml_doc_read_buffer_x(doc, "<old><child/></old>",
                                   std::strlen("<old><child/></old>")), 10);
  REQUIRE(root_has_type(doc, "old"), 11);
  committed = cc_xml_doc_get_root(doc);
  cc_xml_doc_set_current_x(doc, committed);
  cc_xml_doc_set_filter_cb_x(doc, set_current_filter, NULL);
  REQUIRE(!cc_xml_doc_read_buffer_x(doc, "<new><child></new>",
                                    std::strlen("<new><child></new>")), 12);
  REQUIRE(cc_xml_doc_get_root(doc) == committed, 13);
  REQUIRE(cc_xml_doc_get_current(doc) == committed, 14);
  cc_xml_doc_set_filter_cb_x(doc, NULL, NULL);

  // These lengths cannot be passed to XML_Parse(int).  The short pointer
  // deliberately proves rejection happens before the buffer is accessed.
  REQUIRE(!cc_xml_doc_read_buffer_x(doc, "x", too_large), 70);
  REQUIRE(cc_xml_doc_get_root(doc) == committed, 71);
  REQUIRE(cc_xml_doc_get_current(doc) == committed, 72);
  REQUIRE(!cc_xml_doc_parse_buffer_partial_x(doc, "x", too_large), 73);
  REQUIRE(cc_xml_doc_get_root(doc) == committed, 74);
  REQUIRE(cc_xml_doc_parse_buffer_partial_x(doc, "<staged>",
                                            std::strlen("<staged>")), 75);
  REQUIRE(!cc_xml_doc_parse_buffer_partial_done_x(doc, "x", too_large), 76);
  REQUIRE(cc_xml_doc_get_root(doc) == committed, 77);
  REQUIRE(cc_xml_doc_get_current(doc) == committed, 78);
  REQUIRE(cc_xml_doc_parse_buffer_partial_x(doc, "<staged>",
                                            std::strlen("<staged>")), 79);
  REQUIRE(!cc_xml_doc_parse_buffer_partial_x(doc, "x", too_large), 80);
  REQUIRE(cc_xml_doc_get_root(doc) == committed, 81);
  REQUIRE(cc_xml_doc_get_current(doc) == committed, 82);
  if (sizeof(size_t) > sizeof(unsigned int)) {
    // A simple int overflow may already fail in Expat.  This value wraps
    // to a valid short XML length on 64-bit targets, so the old cast could
    // silently accept the wrong amount of input.
    const size_t wrapped = static_cast<size_t>(UINT_MAX) + 1 +
      std::strlen("<wrap/>");
    REQUIRE(!cc_xml_doc_read_buffer_x(doc, "<wrap/>", wrapped), 83);
    REQUIRE(cc_xml_doc_get_root(doc) == committed, 84);
    REQUIRE(cc_xml_doc_parse_buffer_partial_x(doc, "<staged>",
                                              std::strlen("<staged>")), 85);
    const size_t wrapped_final = static_cast<size_t>(UINT_MAX) + 1 +
      std::strlen("</staged>");
    REQUIRE(!cc_xml_doc_parse_buffer_partial_done_x(doc, "</staged>",
                                                     wrapped_final), 86);
    REQUIRE(cc_xml_doc_get_root(doc) == committed, 87);
    REQUIRE(cc_xml_doc_get_current(doc) == committed, 88);
  }

  // Both successful and failed parsers must be reusable.
  REQUIRE(cc_xml_doc_read_buffer_x(doc, "<new/>", std::strlen("<new/>")), 15);
  REQUIRE(root_has_type(doc, "new"), 16);
  REQUIRE(cc_xml_doc_get_current(doc) == NULL, 17);

  // A partial parse stays private until done and rolls back on final error.
  committed = cc_xml_doc_get_root(doc);
  cc_xml_doc_set_current_x(doc, committed);
  REQUIRE(cc_xml_doc_parse_buffer_partial_x(doc, "<staged>",
                                            std::strlen("<staged>")), 20);
  REQUIRE(cc_xml_doc_get_root(doc) == committed, 21);
  REQUIRE(cc_xml_doc_get_current(doc) == committed, 22);
  REQUIRE(!cc_xml_doc_parse_buffer_partial_done_x(doc, "</broken>",
                                                  std::strlen("</broken>")), 23);
  REQUIRE(cc_xml_doc_get_root(doc) == committed, 24);
  REQUIRE(cc_xml_doc_get_current(doc) == committed, 25);

  // Starting another whole read abandons an unfinished partial transaction.
  REQUIRE(cc_xml_doc_parse_buffer_partial_x(doc, "<abandoned>",
                                            std::strlen("<abandoned>")), 26);
  REQUIRE(!cc_xml_doc_read_file_x(doc, missingpath), 27);
  REQUIRE(cc_xml_doc_get_root(doc) == committed, 28);
  REQUIRE(cc_xml_doc_get_current(doc) == committed, 29);

  // Retry the partial API after failures, then reuse it again after success.
  REQUIRE(cc_xml_doc_parse_buffer_partial_x(doc, "<fresh>",
                                            std::strlen("<fresh>")), 30);
  REQUIRE(cc_xml_doc_get_root(doc) == committed, 31);
  REQUIRE(cc_xml_doc_parse_buffer_partial_done_x(doc, "</fresh>",
                                                 std::strlen("</fresh>")), 32);
  REQUIRE(root_has_type(doc, "fresh"), 33);
  REQUIRE(cc_xml_doc_get_current(doc) == NULL, 34);
  REQUIRE(cc_xml_doc_read_buffer_x(doc, "<after-partial/>",
                                   std::strlen("<after-partial/>")), 35);
  REQUIRE(root_has_type(doc, "after-partial"), 36);

  // File failures, including empty, multichunk and real read errors, must
  // preserve the previous DOM and filename. The directory case exercises
  // ferror() on POSIX and fopen() failure on platforms rejecting directories.
  REQUIRE(cc_xml_doc_read_file_x(doc, validpath), 40);
  REQUIRE(root_has_type(doc, "file-old"), 41);
  REQUIRE(std::strcmp(cc_xml_doc_get_filename(doc), validpath) == 0, 42);
  committed = cc_xml_doc_get_root(doc);
  cc_xml_doc_set_current_x(doc, committed);

  REQUIRE(!cc_xml_doc_read_file_x(doc, malformedpath), 43);
  REQUIRE(cc_xml_doc_get_root(doc) == committed, 44);
  REQUIRE(cc_xml_doc_get_current(doc) == committed, 45);
  REQUIRE(std::strcmp(cc_xml_doc_get_filename(doc), validpath) == 0, 46);

  REQUIRE(!cc_xml_doc_read_file_x(doc, emptypath), 47);
  REQUIRE(cc_xml_doc_get_root(doc) == committed, 48);
  REQUIRE(cc_xml_doc_get_current(doc) == committed, 49);
  REQUIRE(std::strcmp(cc_xml_doc_get_filename(doc), validpath) == 0, 50);

  REQUIRE(!cc_xml_doc_read_file_x(doc, COIN_XML_TEST_DIRECTORY), 51);
  REQUIRE(cc_xml_doc_get_root(doc) == committed, 52);
  REQUIRE(cc_xml_doc_get_current(doc) == committed, 53);
  REQUIRE(std::strcmp(cc_xml_doc_get_filename(doc), validpath) == 0, 54);

  REQUIRE(!cc_xml_doc_read_file_x(doc, largemalformedpath), 55);
  REQUIRE(cc_xml_doc_get_root(doc) == committed, 56);
  REQUIRE(cc_xml_doc_get_current(doc) == committed, 57);
  REQUIRE(std::strcmp(cc_xml_doc_get_filename(doc), validpath) == 0, 58);

  REQUIRE(cc_xml_doc_read_file_x(doc, largevalidpath), 59);
  REQUIRE(root_has_type(doc, "large"), 60);
  REQUIRE(cc_xml_doc_get_current(doc) == NULL, 61);
  REQUIRE(std::strcmp(cc_xml_doc_get_filename(doc), largevalidpath) == 0, 62);

  REQUIRE(cc_xml_doc_read_file_x(doc, validpath), 63);
  REQUIRE(root_has_type(doc, "file-old"), 64);

cleanup:
  if (doc != NULL) cc_xml_doc_delete_x(doc);
  std::remove(validpath);
  std::remove(malformedpath);
  std::remove(emptypath);
  std::remove(largemalformedpath);
  std::remove(largevalidpath);
  return result;
}
