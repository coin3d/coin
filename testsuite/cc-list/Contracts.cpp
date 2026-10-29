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
