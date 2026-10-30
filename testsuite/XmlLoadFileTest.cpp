#include "xml/utils.h"

#include <cstdio>
#include <cstring>

int
main()
{
  char * unexpected = cc_xml_load_file("");
  if (unexpected) {
    delete [] unexpected;
    return 1;
  }

  const char * path = "coin_xml_load_file_fixture.bin";
  const char data[] = { 'a', '\0', 'b', '\n' };
  FILE * file = fopen(path, "wb");
  if (!file) return 2;
  const size_t written = fwrite(data, 1, sizeof(data), file);
  const int closeerror = fclose(file);
  if (written != sizeof(data) || closeerror != 0) {
    remove(path);
    return 3;
  }

  char * loaded = cc_xml_load_file(path);
  remove(path);
  if (!loaded) return 4;
  const bool matches = memcmp(loaded, data, sizeof(data)) == 0 &&
    loaded[sizeof(data)] == '\0';
  delete [] loaded;
  if (!matches) return 5;

  file = fopen(path, "wb");
  if (!file) return 6;
  if (fclose(file) != 0) {
    remove(path);
    return 7;
  }
  loaded = cc_xml_load_file(path);
  remove(path);
  if (!loaded) return 8;
  const bool empty = loaded[0] == '\0';
  delete [] loaded;
  if (!empty) return 9;

  file = fopen(path, "wb+");
  if (!file) return 10;
  if (fwrite(data, 1, sizeof(data), file) != sizeof(data) ||
      fseek(file, 0, SEEK_SET) != 0) {
    fclose(file);
    remove(path);
    return 11;
  }
  loaded = cc_xml_read_exact_file(file, sizeof(data) + 1);
  fclose(file);
  remove(path);
  if (loaded) {
    delete [] loaded;
    return 12;
  }
  return 0;
}
