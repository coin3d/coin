#include <Inventor/C/base/list.h>

#include <cstdio>
#include <cstring>

#if defined(__unix__) || defined(__APPLE__)
#include <sys/resource.h>
#endif
#if defined(_MSC_VER)
#include <crtdbg.h>
#include <cstdlib>
#endif

// Do not use assert for the test oracle: NDEBUG is one of the tested modes.
#define CHECK(condition) do { \
  if (!(condition)) { \
    std::fprintf(stderr, "contract check failed at line %d: %s\n", \
                 __LINE__, #condition); \
    return 1; \
  } \
} while (false)

int
main(int argc, char ** argv)
{
#if defined(__unix__) || defined(__APPLE__)
  const struct rlimit no_core = { 0, 0 };
  setrlimit(RLIMIT_CORE, &no_core);
#endif
#if defined(_MSC_VER)
  // Keep expected assertion failures noninteractive on Windows CI.
  _set_error_mode(_OUT_TO_STDERR);
  _set_abort_behavior(0, _WRITE_ABORT_MSG | _CALL_REPORTFAULT);
#if defined(_DEBUG)
  _CrtSetReportMode(_CRT_ASSERT, _CRTDBG_MODE_FILE);
  _CrtSetReportFile(_CRT_ASSERT, _CRTDBG_FILE_STDERR);
#endif
#endif
  CHECK(argc == 2);
  const char * scenario = argv[1];
  int values[] = { 0, 1, 2, 3, 4, 5, 6 };
  cc_list * list = cc_list_construct_sized(8);
  CHECK(list != NULL);
  cc_list_append(list, &values[0]);
  cc_list_append(list, &values[1]);
  void ** storage = cc_list_get_array(list);

  if (std::strcmp(scenario, "valid") == 0) {
    // NULL is a valid stored item; only the list handle must be non-NULL.
    cc_list_append(list, NULL);
    CHECK(cc_list_find(list, NULL) == 2);
    CHECK(cc_list_get(list, 2) == NULL);
    cc_list * nullcopy = cc_list_clone(list);
    CHECK(nullcopy != NULL);
    CHECK(cc_list_pop(nullcopy) == NULL);
    CHECK(cc_list_get_length(nullcopy) == 2);
    cc_list_destruct(nullcopy);
    CHECK(cc_list_pop(list) == NULL);
    CHECK(cc_list_get_length(list) == 2);
    cc_list_insert(list, NULL, 0);
    cc_list_remove_item(list, NULL);
    CHECK(cc_list_get_length(list) == 2);
    cc_list_insert(list, &values[2], 0);
    cc_list_insert(list, &values[3], 3);
    CHECK(cc_list_get_length(list) == 4);
    CHECK(cc_list_get(list, 0) == &values[2]);
    CHECK(cc_list_get(list, 3) == &values[3]);
    cc_list_remove_item(list, &values[0]);
    cc_list_remove_fast(list, 0);
    CHECK(cc_list_get_length(list) == 2);
    CHECK(cc_list_get(list, 0) == &values[3]);
    CHECK(cc_list_get(list, 1) == &values[1]);
    cc_list_truncate(list, 2);
    cc_list_truncate_fit(list, 1);
    cc_list_push(list, &values[4]);
    CHECK(cc_list_pop(list) == &values[4]);
    CHECK(cc_list_pop(list) == &values[3]);
    cc_list_truncate(list, 0);
    cc_list_truncate_fit(list, 0);
    // Cross the four-element inline capacity after clearing and fitting.
    for (int i = 0; i < 7; ++i) cc_list_append(list, &values[i]);
    cc_list * copy = cc_list_clone(list);
    CHECK(copy != NULL);
    for (int i = 6; i >= 0; --i) CHECK(cc_list_pop(copy) == &values[i]);
    CHECK(cc_list_get_length(list) == 7);
    cc_list_destruct(copy);
    cc_list_destruct(list);
    return 0;
  }

  // Invalid handles are diagnosed only in extra-debug/assertion mode.
  // The other modes do not promise that NULL list handles are usable.
  if (std::strncmp(scenario, "null-", 5) == 0) {
    cc_list_destruct(list);
    if (std::strcmp(scenario, "null-clone") == 0) cc_list_clone(NULL);
    else if (std::strcmp(scenario, "null-destruct") == 0) cc_list_destruct(NULL);
    else if (std::strcmp(scenario, "null-append") == 0) cc_list_append(NULL, NULL);
    else if (std::strcmp(scenario, "null-find") == 0) cc_list_find(NULL, NULL);
    else if (std::strcmp(scenario, "null-insert") == 0) cc_list_insert(NULL, NULL, 0);
    else if (std::strcmp(scenario, "null-remove") == 0) cc_list_remove(NULL, 0);
    else if (std::strcmp(scenario, "null-remove-item") == 0) cc_list_remove_item(NULL, NULL);
    else if (std::strcmp(scenario, "null-remove-fast") == 0) cc_list_remove_fast(NULL, 0);
    else if (std::strcmp(scenario, "null-fit") == 0) cc_list_fit(NULL);
    else if (std::strcmp(scenario, "null-truncate") == 0) cc_list_truncate(NULL, 0);
    else if (std::strcmp(scenario, "null-truncate-fit") == 0) cc_list_truncate_fit(NULL, 0);
    else if (std::strcmp(scenario, "null-get-length") == 0) cc_list_get_length(NULL);
    else if (std::strcmp(scenario, "null-get-array") == 0) cc_list_get_array(NULL);
    else if (std::strcmp(scenario, "null-get") == 0) cc_list_get(NULL, 0);
    else if (std::strcmp(scenario, "null-push") == 0) cc_list_push(NULL, NULL);
    else if (std::strcmp(scenario, "null-pop") == 0) cc_list_pop(NULL);
    else CHECK(false);
    // Returning normally would mean that the expected diagnostic was lost.
    return 0;
  }

  if (std::strcmp(scenario, "get-negative") == 0) CHECK(cc_list_get(list, -1) == NULL);
  else if (std::strcmp(scenario, "get-end") == 0) CHECK(cc_list_get(list, 2) == NULL);
  else if (std::strcmp(scenario, "insert-negative") == 0) cc_list_insert(list, &values[2], -1);
  else if (std::strcmp(scenario, "insert-past-end") == 0) cc_list_insert(list, &values[2], 3);
  else if (std::strcmp(scenario, "remove-negative") == 0) cc_list_remove(list, -1);
  else if (std::strcmp(scenario, "remove-end") == 0) cc_list_remove(list, 2);
  else if (std::strcmp(scenario, "remove-fast-negative") == 0) cc_list_remove_fast(list, -1);
  else if (std::strcmp(scenario, "remove-fast-end") == 0) cc_list_remove_fast(list, 2);
  else if (std::strcmp(scenario, "truncate-negative") == 0) cc_list_truncate(list, -1);
  else if (std::strcmp(scenario, "truncate-past-end") == 0) cc_list_truncate(list, 3);
  else if (std::strcmp(scenario, "truncate-fit-negative") == 0) cc_list_truncate_fit(list, -1);
  else if (std::strcmp(scenario, "truncate-fit-past-end") == 0) cc_list_truncate_fit(list, 3);
  else if (std::strcmp(scenario, "remove-missing") == 0) cc_list_remove_item(list, &values[2]);
  else if (std::strcmp(scenario, "pop-empty") == 0) {
    cc_list_truncate(list, 0);
    CHECK(cc_list_pop(list) == NULL);
    CHECK(cc_list_get_length(list) == 0);
    cc_list_destruct(list);
    return 0;
  }
  else CHECK(false);

  CHECK(cc_list_get_length(list) == 2);
  CHECK(cc_list_get_array(list) == storage);
  CHECK(cc_list_get(list, 0) == &values[0]);
  CHECK(cc_list_get(list, 1) == &values[1]);
  cc_list_destruct(list);
  return 0;
}
